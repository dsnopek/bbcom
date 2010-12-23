
from webob.dec import wsgify
from webob.exc import *
from webob import Response

import anki
from anki.facts import Fact
from anki.models import Model, CardModel, FieldModel

from threading import Thread, Lock, current_thread
from Queue import Queue, PriorityQueue

try:
    import simplejson as json
except ImportError:
    import json

import os, errno, time, logging

__all__ = ['DeckThread']

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

def _log_exception(func):
    def newFunc(self, *args, **kw):
        try:
            return func(self, *args, **kw)
        except Exception, e:
            logging.error('DeckThread[%s] Unable to %s(*%s, **%s): %s'
                % (self.path, func.func_name, repr(args), repr(kw), e))
            return e
    newFunc.func_name = func.func_name
    return newFunc

def _defer_none(func, priority=0):
    func = _log_exception(func)
    def newFunc(*args, **kw):
        self = args[0]
        if current_thread() is self._thread:
            func(*args, **kw)
        else:
            self._queue.put((priority, func, args, kw, None))
    newFunc.func_name = func.func_name
    return newFunc

def _defer(func, priority=0):
    func = _log_exception(func)
    def newFunc(*args, **kw):
        self = args[0]
        if current_thread() == self._thread:
            return func(*args, **kw)
        return_queue = Queue()
        self._queue.put((priority, func, args, kw, return_queue))
        ret = return_queue.get(True)
        if isinstance(ret, Exception):
            raise ret
        return ret
    newFunc.func_name = func.func_name
    return newFunc

def _check_deck(func):
    def newFunc(self, *args, **kw):
        if self.deck is None:
            raise HTTPBadRequest('Cannot do %s without first doing open_deck or create_deck' % func.func_name)
        return func(self, *args, **kw)
    newFunc.func_name = func.func_name
    return newFunc

def _external(func):
    func.external = True
    return func

class DeckThread(object):
    def __init__(self, path):
        self.path = os.path.abspath(path)
        self.deck = None

        self._queue = PriorityQueue()
        self._thread = None
        self._running = False
        self.last_timestamp = time.time()

    @property
    def running(self):
        return self._running

    def _run(self):
        logging.info('DeckThread[%s]: Starting...' % self.path)

        # creates or opens the deck
        if os.path.exists(self.path):
            self.open_deck()
        else:
            self.create_deck()

        try:
            while self._running:
                priority, func, args, kw, return_queue = self._queue.get(True)
                self.last_timestamp = time.time()
                ret = func(*args, **kw)
                if return_queue is not None:
                    return_queue.put(ret)
        finally:
            self.close_deck()
            # clean out old thread object
            self._thread = None
            # in case we got here via an exception
            self._running = False

            logging.info('DeckThread[%s]: Stopped!' % self.path)

    def start(self):
        if not self._running:
            self._running = True
            assert self._thread is None
            self._thread = Thread(target=self._run)
            self._thread.start()

    @_defer_none
    def stop(self):
        self._running = False

    def stop_and_wait(self):
        """ Tell the thread to stop and wait for it to happen. """
        self.stop()
        if self._thread is not None:
            self._thread.join()

    @classmethod
    def external_allowed(cls, name):
        if hasattr(cls, name):
            func = getattr(cls, name)
            return callable(func) and hasattr(func, 'external') and func.external
        return False

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

    @_defer_none
    def create_deck(self):
        global thread_pool

        if self.deck is not None:
            return

        # acquire the lock to this deck
        thread_pool.lock(self.path)

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

    @_defer_none
    def open_deck(self):
        global thread_pool

        if self.deck is not None:
            return

        # acquire the lock to this deck
        thread_pool.lock(self.path)

        self.deck = anki.DeckStorage.Deck(self.path)

    @_defer_none
    def close_deck(self):
        global thread_pool

        if self.deck is None:
            return

        self.deck.close()
        self.deck = None

        # delete the cache for 'External ID' on this deck
        if hasattr(self, '_external_field_id'):
            delattr(self, '_external_field_id')

        # release our lock to the deck
        thread_pool.unlock(self.path)

    @_external
    @_defer_none
    @_check_deck
    def add_fact(self, fields):
        fact_id = self._find_fact(fields['External ID'])
        if fact_id is not None:
            fields['id'] = fact_id
            self.save_fact(fields)
        else:
            fact = self.deck.newFact()
            for key in fact.keys():
                fact[key] = unicode(fields[key])

            self.deck.addFact(fact)
            self.deck.save()

    @_external
    @_defer_none
    @_check_deck
    def save_fact(self, fact):
        newFact = self.deck.s.query(Fact).get(int(fact['id']))
        for key in newFact.keys():
            newFact[key] = fact[key]

        newFact.setModified(textChanged=True)
        self.deck.setModified()
        self.deck.save()

    @property
    def external_field_id(self):
        if not hasattr(self, '_external_field_id'):
            # find a field model id for a field named "External ID"
            self._external_field_id = self.deck.s.scalar("SELECT id FROM fieldModels WHERE name = :name", name=u'External ID')
        if self._external_field_id is None:
            raise HTTPBadRequest("No field model named 'External ID'")
        return self._external_field_id

    def _find_fact(self, external_id):
        return self.deck.s.scalar("""
            SELECT factId FROM fields WHERE fieldModelId = :fieldModelId AND
                value = :externalId""", fieldModelId=self._external_field_id, externalId=external_id)

    @_external
    @_defer
    @_check_deck
    def find_fact(self, external_id):
        factId = self._find_fact(external_id)
        if not factId:
            # we need to signal somehow to the calling application that no such
            # deck exists, but without it being considered a "bad error".  404 is 
            # inappropriate that refers to the resource (ie. /find_fact) which is
            # here obviously.
            return None

        fact = self.deck.s.query(Fact).get(factId)
        return self._output_fact(fact)

    @_external
    @_defer_none
    @_check_deck
    def delete_fact(self, fact_id=None, external_id=None):
        if fact_id is None and external_id is not None:
            fact_id = self._find_fact(external_id)
        if fact_id is not None:
            self.deck.deleteFact(int(fact_id))
            self.deck.save()

    @_external
    @_defer
    @_check_deck
    def resync_facts(self, external_ids):
        from anki.facts import fieldsTable
        from sqlalchemy.sql import select, and_, not_

        # remove extra cards
        selectExtra = select([fieldsTable.c.factId],
            and_(
                fieldsTable.c.fieldModelId == self.external_field_id,
                not_(fieldsTable.c.value.in_(external_ids))
            )
        )
        for factId, in self.deck.s.execute(selectExtra).fetchall():
            self.deck.deleteFact(factId)
        self.deck.save()

        # find ids that should be on this deck but which aren't
        missing_ids = []
        for external_id in external_ids:
            if self._find_fact(external_id) is None:
                missing_ids.append(external_id)

        return {'missing':missing_ids}

    @_external
    @_defer
    @_check_deck
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

    @_external
    @_defer_none
    @_check_deck
    def answer_card(self, card_id, ease):
        ease = int(ease)
        card = self.deck.cardFromId(card_id)
        if card:
            self.deck.answerCard(card, ease)
            self.deck.save()

