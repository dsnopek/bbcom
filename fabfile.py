
from __future__ import with_statement
from fabric.api import *
import os, datetime

# I'm a little weary of this being here by default, since I don't want to accidently
# do something on production (with the exception of pulling data down for testing)
prod_host = 'code.hackyourlife.org'
#env.hosts = [prod_host]

env.local_prj_dir = env.remote_prj_dir = '/home/dsnopek/prj'
env.local_drupal_dir = env.remote_drupal_dir = env.remote_prj_dir+'/lingwo/drupal'
env.local_python_env_dir = env.local_python_env_dir = env.remote_prj_dir+'/lingwo/python-env'

env.repos = ['lingwoorg','lingwo_dictionary','linguatrek','drupal']

class _Drush(object):
    def __init__(self, remote=False):
        self.dir = env.remote_drupal_dir if remote else env.local_drupal_dir
        self.func = run if remote else local
        self.remote = remote

    def run(self, *args):
        # quote the arguments
        def quote(s):
            return '"'+s.replace('"', '\\"')+'"'
        args = [quote(x) for x in args]

        with cd(self.dir):
            res = self.func(" ".join(["drush"]+args))
        return res

    def vset(self, **kw):
        for k, v in kw.items():
            self.run('vset', '--yes', k, v)

    def sql_query(self, s):
        self.run('sql-query', s)

    def cc(self, spec='all'):
        self.run('cc', spec) 

    def en(self, *args):
        for mod in args:
            self.run('en', mod, '--yes')

    def dis(self, *args):
        for mod in args:
            self.run('dis', mod, '--yes')

def _python_env_requirements(limit_to=None):
    f = open('python-env-requirements.txt', 'rt')
    versions = []
    for line in f.readlines():
        if line.startswith('#'):
            continue
        if line == '\n':
            continue
        line = line[:-1]
        if limit_to is not None and line.split('==')[0] not in limit_to:
            continue

        versions.append(line)
    return versions

# TODO: these should be put into some kind of object so it can go both local and remote
def _pip(*args):
    local(os.path.join(env.local_python_env_dir, 'bin', 'pip')+' '+' '.join(args), capture=False)
def _python(*args):
    local(os.path.join(env.local_python_env_dir, 'bin', 'python')+' '+' '.join(args), capture=False)

@hosts(prod_host)
def pull_live_db():
    """Pull data from the live database and set it up here for testing"""

    backup = 'bibliobird-backup.mysql.gz'

    with cd(env.remote_drupal_dir):
        with hide('stdout','stderr'):
            run("drush bam-backup db manual 60a4968e1a793e5a8a20fa52644244e2")
        get("{0}/lingwo_backup/manual/{1}".format(env.remote_prj_dir,backup), "/tmp/")

    with cd(env.local_drupal_dir):
        local("zcat /tmp/{0} | drush sql-cli".format(backup))
        local("rm -f /tmp/{0}".format(backup))

    make_testing_safe()

def make_testing_safe():
    """Configures drupal such that it is safe to develop with it."""
    drush = _Drush(remote=False)

    drush.cc()

    drush.sql_query("UPDATE languages SET domain = 'http://en.localdomain:35637', prefix = '' WHERE language = 'en'")
    drush.sql_query("UPDATE languages SET domain = 'http://pl.localdomain:35637', prefix = '' WHERE language = 'pl'")

    drush.vset(
        language_negotiation="3",
        preprocess_css="0",
        preprocess_js="0",
        site_name="TESTING",
        bbcom_news_mailchimp_integration="0"
    )

    # disable scheduled backups
    drush.sql_query("DROP TABLE devel_queries; DROP TABLE devel_times")

    # disable dangerous modules
    drush.dis('googleanalytics')

    # enable the development module
    drush.sql_query("DROP TABLE devel_queries; DROP TABLE devel_times")
    drush.en('devel')
    drush.vset(smtp_library="sites/all/modules/devel/devel.module")

def branch(target, source='mainline'):
    """Branch all the repos to create a new family of branches."""

    for repo in env.repos:
        dest = os.path.join(env.local_prj_dir, 'lingwo', repo+'.'+target)
        if os.path.exists(dest):
            print("Not branching {0}, branch {1} already exists".format(repo, target))
            continue

        with cd(os.path.join(env.local_prj_dir, 'lingwo')):
            local('bzr branch {repo}.{source} {repo}.{target}'.format(repo=repo, source=source, target=target), capture=False)
        with cd(dest):
            remote = 'bzr+ssh://code.hackyourlife.org/home/dsnopek/bzr-lingwo/{repo}/{target}'.format(repo=repo, target=target)
            local('bzr push {0}'.format(remote), capture=False)
            local('bzr bind {0}'.format(remote), capture=False)

def activate_branch(branch):
    """Switch all our symlinks to point to the given family of branches."""

    for repo in env.repos:
        with cd(os.path.join(env.local_prj_dir, 'lingwo')):
            source = repo+'.'+branch
            if not os.path.exists(os.path.join(env.local_prj_dir, 'lingwo', source)):
                abort('Branch {0} doesn\'t exist'.format(repo+'.'+target))

            # also do the 'bbcom' repo if necessary
            links = [repo]
            if repo == 'lingwoorg':
                links.append('bbcom')

            for link in links:
                local('rm -f {0}'.format(link), capture=False)
                local('ln -s {0} {1}'.format(source, link), capture=False)

