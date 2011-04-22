<?php 
// $Id: l10n_update.inc,v 1.9.2.3 2010/09/15 09:25:18 jareyero Exp $
/**
 * @file
 *   Reusable API for l10n remote updates
 */

include_once './includes/locale.inc';
include_once 'l10n_update.locale.inc';

/**
 * Default update server, filename and URL
 */
define('L10N_UPDATE_DEFAULT_SERVER', 'localize.drupal.org');
define('L10N_UPDATE_DEFAULT_SERVER_URL', 'http://localize.drupal.org/l10n_server.xml');
define('L10N_UPDATE_DEFAULT_UPDATE_URL', 'http://ftp.drupal.org/files/translations/%core/%project/%project-%release.%language.po');
// Translation filename, will be used just for local imports
define('L10N_UPDATE_DEFAULT_FILENAME', '%project-%release.%language.po');
// Translation status: String imported from po
define('L10N_UPDATE_STRING_DEFAULT', 0);
// Translation status: Custom string, overridden original import
define('L10N_UPDATE_STRING_CUSTOM', 1);

/**
 * Retrive data for default server
 */
function l10n_update_default_server() {
  return array(
    'name' => variable_get('l10n_update_default_server', L10N_UPDATE_DEFAULT_SERVER),
    'server_url' => variable_get('l10n_update_default_server_url', L10N_UPDATE_DEFAULT_SERVER_URL),
    'update_url' => variable_get('l10n_update_default_update_url', L10N_UPDATE_DEFAULT_UPDATE_URL)
  );
}

/**
 * Download and import remote file
 */
function l10n_update_download_import($download_url, $locale, $mode = LOCALE_IMPORT_OVERWRITE) {
  if ($file = l10n_update_download_file($download_url)) {
    $result = l10n_update_import_file($file, $locale, $mode);
    return $result;
  }
}

/**
 * Import local file into the database
 * @param $file
 * @param $locale
 * @param $mode
 * @return unknown_type
 */
function l10n_update_import_file($file, $locale, $mode = LOCALE_IMPORT_OVERWRITE) {   
  // If the file is a filepath, create a $file object
  if (is_string($file)) {
    $filepath = $file;
    $file = new stdClass();
    $file->filepath = $filepath;
    $file->filename = $filepath;
  }
  return _l10n_update_locale_import_po($file, $locale, $mode , 'default');
}

/**
 * Get remote file and download it to a temporary path
 */
function l10n_update_download_file($download_url, $destination = NULL) {
  $t = get_t();
  $variables['%download_link'] = $download_url;
  
  $file = $destination ? $destination : tempnam(file_directory_temp(), 'translation-');

  if ($file) {
    $variables['%tmpfile'] = $file;
    if (($contents = drupal_http_request($download_url)) && file_put_contents($file, $contents->data)) {
      watchdog('l10n_update', 'Successfully downloaded %download_link to %tmpfile', $variables);
      return $file; 
    }
    else {
      watchdog('l10n_update', 'Unable to download and save %download_link file to %tmpfile.', $variables, WATCHDOG_ERROR);
    }
  }
  else {
    $variables['%tmpdir'] = file_directory_temp();
    watchdog('l10n_update', 'Error creating temporary file for download in %tmpdir. Remote file is %download_link.', $variables, WATCHDOG_ERROR);
  }  
}

/**
 * Get names for the language list from locale system
 */
function l10n_update_get_language_names($string_list) {
  $t = get_t();
  $language_codes = array_map('trim', explode(',', $string_list));
  $languages = _locale_get_predefined_list();
  $result = array();
  foreach ($language_codes as $lang) {      
    if (array_key_exists($lang, $languages)) {
      // Try to use verbose locale name
      $name = $lang;
      $name = $languages[$name][0] . (isset($languages[$name][1]) ? ' '. $t('(@language)', array('@language' => $languages[$name][1])) : '');
      $result[$lang] = $name;
    }
  }
  return $result;
}

/**
 * Build project data as an object
 */
function _l10n_update_build_project($name, $version = NULL, $server = L10N_UPDATE_DEFAULT_SERVER, $path = L10N_UPDATE_DEFAULT_URL) {
  $project = new stdClass();
  $project->name = $name;
  $project->version = $version;
  $project->l10n_server = $server;
  $project->l10n_path = $path;
  return $project;
}

