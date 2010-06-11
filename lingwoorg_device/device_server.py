
from SimpleXMLRPCServer import SimpleXMLRPCServer

keys = { 'device': '232c11cd4ad53a82dc96c19b825bf704' }
users = {
    'Device': {
        'password': 'iT8_sL@ba',
    },
}
sessions = {}

def session_create(username=None):
    import random, string
    name = ''.join([random.choice(string.ascii_letters + string.digits) for i in range(20)])
    sessions[name] = username
    return name

def session_check(name):
    return sessions.has_key(name)

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

def lingwoorg_device_get_content_item(sessid, id):
    if id == '211' or id == '394':
        import simplejson as json
        return json.load(open(id+'.json', 'rt'))

    raise Exception('No such content item')
    
server.register_function(system_connect, 'system.connect')
register_wrapped_function(user_login, 'user.login')
register_wrapped_function(lingwoorg_device_get_content_item, 'lingwoorg_device.get_content_item')

server.serve_forever()

