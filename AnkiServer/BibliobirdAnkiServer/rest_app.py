
import os

from AnkiServer.apps.rest_app import RestHandlerBase, RestApp

def setup_new_collection(col):
    """Create the 'External' model used by Bibliobird.com."""
    mm = col.models

    m = mm.new('External')
    for field_name in ['Front', 'Back', 'External ID']:
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

# Our entry point
def make_app(global_conf, **local_conf):
    # setup the logger (copied from AnkiServer.apps.rest_app.make_app())
    from AnkiServer.utils import setup_logging
    setup_logging(local_conf.get('logging.config_file'))

    app = RestApp(
        data_root=local_conf['data_root'],
        allowed_hosts=local_conf.get('allowed_hosts', '127.0.0.1'),
        setup_new_collection=setup_new_collection
    )

    return app

