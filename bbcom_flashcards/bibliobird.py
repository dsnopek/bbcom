# -*- coding: utf-8 -*-
# Copyright: David Snopek <dsnopek@gmail.com>
# License: GNU GPL, version 3 or later; http://www.gnu.org/copyleft/gpl.html
#
# Sync decks to BiblioBird.com instead of AnkiWeb.
#
# This allows BiblioBird users to access a special "bibliobird" deck, which
# contains flashcards for all the words in their "Words I Am Learning" list
# on BiblioBird.com.  Changes on BiblioBird will automatically be sync'ed to
# this deck and vice-a-versa.
#
# You can also sync any other decks you like to BiblioBird -- for the time 
# being, there are no restrictions what-so-ever.
#

import anki.sync

# production
anki.sync.SYNC_URL = 'http://anki.bibliobird.com/sync/'
anki.sync.SYNC_HOST = 'anki.bibliobird.com'
anki.sync.SYNC_PORT = 80

# testing
#anki.sync.SYNC_URL = 'http://localhost:27701/sync/'
#anki.sync.SYNC_HOST = 'localhost'
#anki.sync.SYNC_PORT = 27701