/**
 * Update the file history table
 * 
 * @param $file
 *   Object representing the file just imported or downloaded
 */
function l10n_update_file_history($file) {
  // Update or write new record
  if (db_result(db_query("SELECT project FROM {l10n_update_file} WHERE project = '%s' AND language = '%s'", $file->project, $file->language))) {
    $update = array('project', 'language');
  }
  else {
    $update = array();
  }
  return drupal_write_record('l10n_update_file', $file, $update);
}

/**
 * Check file existence and last updated time
 */
function l10n_update_http_check($url, $headers = array()) {
  $result = l10n_update_http_request($url, $headers, 'HEAD');
  if ($result && $result->code == '200') {
    $result->updated = isset($result->headers['Last-Modified']) ? strtotime($result->headers['Last-Modified']) : 0;
  }
  return $result;
}
/**
 * Perform an HTTP request. Installer safe simplified version.
 * 
 * We cannot use drupal_http_request() at install, see http://drupal.org/node/527484
 * 
 * This one also handles FTP
 * 
 * This is a flexible and powerful HTTP client implementation. Correctly handles
 * GET, POST, PUT or any other HTTP requests. Handles redirects.
 *
 * @param $url
 *   A string containing a fully qualified URI.
 * @param $headers
 *   An array containing an HTTP header => value pair.
 * @param $method
 *   A string defining the HTTP request to use.
 * @param $data
 *   A string containing data to include in the request.
 * @param $retry
 *   An integer representing how many times to retry the request in case of a
 *   redirect.
 * @param $check
 *   Just check file existence and headers (update time)
 * @return
 *   An object containing the HTTP request headers, response code, headers,
 *   data and redirect status.
 */
