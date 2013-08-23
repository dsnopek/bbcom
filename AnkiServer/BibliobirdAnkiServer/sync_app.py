
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
    
    def _connect_mysql(self):
        if self.conn is None and len(self.mysql_args) > 0:
            self.conn = MySQLdb.connect(**self.mysql_args)

    def _execute_sql(self, sql, args=()):
        self._connect_mysql()
        try:
            cur = self.conn.cursor()
            cur.execute(sql, args)
        except MySQLdb.OperationalError, e:
            if e.args[0] == 2006:
                # MySQL server has gone away message
                self.conn = None
                self._connect_mysql()
                cur = self.conn.cursor()
                cur.execute(sql, args)
        return cur

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

