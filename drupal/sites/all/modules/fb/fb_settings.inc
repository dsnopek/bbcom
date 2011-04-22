<?php
// $Id: fb_settings.inc,v 1.29 2010/11/02 18:49:00 yogadex Exp $

/**
 * @file
 * This file is to be included from your sites/.../settings.php file.
 *
 * In this code we set up special session management and url
 * rewriting.  These things must be done before modules are loaded, so
 * the code is here instead of fb.module. And that is why this
 * must be include from settings.php.
 *
 */


// Each of these are things we can learn and store in fb_settings().
// The CB (callback) values are learned via URL rewriting.
// Include fb_url_rewrite.inc in your settings.php to enable this.
define('FB_SETTINGS_CB', 'fb_cb');
define('FB_SETTINGS_CB_TYPE', 'fb_cb_type');
define('FB_SETTINGS_CB_SESSION', 'fb_sess');

// Things we can learn from the facebook session.
define('FB_SETTINGS_APIKEY', 'apikey');
define('FB_SETTINGS_ID', 'app_id');
define('FB_SETTINGS_FBU', 'fbu');
define('FB_SETTINGS_TOKEN', 'token');
define('FB_SETTINGS_PROFILE_ID', 'profile_id');
define('FB_SETTINGS_TYPE', 'type'); // page type not same as cb type
define('FB_SETTINGS_COOKIE_DOMAIN', 'cookie_domain');

// Possible values for settings.
define('FB_SETTINGS_TYPE_CANVAS', 'canvas');
define('FB_SETTINGS_TYPE_CONNECT', 'connect');
define('FB_SETTINGS_TYPE_PROFILE', 'profile');


/**
 * Helper function to remember values as we learn them.
 */
function fb_settings($key = NULL, $value = NULL) {
  static $cache = array();
  if (isset($value)) {
    $cache[$key] = $value;
  }
  if (isset($key)) {
    return isset($cache[$key]) ? $cache[$key] : NULL;
  }
  else {
    return $cache;
  }
}


/**
 * Helpers to parse signed_session.  Copied from facebook.php.
 */
/**
 * Base64 encoding that doesn't need to be urlencode()ed.
 * Exactly the same as base64_encode except it uses
 *   - instead of +
 *   _ instead of /
 *
 * @param String base64UrlEncodeded string
 */
function _fb_settings_base64_url_decode($input) {
  return base64_decode(strtr($input, '-_', '+/'));
}

/**
 * See facebook.php for a more reliable version of this function.  We skip
 * validation because we do not yet know the app secret.
 */
function _fb_settings_parse_signed_request($signed_request) {
  list($encoded_sig, $payload) = explode('.', $signed_request, 2);

  // decode the data
  $sig = _fb_settings_base64_url_decode($encoded_sig);
  $data = json_decode(_fb_settings_base64_url_decode($payload), TRUE);

  return $data;
}

/**
 * Get the fb_settings from a parsed signed request.
 * http://developers.facebook.com/docs/authentication/canvas
 */
function _fb_settings_honor_signed_request($sr) {
  if (isset($sr['profile_id'])) {
    // Only on tabs.
    fb_settings(FB_SETTINGS_PROFILE_ID, $sr['profile_id']);
    fb_settings(FB_SETTINGS_TYPE, FB_SETTINGS_TYPE_PROFILE);
    if ($sr['user_id'] != $sr['profile_id']) {
      fb_settings(FB_SETTINGS_FBU, $sr['user_id']);
    }
  }
  elseif (isset($sr['user_id'])) {
    // Probably a canvas page.
    fb_settings(FB_SETTINGS_FBU, $sr['user_id']);
  }

  if (isset($sr['oauth_token'])) {
    fb_settings(FB_SETTINGS_TOKEN, $sr['oauth_token']);
    $tokens = explode('|', $sr['oauth_token']);
    if ($app_id = $tokens[0]) {
      fb_settings(FB_SETTINGS_ID, $app_id);
    }
  }
}

/**
 * Parse a facebook session cookie.  Based on sample code
 * http://developers.facebook.com/docs/guides/web.
 */
function fb_settings_get_facebook_cookie($app_id, $application_secret = NULL) {
  if (!isset($_COOKIE['fbs_' . $app_id]))
    return;

  $args = array();
  parse_str(trim($_COOKIE['fbs_' . $app_id], '\\"'), $args);
  if ($application_secret) {
    ksort($args);
    $payload = '';
    foreach ($args as $key => $value) {
      if ($key != 'sig') {
        $payload .= $key . '=' . $value;
      }
    }
    if (md5($payload . $application_secret) != $args['sig']) {
      return NULL;
    }
  }
  if (!isset($args['session_key'])) {
    // Session key missing first time facebook connect page is loaded (?)
    if ($access_token = $args['access_token']) {
      $tokens = explode('|', $access_token);
      $args['session_key'] = $tokens[1] . '|' . $tokens[2];
    }
  }
  return $args;
}