class DeckThreadPool(object):
    def __init__(self):
        self.decks = {}
        self.locks = {}

        self.monitor_frequency = 5
        self.monitor_inactivity = 30

        monitor = Thread(target=self._monitor_run)
        monitor.daemon = True
        monitor.start()

        self._monitor_thread = monitor

    def _monitor_run(self):
        """ Monitors threads for inactivity and shuts them down. """
        while True:
            cur = time.time()
            for path, deck in self.decks.items():
                if deck.running and cur - deck.last_timestamp >= self.monitor_inactivity:
                    deck.stop()
            time.sleep(self.monitor_frequency)

    def open_deck(self, path):
        path = os.path.abspath(path)

        try:
            deck = self.decks[path]
        except KeyError:
            deck = self.decks[path] = DeckThread(path)

        deck.start()

        return deck

    def shutdown(self):
        for deck in self.decks.values():
            deck.stop()
        self.decks = {}

    def lock(self, path):
        try:
            lock = self.locks[path]
        except KeyError:
            lock = self.locks[path] = Lock()
        lock.acquire()

    def unlock(self, path):
        self.locks[path].release()

thread_pool = DeckThreadPool()

class DeckApp(object):
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

    @wsgify
    def __call__(self, req):
        global thread_pool

        if self.allowed_hosts != '*':
            try:
                remote_addr = req.headers['X-Forwarded-For']
            except KeyError:
                remote_addr = req.remote_addr
            if remote_addr != self.allowed_hosts:
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
        if not DeckThread.external_allowed(func):
            raise HTTPNotFound()
        deck = thread_pool.open_deck(self._get_path(path))
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
        try:
            output = func(**input)
        except Exception, e:
            logging.error(e)
            return HTTPInternalServerError()

        if output is None:
            return Response('', content_type='text/plain')
        else:
            return Response(json.dumps(output), content_type='application/json')

# Our entry point
def make_app(global_conf, **local_conf):
    # setup the logger
    logging_config_file = local_conf.get('logging.config_file')
    if logging_config_file:
        import logging.config
        logging.config.fileConfig(logging_config_file)

    return DeckApp(
        data_root=local_conf.get('data_root', '.'),
        allowed_hosts=local_conf.get('allowed_hosts', '*')
    )

