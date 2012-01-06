<?php

// Setup Mixpanel on this page
function _bbcom_metrics_mp_init() {
  global $user;

  if ($mixpanel_token = variable_get('bbcom_metrics_mixpanel_token', '')) {
    $info = array(
      'mixpanel_token' => $mixpanel_token,
      'commands'       => array(),
    );

    // make sure the commands identifying the user come first
    if (!empty($user->uid)) {
      $info['commands'][] = array('identify', array((string)$user->uid));
      $info['commands'][] = array('name_tag', array($user->name));
    }

    drupal_add_js(array('bbcom_metrics' => $info), 'setting');
    drupal_add_js(drupal_get_path('module', 'bbcom_metrics') . '/bbcom_metrics.js');
    // make sure this is run as early as possible
    drupal_add_js('bbcom_metrics_init();', 'inline');
  }
}

// Merge a new command into the list of commands that will be run by JavaScript on page load
function _bbcom_metrics_mp_javascript($func = NULL, $args = array()) {
  if (variable_get('bbcom_metrics_mixpanel_token', FALSE)) {
    drupal_add_js(array(
      'bbcom_metrics' => array(
        'commands' => array(array($func, $args))
      )
    ), 'setting');
  }
}

// Calls directly to the mixpanel API from the backend
function _bbcom_metrics_mp_backend($event, $properties = array()) {
  if ($mixpanel_token = variable_get('bbcom_metrics_mixpanel_token', '')) {
    $properties['token'] = $mixpanel_token;

    $params = array(
      'event' => $event,
      'properties' => $properties,
    );
    $url = 'http://api.mixpanel.com/track/?data=' . base64_encode(json_encode($params));
    $res = drupal_http_request($url);
    // TODO: do some error handling?
  }
}

// Create signature for the Mixpanel export API
function _bbcom_metrics_mp_signature($params, $api_secret) {
  ksort($params);
  $param_string = '';
  foreach ($params as $param => $value) {
    $param_string .= $param . '=' . $value;
  }
  return md5($param_string . $api_secret);
}

// Access the Mixpanel export API
function _bbcom_metrics_mp_export($method, $params) {
  $api_key = variable_get('bbcom_metrics_mixpanel_key', '');
  $api_secret = variable_get('bbcom_metrics_mixpanel_secret', '');

  if (empty($api_key) || empty($api_secret)) {
    return array('error' => "Must set the Mixpanel API key and secret");
  }

  $params['api_key'] = $api_key;
  $params['format'] = 'json';

  if (!isset($params['expire'])) {
    // default to 10 minutes from now
    $params['expire'] = time() + 600;
  }

  $param_query = '';
  foreach ($params as $param => &$value) {
    if (is_array($value)) {
      $value = json_encode($value);
    }
    $param_query .= '&' . urlencode($param) . '=' . urlencode($value);
  }

  $signature = _bbcom_metrics_mp_signature($params, $api_secret);

  $request_url = 'http://mixpanel.com/api/2.0/' . $method . '/?sig=' . $signature . $param_query;
  $res = drupal_http_request($request_url);
  return json_decode($res->data, TRUE);
}

