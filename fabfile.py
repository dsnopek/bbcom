
from __future__ import with_statement
from fabric.api import *
import os, datetime

# I'm a little weary of this being here by default, since I don't want to accidently
# do something on production (with the exception of pulling data down for testing)
prod_host = 'webuser@bibliobird.com'
#env.hosts = [prod_host]

env.local_prj_dir = '/home/webuser/prj'
env.local_drupal_dir = env.local_prj_dir+'/bbcom/drupal'
env.local_python_env_dir = env.local_prj_dir+'/python-env'
env.local_drush_alias = '@en.master.bibliobird.vm'

env.remote_prj_dir = '/home/webuser/prj';
env.remote_drupal_dir = env.remote_prj_dir+'/bbcom/drupal'
env.remote_python_env_dir = env.remote_prj_dir+'/python-env'
env.remote_drush_alias = '@en.bibliobird.com'

env.repos = ['bbcom','lingwo','lingwo-old']

class _Drush(object):
    def __init__(self, remote=False):
        self.alias = env.remote_drush_alias if remote else env.local_drush_alias
        self.func = run if remote else local
        self.remote = remote

    def run(self, *args, **kw):
        # quote the arguments
        def quote(s):
            return '"'+s.replace('"', '\\"')+'"'
        args = [quote(x) for x in args]
        cmd = " ".join(["drush",self.alias]+args)
        res = None

        if not self.remote and kw.get('capture', True) is False:
            local(cmd, capture=False)
        else:
            res = self.func(cmd)

        return res

    def vset(self, **kw):
        for k, v in kw.items():
            self.run('vset', '--yes', k, v)

    def vdel(self, *args):
        for v in args:
            self.run('vdel', '--yes', v)

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

    drush = _Drush(remote=True)

    backup = 'bibliobird-backup.mysql.gz'

    with cd(env.remote_drupal_dir):
        with hide('stdout','stderr'):
            drush.run('bam-backup', 'db', 'manual', '60a4968e1a793e5a8a20fa52644244e2')
            #run("drush bam-backup db manual 60a4968e1a793e5a8a20fa52644244e2".format(env.remote_drush_alias))
        get("{0}/lingwo_backup/manual/{1}".format(env.remote_prj_dir,backup), "/tmp/")

    with cd(env.local_drupal_dir):
        #local("drush sqlq 'DROP DATABASE bibliobird; CREATE DATABASE bibliobird;'", capture=False)
        local("zcat /tmp/{0} | drush {1} sql-cli".format(backup, env.local_drush_alias))
        local("rm -f /tmp/{0}".format(backup))

    make_testing_safe()

def make_testing_safe():
    """Configures drupal such that it is safe to develop with it."""
    drush = _Drush(remote=False)

    # this seems to correct any problems we encounter after putting a foreign db in place
    drush.run('updatedb', '--yes', capture=False)

    with settings(warn_only=True):
        drush.cc()

    drush.sql_query("UPDATE languages SET domain = 'http://en.master.bibliobird.vm', prefix = '' WHERE language = 'en'")
    drush.sql_query("UPDATE languages SET domain = 'http://pl.master.bibliobird.vm', prefix = '' WHERE language = 'pl'")

    drush.vdel('language_default')

    drush.vset(
        language_negotiation="3",
        preprocess_css="0",
        preprocess_js="0",
        site_name="TESTING",
        bbcom_news_mailchimp_integration="0",
        bbcom_metrics_mixpanel_token="ec4433cb33d091816ce93058686b0ae8",
        bbcom_metrics_mixpanel_key="d151eb6fe740fe1c3d6392c78df25113",
        bbcom_metrics_mixpanel_secret="a59ee2473dee43341d97a0b1bc63bd96"
    )

    # disable notifications, so we can still test it, but we won't hit real users with e-mails
    drush.sql_query("DELETE FROM notifications; DELETE FROM notifications_fields; DELETE FROM notifications_queue;")
    drush.en('messaging_simple')

    # disable dangerous modules
    drush.dis('backup_migrate', 'mollom', 'googleanalytics', 'spambot')

    # enable the development module
    drush.sql_query("DROP TABLE devel_queries; DROP TABLE devel_times")
    drush.en('devel')
    drush.vset(smtp_library="sites/all/modules/devel/devel.module")

def branch(source=None, target=None):
    """Branch all the repos to create a new family of branches."""

    # We do some argument shuffling magic, so that we can specify one argument
    # to branch from 'master', specify both in a sane order...  Not very pythonic
    # but it stops me from losing my mind with how this is supposed to work!
    if source is None and target is None:
        raise TypeError('Must pass atleast one argument')
    if target is None:
        target = source
        source = 'master'
    if source is None:
        source = 'master'

    for repo in env.repos:
        with cd(os.path.join(env.local_prj_dir, 'bibliobird', repo)):
            local('git checkout -b {0} {1}'.format(target, source), capture=False)
            local('git push origin {0}'.format(target), capture=False)

            # TODO: can we simply use the --set-upstream argument of 'git push'
            # setup configuration for this branch
            fd = open(os.path.join(env.cwd, '.git', 'config'), 'at')
            fd.write('[branch "{0}"]\n'.format(target))
            fd.write('\tremote = origin\n')
            fd.write('\tmerge = refs/heads/{0}\n'.format(target))
            fd.close()

