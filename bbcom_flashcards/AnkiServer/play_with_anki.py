
import anki, os
from anki.facts import Fact
from anki.stdmodels import BasicModel
from anki.models import Model, CardModel, FieldModel

def LingwoModel():
    m = Model(u'Lingwo')
    m.addFieldModel(FieldModel(u'Front', True, True))
    m.addFieldModel(FieldModel(u'Back', True, True))
    m.addFieldModel(FieldModel(u'Lingwo ID', True, True))
    m.addCardModel(CardModel(u'Forward', u'%(Front)s', u'%(Back)s'))
    m.addCardModel(CardModel(u'Reverse', u'%(Back)s', u'%(Front)s'))

    m.tags = u"Lingwo"
    return m

# create a deck
def create_deck(deck_path):
    if os.path.exists(deck_path):
        print "Removing old deck..."
        os.unlink(deck_path)
    deck = anki.DeckStorage.Deck(deck_path)
    deck.initUndo()
    deck.addModel(LingwoModel())
    #deck.addModel(BasicModel())
    deck.save()
    return deck
deck = create_deck('test.anki')

# add a fact
def add_fact():
    fact = deck.newFact()
    fact['Front'] = u'red'
    fact['Back'] = u'czerwony'
    fact['Lingwo ID'] = u'en-pl/red(adjective)'
    cards = deck.addFact(fact)
    #print len(cards)
    deck.save()
add_fact()

# get a card
def get_fact(id):
    # first we need to find a field model id for a field named "Lingwo ID"
    fieldModelId = deck.s.scalar("SELECT id FROM fieldModels WHERE name = :name", name=u'Lingwo ID')
    if not fieldModelId:
        raise "No field model named 'Lingwo ID'"
    print fieldModelId

    # then we search for a fact with this field set to the given id
    factId = deck.s.scalar("""
        SELECT factId FROM fields WHERE fieldModelId = :fieldModelId AND
            value = :lingwoId""", fieldModelId=fieldModelId, lingwoId=id)
    print factId

    fact = deck.s.query(Fact).get(factId)
    return fact
fact = get_fact('en-pl/red(adjective)')

def edit_fact(fact):
    fact['Front'] = u'new front'
    fact['Back'] = u'new back'
    fact.setModified(textChanged=True)
    deck.setModified()
    deck.save()
edit_fact(fact)

def delete_fact(id):
    deck.deleteFact(id)
    deck.save()
delete_fact(fact.id)

