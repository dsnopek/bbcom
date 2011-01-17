
import ConfigParser
import unittest

class Env(object):
    def __init__(self, selenium_args=None):
        # create our connection to the selenium browser
        from selenium.webdriver import connect
        if selenium_args is None:
            selenium_args = {}
        self.browser = connect('remote', **selenium_args)

    def open_bibliobird(self, path='', lang='en'):
        # TODO: put something in the config for this!
        bburl = 'http://%s.localdomain:35637'
        self.browser.get(bburl % lang + '/' +path)

    def user_login(self, username, password):
        pass

    def user_login_type(self, user_type):
        pass

    def user_logout(self):
        pass

class NewbieTest(unittest.TestCase):
    def setUp(self):
        env.open_bibliobird()

    def tearDown(self):
        pass

    def testRegister(self):
        # TODO: here we want to register a new user and check that all the validation stuff
        # ie: messages, links, etc -- is in place.
        pass

env = None
def main():
    global env

    SELENIUM_ARGS = 'selenium_args'

    # setup config file
    config = ConfigParser.ConfigParser()
    config.read('test.ini')
    for s in [SELENIUM_ARGS]:
        if not config.has_section(s):
            config.add_section(s)

    # read in the selenium_args
    selenium_args = dict(config.items(SELENIUM_ARGS))
    try:
        selenium_args['javascript_enabled'] = config.getboolean(SELENIUM_ARGS, 'javascript_enabled')
    except ConfigParser.NoOptionError:
        pass

    env = Env(selenium_args=selenium_args)

    unittest.main()

if __name__ == '__main__': main()

