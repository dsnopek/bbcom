#!/usr/bin/env python

from xmlrpclib import ServerProxy, Error
try:
    import json
except ImportError:
    import simplejson as json

DOMAIN = 'device'
KEY = '232c11cd4ad53a82dc96c19b825bf704'
USERNAME = 'Device'
PASSWORD = 'iT8_sL@ba'

class Interface(object):
    def __init__(self, url):
        self.server = ServerProxy(url)
        self.sessid = None

    def connect(self):
        res = self.server.system.connect()
        self.sessid = res['sessid']

    def _call_method(self, method, *user_args):
        import time, random, string, hmac, hashlib

        cmd = method._Method__name
        #timestamp = str(int(time.mktime(time.gmtime())));
        # NOTE: Heh, so this takes Warszawa time!  Really should be UTC...
        timestamp = str(int(time.mktime(time.localtime())));
        nonce = ''.join([random.choice(string.ascii_letters) for x in range(10)])
        hash = hmac.new(KEY, ';'.join([timestamp, DOMAIN, nonce, cmd]), hashlib.sha256).hexdigest()

        args = [hash, DOMAIN, timestamp, nonce, self.sessid] + list(user_args)

        return method(*args)

    def login(self, username=USERNAME, password=PASSWORD):
        res = self._call_method(self.server.user.login, username, password)
        self.sessid = res['sessid']

    def get_content_item(self, id):
        return self._call_method(self.server.lingwoorg_device.get_content_item, id)

    def pull_update(self, software_version, device_name):
        return self._call_method(self.server.lingwoorg_device.pull_update, software_version, device_name)

    def push_update(self, software_version, device_name, data):
        return self._call_method(self.server.lingwoorg_device.push_update, software_version, device_name, data)

# connect
# PHP server
#server = Interface('http://localhost:35637/services/xmlrpc')
# Python server
server = Interface('http://localhost:35638/services/xmlrpc')

server.connect()
server.login()
item = server.get_content_item('211')
#item = server.pull_update('1.0', 'device_1')
#server.login('user_1', 'abc123')
#server.login('user_2', '678def')
#item = server.push_update('1.0', 'device_1', {'user_1': {'wial_add': ['en:adjective:red']}})
print item


