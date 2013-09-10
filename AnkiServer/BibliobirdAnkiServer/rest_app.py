
import os

from AnkiServer.apps.rest_app import RestHandlerBase, RestApp

class CollectionInitializer(object):
    def _setup_model(self, col, did):
        """Create the 'External' model used by Bibliobird.com."""
        mm = col.models

        # NOTE: Supposedly, Anki will check the first field on any model
        # for uniqueness. Since it's really important to us that the 'External ID'
        # is unique, we're giving that one first!

        m = mm.new('External')
        m['did'] = did
        for field_name in ['External ID', 'Front', 'Back']:
            fm = mm.newField(field_name)
            mm.addField(m, fm)

        t = mm.newTemplate('Forward')
        t['qfmt'] = '{{Front}}'
        t['afmt'] = '{{FrontSide}}\n\n<hr id=answer>\n\n{{Back}}'
        mm.addTemplate(m, t)

        t = mm.newTemplate('Reverse')
        t['qfmt'] = '{{Back}}'
        t['afmt'] = '{{FrontSide}}\n\n<hr id=answer>\n\n{{Front}}'
        mm.addTemplate(m, t)

        mm.add(m)
    
    def _setup_deck(self, col):
        """Create the 'bibliobird' deck so we can keep our cards seperate."""
        did = col.decks.id('bibliobird', create=True)
        col.decks.flush()
        return did
    
    def __call__(self, col):
        did = self._setup_deck(col)
        self._setup_model(col, did)

class CollectionHandler(RestHandlerBase):
    """Custom collection funtions for Bibliobird."""

    def resync_notes(self, col, req):
        # build a fast lookup table
        lookup = dict([(id, True) for id in req.data.get('external_ids', [])])

        remove = []

        # Loop through all the notes on the 'bibliobird' deck - adding any to the
        # remove list which aren't on the lookup table, and pruning all the ids
        # that we've found from the lookup table (leaving only the missing ids)
        for nid in col.findNotes('deck:"bibliobird"'):
            note = col.getNote(nid)
            try:
                id = note['External ID']
            except:
                id = False

            # Remove any notes with no 'External ID'
            if not id:
                remove.append(nid)
                continue

            # Remove any notes which can't be found in the lookup list
            if not lookup.has_key(id):
                remove.append(nid)
            else:
                del lookup[id]

        # Remove all the notes on the remove list
        col.remNotes(remove)

        # Return the missing list
        return {'missing': lookup.keys()}

# Our entry point
def make_app(global_conf, **local_conf):
    # setup the logger (copied from AnkiServer.apps.rest_app.make_app())
    from AnkiServer.utils import setup_logging
    setup_logging(local_conf.get('logging.config_file'))

    app = RestApp(
        data_root=local_conf['data_root'],
        allowed_hosts=local_conf.get('allowed_hosts', '127.0.0.1'),
        setup_new_collection=CollectionInitializer()
    )

    app.add_handler_group('collection', CollectionHandler())

    return app

