#!/usr/bin/env python

from xmlrpclib import ServerProxy, Error
try:
    import json
except ImportError:
    import simplejson as json
from Tkinter import *

users = [
    ('Device', 'iT8_sL@ba', 'Device user. For when we have no user yet, but need to sync.'),
    ('user_1', 'abc123', 'Generic test user.'),
    ('user_2', '678def', 'Generic test user, with admin priviledges.'),
    ('user_3', 'blah7',  'Generic test user.'),
]

DOMAIN = 'device'
KEY = '232c11cd4ad53a82dc96c19b825bf704'
USERNAME = users[0][0]
PASSWORD = users[0][1]

# PHP server
#URL = 'http://localhost:35637/services/xmlrpc'
# Python server
URL = 'http://localhost:35638/services/xmlrpc'

class ServerInterface(object):
    def __init__(self, url, log=None):
        self.server = ServerProxy(url, allow_none=True)
        self.sessid = None
        self._in_connect = False

        if log is None:
            def log(data):
                print data
        self.log = log

    def connect(self):
        try:
            res = self.server.system.connect()
        except Exception, e:
            self.log('ERROR: '+str(e))
            raise e

        self.sessid = res['sessid']
        self._in_connect = True

    def _call_method(self, method, *user_args):
        import time, random, string, hmac, hashlib

        cmd = method._Method__name
        #timestamp = str(int(time.mktime(time.gmtime())));
        # NOTE: Heh, so this takes Warszawa time!  Really should be UTC...
        timestamp = str(int(time.mktime(time.localtime())));
        nonce = ''.join([random.choice(string.ascii_letters) for x in range(10)])
        hash = hmac.new(KEY, ';'.join([timestamp, DOMAIN, nonce, cmd]), hashlib.sha256).hexdigest()

        args = [hash, DOMAIN, timestamp, nonce, self.sessid] + list(user_args)

        try:
            res = method(*args)
        except Exception, e:
            self.log('ERROR: '+str(e))
            raise e

        return res

    def login(self, username=USERNAME, password=PASSWORD):
        if self.sessid is not None:
            if self._in_connect:
                # Everything is good, we have passed through the connect stage
                pass
            else:
                # We are still logged in as another user.  This will clear the
                # self.sessid and the next 'if' will connect us.
                self.logout()

        if self.sessid is None:
            # We need to connect before we can login
            self.connect()
            
        res = self._call_method(self.server.user.login, username, password)
        self.sessid = res['sessid']
        self._in_connect = False

    def logout(self):
        res = self._call_method(self.server.user.logout)
        self.sessid = None
        self._in_connect = False
        return res

    def get_content_item(self, id):
        return self._call_method(self.server.lingwoorg_device.get_content_item, id)

    def pull_update(self, software_version, device_name):
        return self._call_method(self.server.lingwoorg_device.pull_update, software_version, device_name)

    def push_update(self, software_version, device_name, data):
        return self._call_method(self.server.lingwoorg_device.push_update, software_version, device_name, data)

    def server_ctrl(self, cmd):
        try:
            self.server.test(cmd)
        except Exception, e:
            self.log('ERROR: '+str(e))
            raise e

class App(object):
    def __init__(self, master, url):
        # connection to server
        self.server = ServerInterface(url, self.log)

        def server_ctrl_factory(cmd):
            def server_ctrl():
                self.log('Sending server command: '+cmd)
                self.server.server_ctrl(cmd)
            return server_ctrl

        server_buttons = [
            ('Toggle Content', server_ctrl_factory('toggle_content')),
            ('Update Content', server_ctrl_factory('update_content')),
            ('Software Update', server_ctrl_factory('software_update')),
            ('Toggle Users', server_ctrl_factory('toggle_users')),
        ]

        def login_factory(username, password):
            def login():
                self.log('Login as \''+username+'\' ...')
                self.server.login(username, password)
            return login

        client_buttons = []
        for username, password, hint in users:
            client_buttons.append(('Login \''+username+'\'', login_factory(username, password)))
        client_buttons.extend([
            ('Logout', self.client_btn_logout),
            ('Pull Update', self.client_btn_pull_update),
            ('Push Update (user_1)', self.client_btn_push_update),
        ])

        def create_frame(parent, text, btns):
            frame = Frame(parent)
            label = Label(frame, text=text)
            label.pack(side=TOP)
            for text, command in btns:
                btn = Button(frame, text=text, command=command)
                btn.pack(side=TOP)
            frame.pack(side=LEFT)
            return frame

        # arrange the buttons
        frame = Frame(master)
        create_frame(frame, 'Server Control', server_buttons)
        create_frame(frame, 'Client Control', client_buttons)
        frame.pack(side=TOP)

        # add the log view
        frame = Frame(master)
        frame.pack(fill=BOTH)
        scrollbar = Scrollbar(frame)
        scrollbar.pack(side=RIGHT, fill=Y)
        self.text = Text(frame, bg='white', yscrollcommand=scrollbar.set)
        self.text.pack(fill=BOTH)
        scrollbar.config(command=self.text.yview)

    def log(self, data):
        if hasattr(self, 'text'):
            self.text.insert(END, data+"\n")
            self.text.see(END)
        else:
            print data

    def client_btn_logout(self):
        self.log('Logging out')
        self.server.logout()

    def client_btn_pull_update(self):
        self.log('Pulling update for this device from the server')
        res = self.server.pull_update('1.0', 'device_1')
        self.log('RESULT: '+repr(res))

    def client_btn_push_update(self):
        items = ['en:adjective:red', 'en:adjective:green', 'en:adjective:blue']
        self.log('push_update: '+repr(items))
        res = self.server.push_update('1.0', 'device_1', {'user_1': {'wial_add': items}})
        self.log('RESULT: '+repr(res))

def main():
    root = Tk()
    app = App(root, URL)
    root.mainloop()

if __name__ == '__main__': main()

