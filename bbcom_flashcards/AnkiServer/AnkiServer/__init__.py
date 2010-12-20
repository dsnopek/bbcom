
from webob.dec import wsgify
from webob.exc import *
from webob import Response

import anki
from anki.facts import Fact
from anki.stdmodels import BasicModel
from anki.models import Model, CardModel, FieldModel

from threading import Thread
from Queue import Queue, PriorityQueue

try:
    import simplejson as json
except ImportError:
    import json

import os

def BiblioBirdModel():
    m = Model(u'BiblioBird')
    # we can only guarantee that the Front will be unique because it will
    # be based on the headword, language, pos.  The Back could be anything!
    m.addFieldModel(FieldModel(u'Front', True, True))
    # while I think that Back should be required, I don't really want this to
    # fail just because of that!!
    m.addFieldModel(FieldModel(u'Back', False, False))
    m.addFieldModel(FieldModel(u'BiblioBird ID', True, True))
    m.addCardModel(CardModel(u'Forward', u'%(Front)s', u'%(Back)s'))
    m.addCardModel(CardModel(u'Reverse', u'%(Back)s', u'%(Front)s'))

    m.tags = u"BiblioBird"
    return m

def log_exception(func):
    import logging
    def newFunc(self, *args, **kw):
        try:
            return func(self, *args, **kw)
        except Exception, e:
            logging.error('DeckHandler[%s] Unable to %s(*%s, **%s): %s'
                % (self.path, func.func_name, repr(args), repr(kw), e))
            return e
    return newFunc

def defer_none(func, priority=0):
    func = log_exception(func)
    def newFunc(*args, **kw):
        self = args[0]
        self._queue.put((priority, func, args, kw, None))
    return newFunc

def defer(func, priority=0):
    func = log_exception(func)
    def newFunc(self, *args, **kw):
        return_queue = Queue()
        self = args[0]
        self._queue.put((priority, func, args, kw, return_queue))
        ret = return_queue.get(True)
        if isinstance(ret, Exception):
            raise e
        return ret
    return newFunc

def check_deck(func):
    def newFunc(self, *args, **kw):
        if self.deck is None:
            raise HTTPBadRequest('Cannot do %s with first open_deck or create_deck' % func.func_name)
        return func(self, *args, **kw)
    return newFunc

class DeckHandler(object):
    def __init__(self, path):
        self.path = path
        self.deck = None

        self._thread = Thread(target=self._run)
        self._queue = PriorityQueue()
        self._running = True

    def _run(self):
        while self._running:
            priority, func, args, kw, return_queue = self._queue.get(True)
            ret = func(*args, **kw)
            if return_queue is not None:
                return_queue.put(ret)

    def start(self):
        self._thread.start()

    @defer_none
    def stop(self):
        self.deck.save()
        self.deck.close()
        self._running = False

    @defer_none
    def create_deck(self):
        if self.deck is not None:
            return

        import errno

        # mkdir -p the path, because it might not exist
        dir = os.path.dirname(full_path)
        try:
            os.makedirs(dir)
        except OSError, exc:
            if exc.errno == errno.EEXIST:
                pass
            else:
                raise

        deck = anki.DeckStorage.Deck(full_path)
        try:
            deck.initUndo()
            deck.addModel(BiblioBirdModel())
            deck.save()
        except Exception, e:
            logging.error('Unable to create_deck(%s): %s' % (self.path, e))
            deck.close()
            deck = None
            raise e

        self.deck = deck

    @defer_none
    def open_deck(self):
        if self.deck is not None:
            return

        self.deck = anki.DeckStorage.Deck(self.path)

    @check_deck
    @defer_none
    def add_fact(self, fields):
        fact = self.deck.newFact()
        for key in fact.keys():
            fact[key] = unicode(fields[key])

        self.deck.addFact(fact)
        self.deck.save()

    @check_deck
    @defer_none
    def save_fact(self, fact):
        newFact = deck.s.query(Fact).get(int(fact['id']))
        for key in newFact.keys():
            newFact[key] = fact[key]

        newFact.setModified(textChanged=True)
        deck.setModified()
        deck.save()


class AnkiServerApp(object):
    """ Our WSGI app. """

    def __init__(self, data_root, allowed_hosts):
        self.data_root = os.path.abspath(data_root)
        self.allowed_hosts = allowed_hosts
        self.decks = {}

    def _get_path(self, path):
        npath = os.path.normpath(os.path.join(self.data_root, path))
        if npath[0:len(self.data_root)] != self.data_root:
            # attempting to escape our data jail!
            raise HTTPBadRequest('"%s" is not a valid path/id' % path)
        return npath

    def create_deck(self, path):
        if not self.decks.has_key(path):
            full_path = self._get_path(path)
            if os.path.exists(full_path):
                raise HTTPBadRequest('"%s" already exists' % path)

            # create our deck handler
            deck_handler = DeckHandler(full_path)
            deck_handler.start()
            deck_handler.create_deck()
            self.decks[path] = deck_handler

        return {'id':path}

    def _open_deck(self, path):
        full_path = self._get_path(path)
        if not os.path.exists(full_path):
            raise HTTPBadRequest('"%s" doesn\'t exist' % path)

        if self.decks.has_key(path):
            deck = self.decks[path]
        else:
            deck = self.decks[path] = DeckHandler(full_path)
            deck.start()

        deck.open_deck()
        return deck

    def open_deck(self, path):
        # open the deck
        self._open_deck(path)

        return {'id':path}

    def _output_fact(self, fact):
        res = dict(zip(fact.keys(), fact.values()))
        res['id'] = str(fact.id)
        return res

    def add_fact(self, deck_id, fields):
        return self._open_deck(deck_id).add_fact(fields)

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

    def find_fact(self, deck_id, bibliobird_id):
        deck = self._open_deck(deck_id)

        try:
            # first we need to find a field model id for a field named "BiblioBird ID"
            fieldModelId = deck.s.scalar("SELECT id FROM fieldModels WHERE name = :name", name=u'BiblioBird ID')
            if not fieldModelId:
                raise HTTPBadRequest("No field model named 'BiblioBird ID'")

            # then we search for a fact with this field set to the given id
            factId = deck.s.scalar("""
                SELECT factId FROM fields WHERE fieldModelId = :fieldModelId AND
                    value = :bibliobirdId""", fieldModelId=fieldModelId, bibliobirdId=bibliobird_id)
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

    def _output_card(self, card):
        return {
            'id': card.id,
            'question': card.question,
            'answer': card.answer,
        }

    def get_card(self, deck_id):
        deck = self._open_deck(deck_id)
        try:
            card = deck.getCard()
            if card:
                # grab the interval strings
                intervals = []
                for i in range(1, 5):
                    intervals.append(deck.nextIntervalStr(card, i))

                card = self._output_card(card)
                card['intervals'] = intervals
        finally:
            deck.close()
        return card

    def answer_card(self, deck_id, card_id, ease):
        ease = int(ease)
        deck = self._open_deck(deck_id)
        try:
            card = deck.cardFromId(card_id)
            if card:
                deck.answerCard(card, ease)
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

