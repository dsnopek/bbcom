
import anki
from anki.facts import Fact
from anki.models import Model, CardModel, FieldModel

from threading import Thread
from Queue import Queue, PriorityQueue

import os

def ExternalModel():
    m = Model(u'External')
    # we can only guarantee that the Front will be unique because it will
    # be based on the headword, language, pos.  The Back could be anything!
    m.addFieldModel(FieldModel(u'Front', True, True))
    # while I think that Back should be required, I don't really want this to
    # fail just because of that!!
    m.addFieldModel(FieldModel(u'Back', False, False))
    m.addFieldModel(FieldModel(u'External ID', True, True))
    m.addCardModel(CardModel(u'Forward', u'%(Front)s', u'%(Back)s'))
    m.addCardModel(CardModel(u'Reverse', u'%(Back)s', u'%(Front)s'))

    m.tags = u"External"
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
    newFunc.func_name = func.func_name
    return newFunc

def defer_none(func, priority=0):
    func = log_exception(func)
    def newFunc(*args, **kw):
        self = args[0]
        self._queue.put((priority, func, args, kw, None))
    newFunc.func_name = func.func_name
    return newFunc

def defer(func, priority=0):
    func = log_exception(func)
    def newFunc(*args, **kw):
        return_queue = Queue()
        self = args[0]
        self._queue.put((priority, func, args, kw, return_queue))
        ret = return_queue.get(True)
        if isinstance(ret, Exception):
            raise e
        return ret
    newFunc.func_name = func.func_name
    return newFunc

def check_deck(func):
    def newFunc(self, *args, **kw):
        if self.deck is None:
            raise HTTPBadRequest('Cannot do %s without first doing open_deck or create_deck' % func.func_name)
        return func(self, *args, **kw)
    newFunc.func_name = func.func_name
    return newFunc

def external(func):
    func.external = True
    return func

class DeckHandler(object):
    def __init__(self, path):
        self.path = path
        self.deck = None

        self._thread = Thread(target=self._run)
        self._queue = PriorityQueue()
        self._running = False

        # creates or opens the deck (will be first thing executed
        # on the thread once it starts)
        if os.path.exists(self.path):
            self.open_deck()
        else:
            self.create_deck()

    def _run(self):
        while self._running:
            priority, func, args, kw, return_queue = self._queue.get(True)
            ret = func(*args, **kw)
            if return_queue is not None:
                return_queue.put(ret)

    # TODO: move to module level scope?
    def _output_fact(self, fact):
        res = dict(zip(fact.keys(), fact.values()))
        res['id'] = str(fact.id)
        return res

    # TODO: move to module level scope?
    def _output_card(self, card):
        return {
            'id': card.id,
            'question': card.question,
            'answer': card.answer,
        }

    def start(self):
        self._running = True
        self._thread.start()

    @classmethod
    def external_allowed(cls, name):
        if hasattr(cls, name):
            func = getattr(cls, name)
            return callable(func) and hasattr(func, 'external') and func.external
        return False

    @defer_none
    def stop(self):
        if self.deck is not None:
            self.deck.save()
            self.deck.close()
        self._running = False

    @defer_none
    def create_deck(self):
        if self.deck is not None:
            return

        import errno

        # mkdir -p the path, because it might not exist
        dir = os.path.dirname(self.path)
        try:
            os.makedirs(dir)
        except OSError, exc:
            if exc.errno == errno.EEXIST:
                pass
            else:
                raise

        deck = anki.DeckStorage.Deck(self.path)
        try:
            deck.initUndo()
            deck.addModel(ExternalModel())
            deck.save()
        except Exception, e:
            deck.close()
            deck = None
            raise e

        self.deck = deck

    @defer_none
    def open_deck(self):
        if self.deck is not None:
            return

        self.deck = anki.DeckStorage.Deck(self.path)

    @external
    @defer_none
    @check_deck
    def add_fact(self, fields):
        fact = self.deck.newFact()
        for key in fact.keys():
            fact[key] = unicode(fields[key])

        self.deck.addFact(fact)
        self.deck.save()

    @external
    @defer_none
    @check_deck
    def save_fact(self, fact):
        newFact = self.deck.s.query(Fact).get(int(fact['id']))
        for key in newFact.keys():
            newFact[key] = fact[key]

        newFact.setModified(textChanged=True)
        self.deck.setModified()
        self.deck.save()

    @external
    @defer
    @check_deck
    def find_fact(self, external_id):
        # first we need to find a field model id for a field named "External ID"
        fieldModelId = self.deck.s.scalar("SELECT id FROM fieldModels WHERE name = :name", name=u'External ID')
        if not fieldModelId:
            raise HTTPBadRequest("No field model named 'External ID'")

        # then we search for a fact with this field set to the given id
        factId = self.deck.s.scalar("""
            SELECT factId FROM fields WHERE fieldModelId = :fieldModelId AND
                value = :externalId""", fieldModelId=fieldModelId, externalId=external_id)
        if not factId:
            # we need to signal somehow to the calling application that no such
            # deck exists, but without it being considered a "bad error".  404 is 
            # inappropriate that refers to the resource (ie. /find_fact) which is
            # here obviously.
            return None

        fact = self.deck.s.query(Fact).get(factId)
        return self._output_fact(fact)

    @external
    @defer_none
    @check_deck
    def delete_fact(self, fact_id):
        self.deck.deleteFact(int(fact_id))
        self.deck.save()

    @external
    @defer
    @check_deck
    def get_card(self):
        card = self.deck.getCard()
        if card:
            # grab the interval strings
            intervals = []
            for i in range(1, 5):
                intervals.append(self.deck.nextIntervalStr(card, i))

            card = self._output_card(card)
            card['intervals'] = intervals
        return card

    @external
    @defer_none
    @check_deck
    def answer_card(self, card_id, ease):
        ease = int(ease)
        card = self.deck.cardFromId(card_id)
        if card:
            self.deck.answerCard(card, ease)
            self.deck.save()

