
import ConfigParser
import unittest
import re

def str2bool(s):
    t = s.lower()
    if t in ['1','yes','true','on']:
        return True
    elif t in ['0','no','false','off']:
        return False
    
    raise ValueError("'{0}' is not a valid boolean string".format(t))

def randstr(l=10):
    import random, string
    return ''.join([random.choice(string.ascii_letters+string.digits) for x in range(l)])

def url2path(url):
    from urlparse import urlparse
    path = urlparse(url).path
    if path[0] == '/':
        path = path[1:]
    return path

class Env(object):
    def __init__(self, config):
        self.config = config
        self.connect_to_selenium()

    def shutdown(self):
        self.browser.close()

    def connect_to_selenium(self):
        from selenium.webdriver import connect

        secname = 'selenium_args'
        if not self.config.has_section(secname):
            self.config.add_section(secname)
        selenium_args = dict(self.config.items(secname))
        if selenium_args.has_key('javascript_enabled'):
            selenium_args['javascript_enabled'] = str2bool(selenium_args['javascript_enabled'])

        self.browser = connect('remote', **selenium_args)

    def load_bbcom(self, path='', lang='en'):
        # TODO: put something in the config for this!
        bburl = 'http://%s.localdomain:35637'
        self.browser.get(bburl % lang + '/'+path)

    @property
    def path(self):
        return url2path(self.browser.current_url)

    def form_submit(self, form_id, submit_value, values,):
        form = self.browser.find_element_by_id(form_id)
        for k, v in values.items():
            elem = form.find_element_by_name(k)
            type = elem.get_attribute('type')
            if type == '' or type is None:
                type = 'text'
            if type in ('text','textarea','password'):
                elem.send_keys(v)
            elif type == 'checkbox':
                if bool(elem.is_selected()) != bool(v):
                    elem.toggle()
            else:
                raise Exception('Unknown form type: '+type)
        form.find_element_by_xpath("//input[@type='submit' and @value='{0}']".format(submit_value)).click()

    def _user_parse_user_path(self, path):
        match = re.match(r'user/(\d+)', path)
        assert match
        return match.group(1)

    def user_discover_uid(self):
        self.load_bbcom('user', lang='en')
        if self.browser.title.startswith('User Login'):
            return

        view_link = self.browser.find_element_by_link_text('View').get_attribute('href')
        return self._user_parse_user_path(url2path(view_link))

    def user_login(self, username, password):
        # logout before logging in!
        self.user_logout()

        self.load_bbcom('user/login', lang='en')
        self.browser.find_element_by_id('edit-name').send_keys(username)
        self.browser.find_element_by_id('edit-pass').send_keys(password)
        self.browser.find_element_by_id('edit-submit').click()
        assert self.path != 'user/login'
        return self._user_parse_user_path(self.path)

    def user_login_type(self, user_type):
        secname = 'user_'+user_type
        username = self.config.get(secname, 'username')
        password = self.config.get(secname, 'password')
        self.user_login(username, password)

    def user_is_logged_in(self):
        return self.user_discover_uid() is not None

    def user_delete(self, uid):
        self.load_bbcom('user/{0}/edit?destination=success'.format(uid), lang='en')
        self.browser.find_element_by_id('edit-delete').click()
        assert self.path == 'user/{0}/delete'.format(uid)
        self.browser.find_element_by_id('edit-submit').click()
        assert self.path == 'success'

    def user_logout(self):
        self.load_bbcom('logout', lang='en')

class NewbieTest(unittest.TestCase):
    def testRegister(self):
        uid = None

        username = randstr()
        password = randstr()
        mail = username+'@example.com'

        try:
            env.load_bbcom('user/register')
            env.form_submit('user-register', 'Create new account', {
                'name': username,
                'mail': mail,
                'conf_mail': mail,
                'pass[pass1]': password,
                'pass[pass2]': password,
                'bbcom_news_optin': False
            })
            uid = env.user_discover_uid()
        finally:
            if uid is not None:
                env.user_login_type('admin')
                env.user_delete(uid)

env = None
def main():
    global env

    config = ConfigParser.ConfigParser()
    config.read('test.ini')

    env = Env(config)
    try:
        unittest.main()
    finally:
        env.shutdown()

if __name__ == '__main__': main()

