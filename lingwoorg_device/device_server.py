
from SimpleXMLRPCServer import SimpleXMLRPCServer
import random

data_ext = '.json'
try:
    import simplejson as loader
except ImportError:
    try:
        import json as loader
    except ImportError:
        import pickle as loader
        data_ext = '.pickle'

#
# Global Server Data
#

keys = { 'device': '232c11cd4ad53a82dc96c19b825bf704' }
users = {
    'Device': {
        'password': 'iT8_sL@ba',
        'admin': False,
    },
    'user_1': {
        'password': 'abc123',
        'admin': False,
    },
    'user_2': {
        'password': '678def',
        'admin': True,
    },
    'user_3': {
        'password': 'blah7',
        'admin': False,
    }
}
sessions = {}

content = {}
for id in ['211','394']:
    content[id] = loader.load(open(id+data_ext, 'rt'))

devices = {
    'device_1': {
        'content': [content.keys()[1]],
        'users': ['user_1'],
    }
}

software_update = None

#
# Backend functions
#

def session_create(username=None):
    import string
    name = ''.join([random.choice(string.ascii_letters + string.digits) for i in range(20)])
    sessions[name] = username
    return name

server = SimpleXMLRPCServer(('127.0.0.1', 35638))
server.RequestHandlerClass.rpc_paths = ('/services/xmlrpc',)

def register_wrapped_function(func, name):
    def wrapper(hash, domain, timestamp, nonce, sessid, *args):
        import hmac, hashlib
        try:
            test_hash = hmac.new(keys[domain], ';'.join([timestamp, domain, nonce, name]), hashlib.sha256).hexdigest()
        except KeyError:
            test_hash = ''
        if hash != test_hash or not sessions.has_key(sessid):
            raise Exception('1 1 Invalid API key.')

        return func(sessid, *args)
    server.register_function(wrapper, name)

def system_connect():
    return {'sessid': session_create()}

def user_login(sessid, username, password):
    if users.has_key(username) and users[username]['password'] == password:
        del sessions[sessid]
        return {'sessid': session_create(username)}

    raise Exception('Wrong username or password')

def user_logout(sessid):
    del sessions[sessid]
    return True

def lingwoorg_device_get_content_item(sessid, software_version, device_name, id):
    if id == '211' or id == '394':
        return content[id]

    raise Exception('No such content item')

def lingwoorg_device_pull_update(sessid, software_version, device_name):
    dev = devices[device_name]
    data = {
        # id:revid of the content
        'content': [':'.join([str(id), str(content[id]['revid'])]) for id in dev['content']],
        # list of usernames which should be defaults on the device
        'users': dev['users'],
    }

    if software_update is not None:
        # URL to an update of the software
        #
        # See this page for more info:
        #   http://stackoverflow.com/questions/2631255/ideas-for-android-application-update
        # Here is the money bit:
        #   AddType application/vnd.android.package-archive apk
        # A different more invasive approach:
        #   http://www.anddev.org/viewtopic.php?p=23928
        #
        data['software_update'] = software_update

    return data

def lingwoorg_device_push_update(sessid, software_version, device_name, data):
    # check if the device exists
    devices[device_name]
    iam = sessions[sessid]

    # data will be keyed to the user, ie:
    # {'user_1': {'wial_add': ['en:adjective:red', ...]}}
    for username in data.keys():
        for word in data[username]['wial_add']:
            if iam != username and not users[iam]['admin']:
                raise Exception('Current user doesn\'t have permission to wial_add another user')
            print device_name, ': wial_add('+username+') :', word

    # returns True if everything was successful or throws an exception otherwise
    return True
    
# Real XML-RPC functions
server.register_function(system_connect, 'system.connect')
register_wrapped_function(user_login, 'user.login')
register_wrapped_function(user_logout, 'user.logout')
register_wrapped_function(lingwoorg_device_get_content_item, 'lingwoorg_device.get_content_item')
register_wrapped_function(lingwoorg_device_pull_update, 'lingwoorg_device.pull_update')
register_wrapped_function(lingwoorg_device_push_update, 'lingwoorg_device.push_update')

def test(cmd):
    global software_update

    dev = devices['device_1']
    if cmd == 'toggle_content':
        if len(dev['content']) == 1:
            dev['content'] = content.keys()
        elif len(dev['content']) > 1:
            dev['content'] = [content.keys()[1]]
    elif cmd == 'update_content':
        # updates a random 10 revisions in the texts and updates the texts too
        for text in content.values():
            text['revid'] += 1
            for i in range(5):
                entry = random.choice(text['entries'])
                entry['revid'] += 1
    elif cmd == 'software_update':
        software_update = 'http://www.lingwo.org/device/updates/lingwo-0.1.2.apk'
    elif cmd == 'toggle_users':
        if len(dev['users']) == 1:
            dev['users'].append('user_3')
        else:
            dev['users'] = ['user_1']
    else:
        raise Exception('Unknown test command')

    return True

# our special test control function
server.register_function(test)

print "Running server..."
server.serve_forever()

