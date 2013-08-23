
import MySQLdb

from AnkiServer.apps.sync_app import SyncApp

class BibliobirdSyncApp(SyncApp):
    def __init__(self, *args, **kw):
        SyncApp.__init__(self, *args, **kw)

        # setup mysql connection
        mysql_args = {}
        for k, v in kw.items():
            if k.startswith('mysql.'):
                mysql_args[k[6:]] = v
        self.mysql_args = mysql_args
        self.conn = None

        # get SQL statements
        self.sql_check_password = kw.get('sql_check_password')
        self.sql_username2dirname = kw.get('sql_username2dirname')

    def authenticate(self, username, password):
        if len(self.mysql_args) > 0 and self.sql_check_password is not None:
            cur = self._execute_sql(self.sql_check_password, (username, password))
            row = cur.fetchone()
            return row is not None

        return True

    def username2dirname(self, username):
        if len(self.mysql_args) > 0 and self.sql_username2dirname is not None:
            cur = self._execute_sql(self.sql_username2dirname, (username,))
            row = cur.fetchone()
            if row is None:
                return None
            return str(row[0])

        return username

# Our entry point
def make_app(global_conf, **local_conf):
    return SyncApp(**local_conf)

