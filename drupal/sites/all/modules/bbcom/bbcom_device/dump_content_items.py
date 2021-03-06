#!/usr/bin/env python

from xmlrpclib import ServerProxy, Error
try:
    import json
except ImportError:
    import simplejson as json
import pickle

DOMAIN = 'device'
KEY = '232c11cd4ad53a82dc96c19b825bf704'
USERNAME = 'Device'
PASSWORD = 'iT8_sL@ba'

# connect
# PHP server
server = ServerProxy('http://localhost:35637/services/xmlrpc')
# Python server
#server = ServerProxy('http://localhost:35638/services/xmlrpc')
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

for id in ['211', '394']:
    # bbcom_device.get_content_item
    print "Dumping "+id+"..."
    res = callMethod(server.bbcom_device.get_content_item, '0.0.01', 'device_1', id)
    res['audio'] = res['audio'].replace('localhost:35637', 'www.lingwo.org')
    json.dump(res, open(id+'.json', 'wt'))
    pickle.dump(res, open(id+'.pickle', 'wt'))

