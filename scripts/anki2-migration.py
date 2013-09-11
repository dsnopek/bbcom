
import sys
sys.path.insert(0, "/usr/share/anki")

import anki
from anki import Collection
from anki.importing.anki1 import Anki1Importer

import os, glob

def usage(msg = None):
    if msg is not None:
       print >> sys.stderr, msg
       print >> sys.stderr, ''

    print >> sys.stderr, "Usage: anki2-migration.py <anki1_root> <anki2_root>"
    sys.exit(1)

def main():
    try:
        anki1_root, anki2_root = sys.argv[1:]
    except:
        usage()

    if not os.path.exists(anki1_root):
        usage('<anki1_root> must exist!')
    if os.path.exists(anki2_root):
        usage('<anki2_root> must NOT exist!')

    os.mkdir(anki2_root)

    # Get the directory list, filter to just the integers and sort 
    # numerically.
    dir_list = []
    for name in os.listdir(anki1_root):
        if name in ['.', '..']:
            continue

        try:
            dir_list.append(int(name))
        except ValueError:
            continue
    dir_list.sort()

    for uid in dir_list:
        print "Migrating %s ..." % (uid)
        migrate_users_decks(os.path.join(anki1_root, str(uid)), os.path.join(anki2_root, str(uid)))

def migrate_users_decks(src, dst):
    # Create the new Anki2 collection
    os.mkdir(dst)
    col = Collection(os.path.join(dst, 'collection.anki2'))

    # loop over all the Anki1 decks, importing them
    for deck_path in glob.glob(os.path.join(src, '*.anki')):
        importer = Anki1Importer(col, deck_path.decode('utf-8'))
        importer.run()

    col.close()

if __name__ == '__main__': main()

