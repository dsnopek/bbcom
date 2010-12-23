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
import anki.lang

# production
anki.sync.SYNC_URL = 'http://anki.bibliobird.com/sync/'
anki.sync.SYNC_HOST = 'anki.bibliobird.com'
anki.sync.SYNC_PORT = 80

# testing
#anki.sync.SYNC_URL = 'http://localhost:27701/sync/'
#anki.sync.SYNC_HOST = 'localhost'
#anki.sync.SYNC_PORT = 27701

# Monkey patch the "_" function to replace the "Create free account" links.
# TODO: There are probably better ways to do this but this was the easiest!
create_free_account_link = '<a href="http://www.bibliobird.com/user/register">'
def t(s):
    global create_free_account_link
    return anki.lang._(s).replace('<a href="http://anki.ichi2.net/">', create_free_account_link)
import ankiqt.forms.preferences
import ankiqt.ui.main
ankiqt.forms.preferences._ = t
ankiqt.ui.main._ = t

