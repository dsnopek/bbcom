
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

#    def create_deck(self, path):
#        if not self.decks.has_key(path):
#            full_path = self._get_path(path)
#            if os.path.exists(full_path):
#                raise HTTPBadRequest('"%s" already exists' % path)
#
#            # create our deck handler
#            deck_handler = DeckHandler(full_path)
#            deck_handler.start()
#            deck_handler.create_deck()
#            self.decks[path] = deck_handler
#
#        return {'id':path}
#
#    def open_deck(self, path):
#        # open the deck
#        self._open_deck(path)
#        return {'id':path}

    def _open_deck(self, path):
        full_path = self._get_path(path)
        #if not os.path.exists(full_path):
        #    raise HTTPBadRequest('"%s" doesn\'t exist' % path)

        if self.decks.has_key(path):
            deck = self.decks[path]
        else:
            deck = self.decks[path] = DeckHandler(full_path)
            deck.start()

        #deck.open_deck()
        return deck

    # these are now mostly noops because we open/create the deck when it
    # is requested to reduce the number of calls to this server
    def open_deck(self, path):
        self._open_deck(path)
        return {'id':path}
    create_deck = open_deck

    def add_fact(self, deck_id, fields):
        return self._open_deck(deck_id).add_fact(fields)

    def save_fact(self, deck_id, fact):
        return self._open_deck(deck_id).save_fact(fact)
        
    def find_fact(self, deck_id, bibliobird_id):
        return self._open_deck(deck_id).find_fact(bibliobird_id)

    def delete_fact(self, deck_id, fact_id):
        return self._open_deck(deck_id).delete_fact(fact_id)

    def get_card(self, deck_id):
        return self._open_deck(deck_id).get_card()

    def answer_card(self, deck_id, card_id, ease):
        return self._open_deck(deck_id).answer_card(card_id, ease)

    @wsgify
    def __call__(self, req):
        if self.allowed_hosts != '*':
            if req.remote_addr != self.allowed_hosts:
                raise HTTPForbidden()

        if req.method != 'POST':
            raise HTTPMethodNotAllowed(allow=['POST'])

        # get the function to call from the path
        func = req.path
        if func[0] == '/':
            func = func[1:]
        if '/' in func or func[0] == '_':
            raise HTTPNotFound()
        if not hasattr(self, func) or not callable(getattr(self, func)):
            raise HTTPNotFound()
        func = getattr(self, func)

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

        # TODO: maybe I should make this more RPC-like, and always return a structure
        # like {'returned': value} where now value could be null (in full JSON RPC-ness).
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
    # TODO: this should be configurable and, um, better.
    # TODO: we should setup the 'translogger' in the *.ini using PasteDeploy
    app = translogger.TransLogger(app)
    return app

def server_runner(app, global_conf, **kw):
    from paste.httpserver import server_runner as paste_server_runner
    try:
        paste_server_runner(app, global_conf, **kw)
    finally:
        shutdown()

