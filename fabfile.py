
from __future__ import with_statement
from fabric.api import *

# I'm a little weary of this being here by default, since I don't want to accidently
# do something on production (with the exception of pulling data down for testing)
prod_host = 'code.hackyourlife.org'
#env.hosts = [prod_host]

env.local_prj_dir = env.remote_prj_dir = '/home/dsnopek/prj'
env.local_drupal_dir = env.remote_drupal_dir = env.remote_prj_dir+'/lingwo/drupal'

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