def merge(source, target='mainline', message=None):
    """Merge a family of branches into another family (by default, "mainline")."""

    base_message = 'Merging {0} branch.'.format(source) 
    if message is None:
        message = base_message
    else:
        message = base_message + '  ' + message
    
    for repo in env.repos:
        with cd(os.path.join(env.local_prj_dir, 'lingwo', repo+'.'+target)):
            output = local('bzr merge ../{0} 2>&1'.format(repo+'.'+source), capture=True)
            # TODO: we should also be able to test output.stderr
            if output != 'Nothing to do.':
                local('bzr commit -m "{0}"'.format(message.replace('"', '\\"')), capture=False)

def make_release(name):
    """Merges mainline into production and tags it for release."""

    # merge mainline into production
    merge('mainline', 'production', 'Creating release {0}'.format(name))

    # tag production with release name
    for repo in env.repos:
        with cd(os.path.join(env.local_prj_dir, 'lingwo', repo+'.production')):
            local('bzr tag release--{0}'.format(name))

@hosts(prod_host)
def deploy(release_name):
    drush = _Drush(remote=True)
    today = datetime.date.today()

    # backup the database
    drush.run('bam-backup', 'db', 'manual', 'default')

    # backup the code
    with cd(env.remote_prj_dir):
        run('tar -cjvpf lingwo-{0}.tar.bz2 lingwo'.format(today.strftime('%Y-%m-%d'))

    # check out the new code
    repos = env.repos[:]
    # TODO: for now, we have to deal with lingwoorg really living inside bbcom
    repos[repos.index('lingwoorg')] = 'bbcom'
    for repo in repos:
        with cd(os.path.join(env.remote_prj_dir, 'lingwo', repo):
            run('bzr up -r tag:{0}'.format(release_name))

    # update the database (should also force new dependencies to be enabled)
    drush.run('updatedb')

def create_bootstrap():
    """Creates the boostrap.py for bootstrapping our development environment."""

    # TODO: The bootstrap should also add this to bin/activate:
    #
    #  # DRS: make our fabfile always get called
    #  alias fab='fab -f /home/dsnopek/prj/lingwo/bbcom/fabfile.py'
    #
    # TODO: The bootstrap should initialize the top project directory
    # as a Bazaar repository, so that revisiions are shared between branches.
    #

    import virtualenv, textwrap

    deps = ['pip', 'virtualenv', 'Fabric', 'bzr', 'pycrypto', 'paramiko']
    deps = ', '.join(["'{0}'".format(x) for x in _python_env_requirements(deps)])

    output = virtualenv.create_bootstrap_script(textwrap.dedent("""
    import os, sys, subprocess

    def extend_parser(parser):
        for opt in ['clear','no-site-packages','unzip-setuptools','relocatable','distribute','setuptools']:
            if parser.has_option('--'+opt):
                parser.remove_option('--'+opt)

        parser.add_option('--bbcom-branch', dest='bbcom_branch', metavar='BRANCH',
            help='Location of the bbcom_branch',
            default='bzr+ssh://code.hackyourlife.org/home/dsnopek/bzr-lingwo/lingwoorg/refactor-1')

        parser.add_option('--only-virtualenv', dest='only_virtualenv', action='store_true',
            help='Only create the virtualenv, not the entire BiblioBird project directory',
            default=False)

    def adjust_options(options, args):
        if len(args) > 0:
            if os.path.exists(args[0]):
                print >> sys.stderr, "ERROR: %s already exists!" % args[0]
                sys.exit(1)
            if not options.only_virtualenv:
                os.mkdir(args[0])
                options.bbcom_project_dir = args[0]
                args[0] = join(args[0], 'python-env')

        options.no_site_packages = True
        options.prompt = 'bibliobird'

    def after_install(options, home_dir):
        subprocess.call([join(home_dir, 'bin', 'pip'),
            'install', '-U', {deps}])
        if not options.only_virtualenv:
            subprocess.call([join(home_dir, 'bin', 'bzr'),
                'co', options.bbcom_branch,
                join(options.bbcom_project_dir, 'bbcom')])
            subprocess.call([join(home_dir, 'bin', 'fab'),
                '-f', join(options.bbcom_project_dir, 'bbcom', 'fabfile.py'), 'bootstrap'],
                cwd=join(options.bbcom_project_dir, 'bbcom'))
    """.format(deps=deps)))

    f = open('bootstrap.py', 'w')
    f.write(output)
    f.close()

def setup_python_env():
    """Sets up our python environment."""
    # TODO: if the python-env doesn't exist, we should create it with the bootstrap script

    tarball_installs = {
        'nltk==2.0b9':    'http://nltk.googlecode.com/files/nltk-2.0b9.zip#egg=nltk',
        'html5lib==0.90': 'http://html5lib.googlecode.com/files/html5lib-0.90.zip#egg=html5lib',
    }
    anki_tarball = 'http://anki.googlecode.com/files/anki-1.0.1.tgz'
    requirements = _python_env_requirements()

    for i, r in enumerate(requirements[:]):
        if tarball_installs.has_key(r):
            requirements[i] = tarball_installs[r]
        elif r.split('==')[0] == 'anki':
            # we handle Anki special at the end
            del requirements[i]

    # we have to install PyYAML first due to a weird bug in nltk
    _pip('install', '-U', *_python_env_requirements(['PyYAML']))
    # install (almost) all the requirements
    _pip('install', '-U', *requirements)

    # install libanki special
    from tempfile import mkdtemp
    from shutil import rmtree
    tempdir = mkdtemp()
    try:
        fn = os.path.basename(anki_tarball)
        dn = os.path.splitext(fn)[0]

        with cd(tempdir):
            local('wget '+anki_tarball, capture=False)
            local('tar -xzvf '+fn, capture=False)
        with cd(os.path.join(tempdir, dn, 'libanki')):
            _python('setup.py', 'install')
    finally:
        rmtree(tempdir)

def bootstrap():
    """Takes an empty project directory, populates and sets everything up."""
    setup_python_env()

