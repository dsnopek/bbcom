#!/usr/bin/env python

from xmlrpclib import ServerProxy, Error
try:
    import json
except ImportError:
    import simplejson as json

DOMAIN = 'skystudio'
KEY = 'c19d6a8b2a58e8c74e6cfa4fa312d918'
USERNAME = 'Importer'
PASSWORD = 'IamTHEiMP0rt3r'

# connect
server = ServerProxy('http://localhost:35637/services/xmlrpc')
res = server.system.connect()
sessid = res['sessid']

def callMethod(method, *user_args):
    import time, random, string, hmac, hashlib

    cmd = method._Method__name
    #timestamp = str(int(time.mktime(time.gmtime())));
    # NOTE: Heh, so this takes Warszawa time!  Really should be UTC...
    timestamp = str(int(time.mktime(time.localtime())));
    nonce = ''.join([random.choice(string.ascii_letters) for x in range(10)])
    hash = hmac.new(KEY, ';'.join([timestamp, DOMAIN, nonce, cmd]), hashlib.sha256).hexdigest()

    args = [hash, DOMAIN, timestamp, nonce, sessid] + list(user_args)

    return method(*args)

# login
res = callMethod(server.user.login, USERNAME, PASSWORD)
sessid = res['sessid']

# lingwoorg_content.get_content_item
res = callMethod(server.lingwoorg_content.get_content_item, '211')
print json.dumps(res)