/**
 * By changing the $cookie_domain, we force drupal to use a different session
 * when a user is logged into a facebook application.  We base the
 * $cookie_domain on the apikey of the application, if we can learn it.
 *
 * Facebook provides a number of "migrations" and historically has offered
 * different data to applications.  So the code below tries a variety of ways
 * to learn the settings.
 */

if (function_exists('_fb_settings_parse') &&
    ($apikey = _fb_settings_parse(FB_SETTINGS_CB))) {
  // Learned apikey from url rewrite.
  // Either canvas page or profile tab.
  fb_settings(FB_SETTINGS_APIKEY, $apikey);
  fb_settings(FB_SETTINGS_TYPE, FB_SETTINGS_TYPE_CANVAS);
  if (isset($_REQUEST['signed_request']) &&
      ($sr = _fb_settings_parse_signed_request($_REQUEST['signed_request']))) {
    // Prefer signed request data to cookie data.
    _fb_settings_honor_signed_request($sr);
  }
  else {
    $data = fb_settings_get_facebook_cookie($apikey);
    if (isset($data)) {
      if (isset($data['uid'])) {
        fb_settings(FB_SETTINGS_FBU, $data['uid']);
      }
    }
  }
}
elseif (isset($_REQUEST['signed_request']) &&
        ($sr = _fb_settings_parse_signed_request($_REQUEST['signed_request']))) {
  // Reach this clause on canvas page when admin has not enabled url_rewrite.
  // http://developers.facebook.com/docs/authentication/canvas
  // We get useful info from signed_request only when user is logged in and therefore oauth_token is set.
  fb_settings(FB_SETTINGS_TYPE, FB_SETTINGS_TYPE_CANVAS);
  _fb_settings_honor_signed_request($sr);
}
elseif (isset($_REQUEST['session'])) {
  // New SDK's use session param for canvas pages.
  // Deprecated?  'session' has been replaced with 'signed_request'?
  $session = json_decode($_REQUEST['session'], TRUE);
  fb_settings(FB_SETTINGS_TYPE, FB_SETTINGS_TYPE_CANVAS);
  fb_settings(FB_SETTINGS_FBU, $session['uid']);
  fb_settings(FB_SETTINGS_TOKEN, $session['access_token']);
  // Which app?
  $access_tokens = explode('|', $session['access_token']);
  if ($app_id = $access_tokens[0]) {
    fb_settings(FB_SETTINGS_ID, $app_id);
  }
}
else {
  // We're not in a canvas page.
  // We might be in a facebook connect page.  We have to inspect cookies to make sure.
  $apikey = isset($conf['fb_apikey']) ? $conf['fb_apikey'] : NULL;
  $secret = isset($conf['fb_secret']) ? $conf['fb_secret'] : NULL;
  if ($apikey) {
    $session = fb_settings_get_facebook_cookie($apikey, $secret);
    // Honor connect session only when cookie is set.
    if (count($session)) {
      fb_settings(FB_SETTINGS_APIKEY, $apikey);
      fb_settings(FB_SETTINGS_TYPE, FB_SETTINGS_TYPE_CONNECT);
      fb_settings(FB_SETTINGS_FBU, $session['uid']);
    }
  }
  else {
    // Less efficent cookie inspection.
    // Could inspect wrong cookie if more than one application supported!!!!
    $session_key = '';

    foreach ($_COOKIE as $key => $value) {
      if (strpos($key, 'fbs_') === 0) {
        $apikey = substr($key, 4);
        $session = fb_settings_get_facebook_cookie($apikey);
        if (count($session)) {
          fb_settings(FB_SETTINGS_APIKEY, $apikey);
          fb_settings(FB_SETTINGS_TYPE, FB_SETTINGS_TYPE_CONNECT);
          fb_settings(FB_SETTINGS_FBU, $session['uid']);
          break;
        }
      }
    }
  }
}

if (fb_settings(FB_SETTINGS_TYPE) != FB_SETTINGS_TYPE_CONNECT) {
  // Cookie domain unique to app and page type.
  $unique_id = fb_settings(FB_SETTINGS_APIKEY) ? fb_settings(FB_SETTINGS_APIKEY) : fb_settings(FB_SETTINGS_ID);
  $cookie_domain = isset($cookie_domain) ? $cookie_domain : '' .
    fb_settings(FB_SETTINGS_TYPE) . $unique_id;
  fb_settings(FB_SETTINGS_COOKIE_DOMAIN, $cookie_domain); // for debugging.
}
if (fb_settings(FB_SETTINGS_FBU)) {
  // Disable Drupal cache when logged into facebook.
  $conf['cache'] = 0;
}