def checkout(branch):
    """Switch all our symlinks to point to the given family of branches."""

    for repo in env.repos:
        with cd(os.path.join(env.local_prj_dir, 'bibliobird', repo)):
            local('git checkout {0}'.format(branch))

def merge(source, target='master', message=None):
    """Merge a family of branches into another family (by default, "master")."""

    base_message = 'Merge branch \'{0}\''.format(source)
    if message is None:
        message = base_message
    else:
        message = base_message + '  ' + message

    # TODO: we need to check for uncommited changes!
    
    for repo in env.repos:
        with cd(os.path.join(env.local_prj_dir, 'bibliobird', repo)):
            local('git checkout {0}'.format(target), capture=False)
            #output = local('git merge --no-commit --squash {0}'.format(source), capture=True)
            output = local('git merge --no-commit {0}'.format(source), capture=True)
            if 'Already up-to-date' not in output:
                with settings(warn_only=True):
                    local('git commit -m "{0}"'.format(message.replace('"', '\\"')), capture=False)
                local('git push --all', capture=False)

def pull(branch=None):
    """Pull all the code on a given branch."""

    cmd = 'git pull'
    if branch is None:
        cmd += ' --all'

    for repo in env.repos:
        with cd(os.path.join(env.local_prj_dir, 'bibliobird', repo)):
            if branch is not None:
                local('git checkout {0}'.format(branch), capture=False)
            local(cmd, capture=False)

def push(branch=None):
    """Push all the code on a given branch."""

    cmd = 'git push'
    if branch is None:
        cmd += ' --all'

    for repo in env.repos:
        with cd(os.path.join(env.local_prj_dir, 'bibliobird', repo)):
            if branch is not None:
                local('git checkout {0}'.format(branch), capture=False)
            local(cmd, capture=False)

def tag_release(name):
    """Tags production for release."""

    # tag production with release name
    for repo in env.repos:
        with cd(os.path.join(env.local_prj_dir, 'bibliobird', repo)):
            local('git checkout production', capture=False)
            local('git tag release--{0}'.format(name), capture=False)
            local('git push --tags', capture=False)

def make_release(name):
    """Merges master into production and then tags for release."""

    # merge mainline into production
    merge('master', 'production', 'Creating release {0}'.format(name))
    
    tag_release(name)

@hosts(prod_host)
def backup_live_db():
    drush = _Drush(remote=True)
    drush.run('bam-backup', 'db', 'manual', 'default')

@hosts(prod_host)
def backup_live_code():
    today = datetime.date.today()
    with cd(env.remote_prj_dir):
        # TODO: we should specifically target certain directories!  We don't want to backup
        # "python-env" along with the other code.  It should be:
        #
        #   env.repos + ['anki_files']
        #   (remember to swap bbcom in if it hasn't been yet)
        #
        run('tar -cjvpf bibliobird-{0}.tar.bz2 bibliobird'.format(today.strftime('%Y-%m-%d')))

@hosts(prod_host)
def unsafe_deploy(release_name):
    """UNSAFE: Does deployment without first backing things up!"""

    # check out the new code
    for repo in env.repos:
        with cd(os.path.join(env.remote_prj_dir, 'bibliobird', repo)):
            run('git checkout tags/release--{0}'.format(release_name))

    # update the database (should also force new dependencies to be enabled)
    drush = _Drush(remote=True)
    drush.run('updatedb')


@hosts(prod_host)
def deploy(release_name):
    """Backs up db/code and the deploys a release."""
    backup_live_db()
    backup_live_code()
    unsafe_deploy(release_name)

def create_bootstrap():
    """Creates the boostrap.py for bootstrapping our development environment."""

    # TODO: The bootstrap should also add this to bin/activate:
    #
    #  # DRS: make our fabfile always get called
    #  alias fab='fab -f /home/dsnopek/prj/bibliobird/bbcom/fabfile.py'
    #
    # TODO: The bootstrap should initialize the top project directory
    # as a Bazaar repository, so that revisiions are shared between branches.
    #

    import virtualenv, textwrap

    deps = ['pip', 'virtualenv', 'Fabric', 'pycrypto', 'paramiko']
    deps = ', '.join(["'{0}'".format(x) for x in _python_env_requirements(deps)])

    output = virtualenv.create_bootstrap_script(textwrap.dedent("""
    import os, sys, subprocess

    def extend_parser(parser):
        for opt in ['clear','no-site-packages','unzip-setuptools','relocatable','distribute','setuptools']:
            if parser.has_option('--'+opt):
                parser.remove_option('--'+opt)

        parser.add_option('--bbcom-branch', dest='bbcom_branch', metavar='BRANCH',
            help='Location of the bbcom_branch',
            default='bzr+ssh://code.hackyourlife.org/home/dsnopek/bzr-lingwo/bbcom/mainline')

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
    anki_tarball = 'http://anki.googlecode.com/files/anki-1.2.8.tgz'
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

