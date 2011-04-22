<?php
/**
 * @file
 *   Notifications for anonymous users
 */

// We will reuse some of the main Notifications form
require_once drupal_get_path('module', 'notifications') . '/notifications.pages.inc';

/**
 * Menu callback add subscription
 * 
 * As it needs an address every time, no need for signed pages
 */
function notifications_anonymous_page_subscribe($type, $fields, $values, $send_interval = NULL, $send_method = NULL) {
  $account = drupal_anonymous_user();

  // Build subscriptions object  
  $subscription = Notifications_Subscription::build((object)array(
    'uid' => $account->uid,
    'type' => $type,
    'send_interval' => $send_interval,
    'send_method' => $send_method,
  ));
  $subscription->set_account($account);
  $subscription->add_field_args($fields, $values);
  // Check permissions and if allowed, ask for confirmation
  if (notifications_user_allowed_subscription($account, $subscription)) {
    drupal_set_title(t('Confirm your subscription'));
    return drupal_get_form('notifications_anonymous_subscription_confirm_form', $subscription);
  }
  else {
    drupal_set_message(t('Subscription type or parameters not allowed'), 'error');
    drupal_goto();
  }
    
  drupal_access_denied();
}

/**
 * Page callback. Provide a form to request to edit a destination
 */
function notifications_anonymous_request_page() {
  $output = '<p>' . t('If you want to unsubscribe or manage subscriptions for a destination, enter the address here and click on Submit.') . '</p>';
  $output .= '<p>' . t('You will get a message with a link that will allow you to manage your subscriptions.') . '</p>';
  $output .= drupal_get_form('notifications_anonymous_request_form');
  return $output;  
}


/**
 * Page callback. Manage anonymous destinations.
 */
function notifications_anonymous_destination_page($destination, $op = 'unsubscribe') {
  notifications_include('destination.inc');
  // Just for administrators or signed requests
  if (!user_access('administer notifications') && !notifications_check_signature()) {
    drupal_access_denied();
    exit(0);
  }
  drupal_set_title(t('Subscriptions for @name', array('@name' => $destination->address_name())));
  $options = notifications_anonymous_manage_links('destination', $destination, 'link');
  switch ($op) {
    case 'edit':
      $output .= drupal_get_form('notifications_destination_edit_form', $destination, $options);
      break;
    case 'delete':
    case 'unsubscribe':
      $output .= drupal_get_form('notifications_destination_unsubscribe_form', $destination, $options);
      break;
    case 'manage':
      $output .= drupal_get_form('notifications_destination_manage_form', $destination, $options);
      break;
  }
  return $output;
}

/**
 * Page callback. Manage anonymous subscriptions.
 */
function notifications_anonymous_subscription_page($subscription, $op = 'unsubscribe') {
  module_load_include('pages.inc', 'notifications');
  // Just for administrators or signed requests
  if (!user_access('administer notifications') && !notifications_check_signature()) {
    drupal_access_denied();
    exit(0);
  }

  switch ($op) {
    case 'unsubscribe':
      $output .= drupal_get_form('notifications_form_unsubscribe_confirm', $subscription);
      break;
    case 'edit':
      // After editing this subscription, forward to the same signed page
      $output .= drupal_get_form('notifications_anonymous_subscription_edit_form', $subscription);
      break;
  }
  return $output;
}

/**
 * Admin settings form
 */
function notifications_anonymous_admin_settings_form() {
  $anonymous_methods = array();
  foreach (messaging_method_info() as $method => $info) {
    if (!empty($info['anonymous'])) {
      $anonymous_methods[$method] = check_plain($info['name']);
    }
  }
  $form['notifications_anonymous_send_methods'] = array(
    '#title' => t('Allowed messaging methods'),
    '#type' => 'checkboxes',
    '#options' => $anonymous_methods,
    '#default_value' => variable_get('notifications_anonymous_send_methods', array('mail')),
    '#description' => t('Select send methods allowed for anonymous users. Only one of each type (I.e. only one mail method) is allowed.'),
  );
  $form['notifications_anonymous_send_intervals'] = array(
    '#title' => t('Allowed send intervals'),
    '#type' => 'checkboxes',
    '#options' => _notifications_send_intervals(),
    '#default_value' => variable_get('notifications_anonymous_send_intervals', array(0)),  
  );
  // We cannot use form filter because of inmediate send interval is always filtered out
  //$form['array_filter'] = array('#type' => 'value', '#value' => TRUE);
  $form = system_settings_form($form);
  $form['#validate'][] = 'notifications_anonymous_admin_settings_form_validate';
  return $form;
}

/**
 * Validate admin settings
 */
function notifications_anonymous_admin_settings_form_validate($form, &$form_state) {
  $send_methods = array_filter($form_state['values']['notifications_anonymous_send_methods']);
  $form_state['values']['notifications_anonymous_send_methods'] = $send_methods;
  // Check we have only one of each type
  $types = array();
  foreach ($send_methods as $method) {
    $type = messaging_method_info($method, 'address_type');
    if (in_array($type, $types)) {
      form_set_error('notifications_anonymous_send_methods', t('Only one method of each type is allowed.'));
      break;
    }
    $types[] = $type;
  }
}
