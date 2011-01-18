
from __future__ import with_statement
from fabric.api import *
import os

# I'm a little weary of this being here by default, since I don't want to accidently
# do something on production (with the exception of pulling data down for testing)
prod_host = 'code.hackyourlife.org'
#env.hosts = [prod_host]

env.local_prj_dir = env.remote_prj_dir = '/home/dsnopek/prj'
env.local_drupal_dir = env.remote_drupal_dir = env.remote_prj_dir+'/lingwo/drupal'
env.local_python_env_dir = env.local_prj_dir = env.remote_prj_dir+'/lingwo/python-env'

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

def _python_env_requirements(eggs):
    f = open('python-env-requirements.txt', 'rt')
    versions = []
    for line in f.readlines():
        line = line[:-1]
        for egg in eggs:
            if line.startswith(egg):
                versions.append(line)
    return versions

def _pip(*args):
    local(os.path.join(env.local_python_env_dir, 'bin', 'pip')+' '+' '.join(args), capture=False)

def _get(source, destdir):
    pass

    def install_from_url(self, url):

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

def create_bootstrap():
    """Creates the boostrap.py for bootstrapping our development environment."""

    import virtualenv, textwrap

    deps = ['pip', 'virtualenv', 'fabric', 'bzr', 'pycrypto', 'paramiko']
    deps = ', '.join(["'{0}'".format(x) for x in _python_env_requirements(deps)])

    output = virtualenv.create_bootstrap_script(textwrap.dedent("""
    import os, sys, subprocess

    def extend_parser(parser):
        for opt in ['clear','no-site-packages','unzip-setuptools','relocatable','distribute','setuptools']:
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
    """.format(deps=deps)))

    f = open('bootstrap.py', 'w')
    f.write(output)
    f.close()

def setup_python_env():
    """Sets up our python environment."""
    # TODO: if the python-env doesn't exist, we should create it with the bootstrap script

    # due to a weirdness in nltk, we have to install PyYAML before the rest of the requirements
    _pip('install', _python_env_requirements('PyYAML')[0])
    _pip('install', '-r', 'python-env-requirements.txt')

