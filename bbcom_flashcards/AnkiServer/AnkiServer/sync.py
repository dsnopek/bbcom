
from webob.dec import wsgify
from webob.exc import *
from webob import Response

import anki
from anki.sync import HttpSyncServer, CHUNK_SIZE
from anki.db import sqlite
from anki.utils import checksum

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

# TODO: this works globally, we need to work per-user
class SyncApp(HttpSyncServer):
    operations = ['getDecks','summary','applyPayload','finish','createDeck','getOneWayPayload']

    def __init__(self, data_root, base_url='/'):
        HttpSyncServer.__init__(self)

        self.data_root = data_root
        self.base_url = base_url

        self._getDecks()

    def _getDecks(self):
        self.decks = {}

        # It is a dict of {'deckName':[modified,lastSync]}
        for fn in os.listdir(self.data_root):
            if len(fn) > 5 and fn[-5:] == '.anki':
                conn = sqlite.connect(fn)
                cur = conn.cursor()
                cur.execute("select modified, lastSync from decks")

                self.decks[fn[:-5]] = list(cur.fetchone())

                cur.close()
                conn.close()

    def getDecks(self, libanki, client, sources, pversion):
        self._getDecks()
        return HttpSyncServer.getDecks(self, libanki, client, sources, pversion)

    def createDeck(self, name):
        # The HttpSyncServer.createDeck doesn't return a value value!  This seems to be
        # a bug in libanki.sync ...
        return self.stuff({"status": "OK"})

    def finish(self):
        # The HttpSyncServer has no finish() function...  I can only assume this is a bug too!
        return self.stuff("OK")

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
            if url in self.operations:
                try:
                    d = req.str_params.getone('d')
                    # AnkiDesktop actually passes us the string value 'None'!
                    if d != 'None':
                        self.deck = anki.DeckStorage.Deck(d+'.anki')
                except KeyError:
                    pass

                func = getattr(self, url)
                args = makeArgs(req.str_params)
                resp = Response(status='200 OK', content_type='application/json', content_encoding='deflate', body=func(**args))

                if self.deck is not None:
                    self.deck.save()
                    self.deck.close()
                    self.deck = None

                return resp
            elif url == 'fulldown':
                d = unicode(req.str_params['d'], 'utf-8')+'.anki'

                # set the syncTime before we send it
                c = sqlite.connect(d)
                lastSync = time.time()
                c.execute("update decks set lastSync = ?", [lastSync])
                c.commit()
                c.close()

                return Response(status='200 OK', content_type='application/octet-stream', content_encoding='deflate', content_disposition='attachment; filename="'+d+'"', app_iter=FileIterable(d))
            elif url == 'fullup':
                d = unicode(req.str_params['d'], 'utf-8')
                path = os.path.abspath(d+'.anki')
                infile = req.str_params['deck'].file
                lastSync = self._fullup(path, infile)
                return Response(status='200 OK', content_type='application/text', body='OK '+str(lastSync))

        return Response(status='200 OK', content_type='text/plain', body='Anki Server')

# Our entry point
def make_app(global_conf, **local_conf):
    return SyncApp(data_root=local_conf.get('data_root', '.'))

def main():
    from wsgiref.simple_server import make_server
    from AnkiServer.deck import thread_pool

    ankiserver = DeckApp('.', '/sync/')
    httpd = make_server('', 8001, ankiserver)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print "Exiting ..."
    finally:
        thread_pool.shutdown()

if __name__ == '__main__': main()

