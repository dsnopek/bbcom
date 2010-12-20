
from webob.dec import wsgify
from webob.exc import *
from webob import Response

from AnkiServer.deck import DeckHandler

try:
    import simplejson as json
except ImportError:
    import json

import os

servers = []
def shutdown():
    global servers
    # clean up our application
    for server in servers:
        server.shutdown()
    servers = []

class AnkiServerApp(object):
    """ Our WSGI app. """

    def __init__(self, data_root, allowed_hosts):
        self.data_root = os.path.abspath(data_root)
        self.allowed_hosts = allowed_hosts
        self.decks = {}

        # add to the server list
        servers.append(self)

    def shutdown(self):
        for deck in self.decks.values():
            deck.stop()
        self.decks = {}

    def _get_path(self, path):
        npath = os.path.normpath(os.path.join(self.data_root, path))
        if npath[0:len(self.data_root)] != self.data_root:
            # attempting to escape our data jail!
            raise HTTPBadRequest('"%s" is not a valid path/id' % path)
        return npath

    def _open_deck(self, path):
        full_path = self._get_path(path)

        if self.decks.has_key(path):
            deck = self.decks[path]
        else:
            deck = self.decks[path] = DeckHandler(full_path)
            deck.start()

        return deck

    @wsgify
    def __call__(self, req):
        if self.allowed_hosts != '*':
            if req.remote_addr != self.allowed_hosts:
                raise HTTPForbidden()

        if req.method != 'POST':
            raise HTTPMethodNotAllowed(allow=['POST'])

        # get the deck and function to call from the path
        func = req.path
        if func[0] == '/':
            func = func[1:]
        parts = func.split('/')
        path = '/'.join(parts[:-1])
        func = parts[-1]
        if not DeckHandler.external_allowed(func):
            raise HTTPNotFound()
        deck = self._open_deck(path)
        func = getattr(deck, func)

        try:
            input = json.loads(req.body)
        except ValueError:
            raise HTTPBadRequest()
        # make the keys into non-unicode strings
        input = dict([(str(k), v) for k, v in input.items()])

        # debug
        from pprint import pprint
        pprint(input)

        # run it!
        output = func(**input)

        if output is None:
            return Response('', content_type='text/plain')
        else:
            return Response(json.dumps(output), content_type='application/json')

# Our entry point
def make_app(global_conf, **local_conf):
    from paste import translogger

    # setup the logger
    logging_config_file = local_conf.get('logging.config_file')
    if logging_config_file:
        import logging.config
        logging.config.fileConfig(logging_config_file)

    app = AnkiServerApp(
        data_root=local_conf.get('data_root', '.'),
        allowed_hosts=local_conf.get('allowed_hosts', '*')
    )
    # TODO: we should setup the 'translogger' in the *.ini using PasteDeploy
    app = translogger.TransLogger(app)
    return app

def server_runner(app, global_conf, **kw):
    from paste.httpserver import server_runner as paste_server_runner
    try:
        paste_server_runner(app, global_conf, **kw)
    finally:
        shutdown()