function l10n_update_http_request($url, $headers = array(), $method = 'GET', $data = NULL, $retry = 3) {
  $result = new stdClass();
  // Return also full URL string
  $result->url = $url;
  // Parse the URL and make sure we can handle the schema.
  $uri = parse_url($url);

  if ($uri == FALSE) {
    $result->error = 'unable to parse URL';
    return $result;
  }

  if (!isset($uri['scheme'])) {
    $result->error = 'missing schema';
    return $result;
  }

  switch ($uri['scheme']) {
    case 'http':
      $port = isset($uri['port']) ? $uri['port'] : 80;
      $host = $uri['host'] . ($port != 80 ? ':'. $port : '');
      $fp = @fsockopen($uri['host'], $port, $errno, $errstr, 15);
      break;
    case 'https':
      // Note: Only works for PHP 4.3 compiled with OpenSSL.
      $port = isset($uri['port']) ? $uri['port'] : 443;
      $host = $uri['host'] . ($port != 443 ? ':'. $port : '');
      $fp = @fsockopen('ssl://'. $uri['host'], $port, $errno, $errstr, 20);
      break;  
    default:
      $result->error = 'invalid schema '. $uri['scheme'];
      return $result;
  }

  // Make sure the socket opened properly.
  if (!$fp) {
    // When a network error occurs, we use a negative number so it does not
    // clash with the HTTP status codes.
    $result->code = -$errno;
    $result->error = trim($errstr);

    // Mark that this request failed. This will trigger a check of the web
    // server's ability to make outgoing HTTP requests the next time that
    // requirements checking is performed.
    // @see system_requirements()
    // variable_set('drupal_http_request_fails', TRUE);

    return $result;
  }

  // Construct the path to act on.
  $path = isset($uri['path']) ? $uri['path'] : '/';
  if (isset($uri['query'])) {
    $path .= '?'. $uri['query'];
  }

  // Create HTTP request.
  $defaults = array(
    // RFC 2616: "non-standard ports MUST, default ports MAY be included".
    // We don't add the port to prevent from breaking rewrite rules checking the
    // host that do not take into account the port number.
    'Host' => "Host: $host",
    'User-Agent' => 'User-Agent: Drupal (+http://drupal.org/)',
    'Content-Length' => 'Content-Length: '. strlen($data)
  );

  // If the server url has a user then attempt to use basic authentication
  if (isset($uri['user'])) {
    $defaults['Authorization'] = 'Authorization: Basic '. base64_encode($uri['user'] . (!empty($uri['pass']) ? ":". $uri['pass'] : ''));
  }

  // If the database prefix is being used by SimpleTest to run the tests in a copied
  // database then set the user-agent header to the database prefix so that any
  // calls to other Drupal pages will run the SimpleTest prefixed database. The
  // user-agent is used to ensure that multiple testing sessions running at the
  // same time won't interfere with each other as they would if the database
  // prefix were stored statically in a file or database variable.
  if (preg_match("/simpletest\d+/", $GLOBALS['db_prefix'], $matches)) {
    $defaults['User-Agent'] = 'User-Agent: ' . $matches[0];
  }

  foreach ($headers as $header => $value) {
    $defaults[$header] = $header .': '. $value;
  }

  $request = $method .' '. $path ." HTTP/1.0\r\n";
  $request .= implode("\r\n", $defaults);
  $request .= "\r\n\r\n";
  $request .= $data;

  $result->request = $request;

  fwrite($fp, $request);

  // Fetch response. If in test mode, don't fetch data;
  $response = '';
  while (!feof($fp) && $chunk = fread($fp, 1024)) {
    $response .= $chunk;
  }
  fclose($fp);

  // Parse response.
  list($split, $result->data) = explode("\r\n\r\n", $response, 2);
  $split = preg_split("/\r\n|\n|\r/", $split);

  list($protocol, $code, $text) = explode(' ', trim(array_shift($split)), 3);
  $result->headers = array();

  // Parse headers.
  while ($line = trim(array_shift($split))) {
    list($header, $value) = explode(':', $line, 2);
    if (isset($result->headers[$header]) && $header == 'Set-Cookie') {
      // RFC 2109: the Set-Cookie response header comprises the token Set-
      // Cookie:, followed by a comma-separated list of one or more cookies.
      $result->headers[$header] .= ','. trim($value);
    }
    else {
      $result->headers[$header] = trim($value);
    }
  }

  $responses = array(
    100 => 'Continue', 101 => 'Switching Protocols',
    200 => 'OK', 201 => 'Created', 202 => 'Accepted', 203 => 'Non-Authoritative Information', 204 => 'No Content', 205 => 'Reset Content', 206 => 'Partial Content',
    300 => 'Multiple Choices', 301 => 'Moved Permanently', 302 => 'Found', 303 => 'See Other', 304 => 'Not Modified', 305 => 'Use Proxy', 307 => 'Temporary Redirect',
    400 => 'Bad Request', 401 => 'Unauthorized', 402 => 'Payment Required', 403 => 'Forbidden', 404 => 'Not Found', 405 => 'Method Not Allowed', 406 => 'Not Acceptable', 407 => 'Proxy Authentication Required', 408 => 'Request Time-out', 409 => 'Conflict', 410 => 'Gone', 411 => 'Length Required', 412 => 'Precondition Failed', 413 => 'Request Entity Too Large', 414 => 'Request-URI Too Large', 415 => 'Unsupported Media Type', 416 => 'Requested range not satisfiable', 417 => 'Expectation Failed',
    500 => 'Internal Server Error', 501 => 'Not Implemented', 502 => 'Bad Gateway', 503 => 'Service Unavailable', 504 => 'Gateway Time-out', 505 => 'HTTP Version not supported'
  );
  // RFC 2616 states that all unknown HTTP codes must be treated the same as the
  // base code in their class.
  if (!isset($responses[$code])) {
    $code = floor($code / 100) * 100;
  }

  switch ($code) {
    case 200: // OK
    case 304: // Not modified
      break;
    case 301: // Moved permanently
    case 302: // Moved temporarily
    case 307: // Moved temporarily
      $location = $result->headers['Location'];

      if ($retry) {
        $result = l10n_update_http_request($result->headers['Location'], $headers, $method, $data, --$retry);
        $result->redirect_code = $result->code;
      }
      $result->redirect_url = $location;

      break;
    default:
      $result->error = $text;
  }

  $result->code = $code;
  return $result;
}

/**
 * Build abstract translation source, to be mapped to a file or a download
 */
function l10n_update_build_string($project, $template) {
  $variables = array(
    '%project' => $project->name,
    '%release' => $project->version,
    '%core' => $project->core,
    '%language' => isset($project->language) ? $project->language : '%language',
    '%filename' => isset($project->filename) ? $project->filename : '%filename',
  );
  return strtr($template, $variables);
}

