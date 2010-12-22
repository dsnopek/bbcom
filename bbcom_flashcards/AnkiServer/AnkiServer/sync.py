
from webob.dec import wsgify
from webob.exc import *
from webob import Response

import anki
from anki.sync import HttpSyncServer, CHUNK_SIZE
from anki.db import sqlite
from anki.utils import checksum

import AnkiServer.deck

import os, zlib, tempfile, time

def makeArgs(mdict):
    d = dict(mdict.items())
    # TODO: use password/username/version for something?
    for k in ['p','u','v','d']:
        if d.has_key(k):
            del d[k]
    return d

class FileIterable(object):
    def __init__(self, fn):
        self.fn = fn
    def __iter__(self):
        return FileIterator(self.fn)

class FileIterator(object):
    def __init__(self, fn):
        self.fn = fn
        self.fo = open(self.fn, 'rb')
        self.c = zlib.compressobj()
        self.flushed = False
    def __iter__(self):
        return self
    def next(self):
        data = self.fo.read(CHUNK_SIZE)
        if not data:
            if not self.flushed:
                self.flushed = True
                return self.c.flush()
            else:
                raise StopIteration
        return self.c.compress(data)

class UserSyncServer(HttpSyncServer):
    operations = ['getDecks','summary','applyPayload','finish','createDeck','getOneWayPayload']

    def __init__(self, data_root):
        self.data_root = data_root
        HttpSyncServer.__init__(self)

    def getDecks(self, libanki, client, sources, pversion):
        self.decks = {}

        # It is a dict of {'deckName':[modified,lastSync]}
        for fn in os.listdir(self.data_root):
            if len(fn) > 5 and fn[-5:] == '.anki':
                d = os.path.join(self.data_root, fn)
                AnkiServer.deck.thread_pool.lock(d)
                try:
                    conn = sqlite.connect(d)
                    cur = conn.cursor()
                    cur.execute("select modified, lastSync from decks")

                    self.decks[fn[:-5]] = list(cur.fetchone())

                    cur.close()
                    conn.close()
                finally:
                    AnkiServer.deck.thread_pool.unlock(d)

        return HttpSyncServer.getDecks(self, libanki, client, sources, pversion)

    def createDeck(self, name):
        # The HttpSyncServer.createDeck doesn't return a valid value!  This seems to be
        # a bug in libanki.sync ...
        return self.stuff({"status": "OK"})

    def finish(self):
        # The HttpSyncServer has no finish() function...  I can only assume this is a bug too!
        return self.stuff("OK")

class SyncApp(object):
    valid_urls = UserSyncServer.operations + ['fullup','fulldown']

    def __init__(self, data_root, base_url='/'):
        self.data_root = data_root

        if base_url[-1] != '/':
            base_url = base_url + '/'
        self.base_url = base_url

    def username2dirname(self, username):
        # TODO: in the future we need to make this call into the database.
        # I think we should put a SELECT statement into development.ini which
        # pulls the username from the Drupal db.
        # TODO: we need to provide a way to say that a user doesn't exist and
        # that we should deny them!  (Maybe if the above SELECT returns no rows?)
        return '1';

    def check_password(self, username, password):
        # TODO: in the future we need to make this call into the database.
        # I think we should put a SELECT statement into development.ini which
        # does the comparison.
        # TODO: we need to provide a way to say that a user doesn't exist and
        # that we should deny them!  (Maybe if the above SELECT returns no rows?)
        return True

    def _fullup(self, path, infile):
        # DRS: most of this function was graciously copied from anki.sync
        (fd, tmpname) = tempfile.mkstemp(dir=os.getcwd(), prefix="fullsync")
        outfile = open(tmpname, 'wb')
        decomp = zlib.decompressobj()
        while 1:
            data = infile.read(CHUNK_SIZE)
            if not data:
                outfile.write(decomp.flush())
                break
            outfile.write(decomp.decompress(data))
        infile.close()
        outfile.close()
        os.close(fd)
        # if we were successful, overwrite old deck
        os.unlink(path)
        os.rename(tmpname, path)
        # reset the deck name
        c = sqlite.connect(path)
        lastSync = time.time()
        c.execute("update decks set syncName = ?, lastSync = ?",
                  [checksum(path.encode("utf-8")), lastSync])
        c.commit()
        c.close()

        return lastSync

    @wsgify
    def __call__(self, req):
        if req.path.startswith(self.base_url):
            url = req.path[len(self.base_url):]
            if url not in self.valid_urls:
                raise HTTPNotFound()

            # get and check username and password
            try:
                u = req.str_params.getone('u')
                p = req.str_params.getone('p')
            except KeyError:
                raise HTTPBadRequest('Must pass username and password')
            if not self.check_password(u, p):
                raise HTTPBadRequest('Incorrect username or password')
            user_path = os.path.join(self.data_root, self.username2dirname(u))

            # get and lock the (optional) deck for this request
            d = None
            try:
                d = unicode(req.str_params.getone('d'), 'utf-8')
                # AnkiDesktop actually passes us the string value 'None'!
                if d == 'None':
                    d = None
            except KeyError:
                pass
            if d is not None:
                # get the full deck path name
                d = os.path.abspath(os.path.join(user_path, d)+'.anki')
                if d[:len(self.data_root)] != self.data_root:
                    raise HTTPBadRequest('Bad deck name')
                # lock the deck for the duration of this operation
                AnkiServer.deck.thread_pool.lock(d)

            try:
                if url in UserSyncServer.operations:
                    userSync = UserSyncServer(user_path)
                    try:
                        if d is not None:
                            userSync.deck = anki.DeckStorage.Deck(d+'.anki')

                        func = getattr(userSync, url)
                        args = makeArgs(req.str_params)
                        resp = Response(status='200 OK', content_type='application/json', content_encoding='deflate', body=func(**args))
                    finally:
                        if userSync.deck is not None:
                            userSync.deck.save()
                            userSync.deck.close()
                elif url == 'fulldown':
                    # set the syncTime before we send it
                    c = sqlite.connect(d)
                    lastSync = time.time()
                    c.execute("update decks set lastSync = ?", [lastSync])
                    c.commit()
                    c.close()

                    resp = Response(status='200 OK', content_type='application/octet-stream', content_encoding='deflate', content_disposition='attachment; filename="'+os.path.basename(d)+'"', app_iter=FileIterable(d))
                elif url == 'fullup':
                    infile = req.str_params['deck'].file
                    lastSync = self._fullup(d, infile)
                    resp = Response(status='200 OK', content_type='application/text', body='OK '+str(lastSync))
            finally:
                if d is not None:
                    AnkiServer.deck.thread_pool.unlock(d)

            return resp


        return Response(status='200 OK', content_type='text/plain', body='Anki Server')

# Our entry point
def make_app(global_conf, **local_conf):
    return SyncApp(
        data_root=local_conf.get('data_root', '.'),
        base_url=local_conf.get('base_url', '/'),
    )

def main():
    from wsgiref.simple_server import make_server

    ankiserver = DeckApp('.', '/sync/')
    httpd = make_server('', 8001, ankiserver)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print "Exiting ..."
    finally:
        AnkiServer.deck.thread_pool.shutdown()

if __name__ == '__main__': main()

