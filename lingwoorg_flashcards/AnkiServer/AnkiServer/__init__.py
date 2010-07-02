
from webob.dec import wsgify
from webob.exc import *
from webob import Response

import anki
from anki.facts import Fact
from anki.stdmodels import BasicModel
from anki.models import Model, CardModel, FieldModel

try:
    import simplejson as json
except ImportError:
    import json

import os

def LingwoModel():
    m = Model(u'Lingwo')
    # we can only guarantee that the Front will be unique because it will
    # be based on the headword, language, pos.  The Back could be anything!
    m.addFieldModel(FieldModel(u'Front', True, True))
    # while I think that Back should be required, I don't really want this to
    # fail just because of that!!
    m.addFieldModel(FieldModel(u'Back', False, False))
    m.addFieldModel(FieldModel(u'Lingwo ID', True, True))
    m.addCardModel(CardModel(u'Forward', u'%(Front)s', u'%(Back)s'))
    m.addCardModel(CardModel(u'Reverse', u'%(Back)s', u'%(Front)s'))

    m.tags = u"Lingwo"
    return m

class AnkiServerApp(object):
    """ Our WSGI app. """

    def __init__(self, data_root, allowed_hosts):
        self.data_root = os.path.abspath(data_root)
        self.allowed_hosts = allowed_hosts

    def _get_path(self, path):
        npath = os.path.normpath(os.path.join(self.data_root, path))
        if npath[0:len(self.data_root)] != self.data_root:
            # attempting to escape our data jail!
            raise HTTPBadRequest('"%s" is not a valid path/id' % path)
        return npath

    def create_deck(self, path):
        full_path = self._get_path(path)
        if os.path.exists(full_path):
            raise HTTPBadRequest('"%s" already exists' % path)

        deck = anki.DeckStorage.Deck(full_path)
        try:
            deck.initUndo()
            deck.addModel(LingwoModel())
            deck.save()
        finally:
            deck.close()

        return {'id':path}

    def _open_deck(self, path):
        full_path = self._get_path(path)
        if not os.path.exists(full_path):
            raise HTTPBadRequest('"%s" doesn\'t exist' % path)

        return anki.DeckStorage.Deck(full_path)

    def open_deck(self, path):
        # verify that it really exists and is an Anki file
        self._open_deck(path).close()

        return {'id':path}

    def _output_fact(self, fact):
        res = dict(zip(fact.keys(), fact.values()))
        res['id'] = str(fact.id)
        return res

    def add_fact(self, deck_id, fields):
        deck = self._open_deck(deck_id)

        try:
            fact = deck.newFact()
            try:
                for key in fact.keys():
                    fact[key] = unicode(fields[key])
            except KeyError, e:
                raise HTTPBadRequest(e)

            deck.addFact(fact)
            deck.save()

            res = self._output_fact(fact)
        finally:
            deck.close()

        return res

    def save_fact(self, deck_id, fact):
        deck = self._open_deck(deck_id)
        
        try:
            newFact = deck.s.query(Fact).get(int(fact['id']))
            for key in newFact.keys():
                newFact[key] = fact[key]

            newFact.setModified(textChanged=True)
            deck.setModified()
            deck.save()
        finally:
            deck.close()

    def find_fact(self, deck_id, lingwo_id):
        deck = self._open_deck(deck_id)

        try:
            # first we need to find a field model id for a field named "Lingwo ID"
            fieldModelId = deck.s.scalar("SELECT id FROM fieldModels WHERE name = :name", name=u'Lingwo ID')
            if not fieldModelId:
                raise HTTPBadRequest("No field model named 'Lingwo ID'")

            # then we search for a fact with this field set to the given id
            factId = deck.s.scalar("""
                SELECT factId FROM fields WHERE fieldModelId = :fieldModelId AND
                    value = :lingwoId""", fieldModelId=fieldModelId, lingwoId=lingwo_id)
            if not factId:
                # we need to signal somehow to the calling application that no such
                # deck exists, but without it being considered a "bad error".  404 is 
                # inappropriate that refers to the resource (ie. /find_fact) which is
                # here obviously.
                return None

            fact = deck.s.query(Fact).get(factId)

            res = self._output_fact(fact)
        finally:
            deck.close()

        return res

    def delete_fact(self, deck_id, fact_id):
        deck = self._open_deck(deck_id)
        try:
            deck.deleteFact(int(fact_id))
            deck.save()
        finally:
            deck.close()

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
    app = translogger.TransLogger(app)
    return app

