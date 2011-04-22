<?php
/**
 * @file
 * User pages for Notifications
 */

/**
 * Menu callback. Overview page for user subscriptions.
 * 
 * We create the page as a form so it can be altered
 * 
 */
function notifications_user_overview($form_state, $account) {
  $form['account'] = array('#type' => 'value', '#value' => $account);
  
  $path = 'user/' . $account->uid;
  
  // Status, send method, interval
  $count = db_query("SELECT status, COUNT(*) AS num FROM {notifications} WHERE uid = %d GROUP BY status", $account->uid);
  $status_list = Notifications_Subscription::status_list();
  while ($current = db_fetch_object($count)) {
    $status[$current->status] = t('You have %count @status subscriptions.', array('%count' => $current->num, '@status' => $status_list[$current->status]));
  }
  if (empty($status)) {
    $status['none'] = t('You don\'t have any subscriptions yet.');
  }
  $send_intervals = notifications_send_intervals();
  $send_methods = _notifications_send_methods($account);
  if ($method = notifications_user_setting('send_method', $account)) {
    if (isset($send_methods[$method])) {      
      $status['method'] = t('Your default sending method for new subscriptions is @send_method', array('@send_method' => $send_methods[$method]));
    }
  }
  $interval = notifications_user_setting('send_interval', $account);
  if (isset($send_intervals[$interval])) {      
    $status['interval'] = t('Your default sending interval for new subscriptions is @send_interval', array('@send_interval' => $send_intervals[$interval]));
  }
  
  $form['status'] = array('#type' => 'item', '#weight' => 10, '#title' => t('Current status'), '#value' => theme('item_list', $status));
  
  // Build shortcut tips
  if (notifications_access_user($account, 'manage')) {
    $tips['admin'] = l(t('Administer your subscriptions'), "$path/notifications/subscriptions");
  }
  $tips['edit'] = l(t('Edit your notifications settings'), "$path/edit");
  
  // Enable / disable all subscriptions
  if (notifications_access_user($account, 'maintain')) {
    if (!empty($status[Notifications_Subscription::STATUS_ACTIVE])) {
      $tips['disable'] = l(t('Temporarily disable all your subscriptions'), "$path/notifications/update/disable");
    }
    if (!empty($status[Notifications_Subscription::STATUS_INACTIVE]) || !empty($status[Notifications_Subscription::STATUS_BLOCKED])) {
      $tips['enable'] = l(t('Enable all your subscriptions'), "$path/notifications/update/enable");
    }    
  }
  $link = notifications_get_link('unsubscribe', array('uid' => $account->uid, 'destination' => TRUE));
  $tips['cancel'] = l(t('Cancel all your subscriptions'), $link['href'], $link['options']);
  
  //$output .= theme('item_list', $tips, t('You can'));
  $form['tips'] = array('#type' => 'item', '#weight' => 20, '#title' => t('You can'), '#value' => theme('item_list', $tips));
   
  return $form;  
}

/**
 * Manage destination form. Edit subscriptions for a destination
 */
function notifications_edit_destination_form($form_state, $destination) {
  notifications_include('destination.inc');
  $account = $destination->get_user();
  $form = notifications_destination_view_subform($destination);
  
  return $form;
}

/**
 * Menu callback add subscription
 * 
 * Presents confirmation page or not depending on confirm parameter
 */
function notifications_page_subscribe($account, $type, $fields, $values, $send_interval = NULL, $send_method = NULL) {
  // Build subscriptions object
  $subscription = notifications_build_subscription(array(
    'uid' => $account->uid,
    'type' => $type,
    'send_interval' => $send_interval,
    'send_method' => $send_method,
  ));
  $subscription->add_field_args($fields, $values);
  $subscription->set_account($account);

  // Check permission again for these parameters
  if ($account->uid && notifications_page_check_access($account->uid) && notifications_user_allowed_subscription($account, $subscription)) {
    if (notifications_page_check_confirmation()) {
      // Subscribe, no confirmation
      notifications_save_subscription($subscription);
      drupal_set_message(t('Your subscription was activated.'));
      drupal_goto();
    }
    else {
      // Ask for confirmation
      drupal_set_title(t('Confirm your subscription'));
      return drupal_get_form('notifications_subscription_confirm_form', $subscription);
    }
  }
  else {
    drupal_set_message(t('Subscription type or parameters not allowed'), 'error');
    drupal_goto();
  }
    
  drupal_access_denied();
}

/**
 * Menu callback for unsubscribe page
 * 
 * @param $type
 *   Either 'sid' or 'uid' (for unsubscribe all)
 * @param $id
 *   Subscription id or user id, depending on type
 */
function notifications_page_unsubscribe($type = NULL, $id = NULL) {
  // Determine subscription and user depending on type
  if (!$type) {
    // Go to generic unsubscribe page with some more options
    return notifications_page_unsubscribe_overview();    
  }
  elseif ($type == 'subscription' && is_numeric($id) && ($subscription = notifications_load_subscription($id))) {
    $account = NULL;
    $owner = $subscription->uid;
  }
  elseif ($type == 'user' && is_numeric($id) && ($account = user_load($id)) && $account->uid) {
    $subscription = NULL;
    $owner = $account->uid;
  }
  
  // Check permissions and present confirmation form or not depending on parameters
  if (($account || $subscription) && notifications_page_check_access($owner)) {
    // Skip confirmation page when requested and the signature is ok
    if (notifications_page_check_confirmation()) {
      if ($subscription) {
        notifications_subscription_delete($subscription->sid);
        drupal_set_message(t('Your subscription has been removed.'));
      }
      elseif ($account) {
        notifications_delete_subscriptions(array('uid' => $account->uid));
        drupal_set_message(t('All your subscriptions have been removed.'));
      }
      drupal_goto();
    } 
    elseif ($account && !db_result(db_query("SELECT COUNT(*) FROM {notifications} WHERE uid = %d", $account->uid))) {
      return t("You don't have any subscription on this site.");
    }
    else {
      // Display confirmation form
      return drupal_get_form('notifications_form_unsubscribe_confirm', $subscription, $account);
    }
  }
  drupal_access_denied();
}

/**
 * Display unsubscribe options for this page
 */
function notifications_page_unsubscribe_overview() { 
  global $user;
  notifications_include('destination.inc');

  //drupal_set_title(t('Unsubscribe'));
  $output = '';
  $send_methods = notifications_destination_anonymous_methods();
  $subscriptions = 0;
  // Provide information for user subscriptions if logged in
  if ($user->uid) {
    $subscriptions = db_result(db_query("SELECT COUNT(*) FROM {notifications} WHERE uid = %d", $user->uid));
    if ($subscriptions) {
      $output .= '<p>' . format_plural($subscriptions, 'There is one subscription for this user account.', 'There are @count subscriptions for this user account.') . '</p>';
      $output .= '<p>' . t('You can:') . '<br />';
      $options = array();
      if (notifications_access_user($account, 'maintain')) {
        $options[] = l(t('Administer your subscriptions'), "user/$user->uid/notifications");
        $options[] = l(t('Temporarily disable all your subscriptions'), "user/$user->uid/notifications/update/disable");
      }
      $options[] = notifications_build_link('unsubscribe', array('title' => t('Cancel all your subscriptions')), 'user', $user, 'link');
      $output .= theme('item_list', $options);      
    }
    else {
      $output .= '<p>' . t('There are no subscriptions for this user account') . '</p>';
      if ($send_methods) {
        $output .= '<p>' . t('However if you are still getting notifications from this site, enter your address below and you\'ll get a link to disable that address.') . '</p>';
      }
    }
  }

  if (!$subscriptions && $send_methods) {
    // Present destination unsubscribe request form
    $output .= drupal_get_form('notifications_destination_request_form', $account, $send_methods);
  }
  elseif (!$user->uid) {
    // WTF? Someone is getting spam from here?
    $output .= '<p>' . t('You may have an account on this site or someone may have created it for you.') . '</p>';
    $output .= '<p>' . t('You can try to <a href="@user-login">log in</a>, request a <a href="@request-password">new password</a>, or contact the site administrator.', array('@user-login' => url('user/login'), '@request-password' => url('user/password'))) . '</p>';
  }
  return $output;
}

/**
 * Check access to current page, included signature
 * 
 * @param $owner
 *   User id, owner of the object in the page (subscription, destination, account)
 *   
 */
function notifications_page_check_access($owner) {
  global $user;
  
  if (user_access('administer notifications')) {
    return TRUE; // Administrator can access all
  }
  elseif ($owner) {
    // Current user is the owner or the URL is signed
    return $user->uid == $owner || notifications_page_check_signature();
  }
  elseif (module_exists('notifications_anonymous')) {
    // Owner is anonymous but we just allow it if the module is enabled the URL is signed
    return notifications_page_check_signature();
  }
}

/**
 * Check current URL is signed
 */
function notifications_page_check_signature() {
  static $signed;
  if (!isset($signed)) {
    $signed = !empty($_GET['signature']) && notifications_check_signature();
  }
  return $signed;
}

/**
 * Check URL is signed and it doesn't need confirmation
 */
function notifications_page_check_confirmation() {
  return !empty($_GET['confirm']) && notifications_page_check_signature();
}

/**
 * Form for unsubscription confirmation
 * 
 * It works for both single subscription or account (all subscriptions)
 */
function notifications_form_unsubscribe_confirm($form_state, $subscription, $account = NULL) {
  // Pass on subscription values
  $form['subscription'] = array('#type' => 'value', '#value' => $subscription);
  $form['account'] = array('#type' => 'value', '#value' => $account);
  if ($subscription) {
    $message = t('Are you sure you want to delete this subscription?');
    // The subscription description will be added here
    $form['info'] = $subscription->form_info();    
  }
  else {
    $message = t('Are you sure you want to remove all your subscriptions on this site?');
  }

  return confirm_form($form,
    $message,
    isset($_GET['destination']) ? $_GET['destination'] : '',
    t('This action cannot be undone.'),
    t('Unsubscribe'),
    t('Cancel')
  );
}

/**
 * Process unsubscribe form submission
 */
function notifications_form_unsubscribe_confirm_submit($form, &$form_state) {
  $subscription = $form_state['values']['subscription'];
  $account = $form_state['values']['account'];
  switch ($form_state['values']['op']) {
    case t('Unsubscribe'):
      if ($subscription) {
        notifications_subscription_delete($subscription->sid);
        drupal_set_message(t('Your subscription has been removed.'));
      }
      elseif ($account) {
        notifications_delete_subscriptions(array('uid' => $account->uid));
        drupal_set_message(t('All your subscriptions have been removed.'));
      }
      $form_state['redirect'] = '';
      break;
    case t('Cancel'):
      // Do nothing, not worth showing a message, just get back
      $form_state['redirect'] = '';
      break;
  }
}

/**
 * Edit subscription page that can be accessed with signed link
 */
function notifications_subscription_edit_page($subscription) {
  global $user;

  // Aditional checks for anonymous subscriptions so not everybody can access them
  if (!$subscription->uid && !user_access('administer notifications') && !user_access('manage subscriptions') && !notifications_check_signature()) {
    drupal_access_denied(); 
  }
  else {
    return drupal_get_form('notifications_subscription_edit_form', $subscription);
  }
}

/**
 * Base subscription form. Present the main fields and check all values
 */
function notifications_subscription_base_form($form_state, $subscription, $destination_path = NULL) {
  // We may have a destination query string instead of a destination path
  if (!empty($_REQUEST['destination'])) {
    $destination_path = $_REQUEST['destination'];
  }
  if (isset($destination_path)) {
    $form['#redirect'] = $destination_path;
  }
  
  // Check the subscription object is built properly
  $subscription = notifications_build_subscription($subscription);
  // Pass on whole subscription and then the fields to be saved along
  $form['subscription'] = array('#type' => 'value', '#value' => $subscription);
  $account = $subscription->get_user();
  $save_fields = array();

  // The subscription description will be added here, fields may be editable or not
  if (!$subscription->is_editable()) {
    // Just display general information, no editable fields
    $form['info'] = $subscription->form_info();
  }
  else {
    $form['fields'] = $subscription->fields_subform();
    $save_fields[] = 'fields'; 
  }
  if ($subscription->is_instance()) {
    $form['info']['#title'] = t('Edit subscription');
  }
  else {
    $form['info']['#title'] = t('Create @type subscription', array('@type' => $subscription->get_title()));
  }

  // Send interval, visible only if more than one
  if ($send_intervals = notifications_send_intervals($account)) {
    $save_fields[] = 'send_interval';
    if (count($send_intervals) == 1) {
      $form['send_interval'] = array('#type' => 'value', '#value' => key($send_intervals));
    }
    else {
      $form['notifications']['send_interval'] = array(
        '#type' => 'select',
        '#title' => t('Send interval'),
        '#options' => $send_intervals,
        '#default_value' => $subscription->send_interval,
      );
    }
  }
  else {
    drupal_set_message(t('No sending intervals available.'), 'error');
    $form['#error']['send_interval'] = TRUE;
  }
  // Send method, visible only if more than one
  if ($send_methods = notifications_send_methods($account)) {
    $save_fields[] = 'send_method';
    if (count($send_methods) == 1) {
      $form['send_method'] = array('#type' => 'value', '#value' => key($send_methods));
    }
    else {
      $form['notifications']['send_method'] = array(
        '#type' => 'select',
        '#title' => t('Send method'),
        '#options' => $send_methods,
        '#default_value' => $subscription->send_method,
        '#disabled' => count($send_methods) < 2,
      );
    }
  }
  else {
    drupal_set_message(t('No sending methods available.'), 'error');
    $form['#error']['send_method'] = TRUE;
  }
  // Add notifications fieldset if send method or send_interval
  if (!empty($form['notifications'])) {
    $form['notifications']['#title'] = t('Notifications');
    $form['notifications']['#type'] = 'fieldset';
    $form['notifications']['#description'] = t('How do you want to get notifications for this subscription.');
  }
  // Status field, show blocked only to administrators
  if ($subscription->is_instance()) {
    $save_fields[] = 'status';
    $status = Notifications_Subscription::status_list();
    if (!user_access('administer notifications') && !user_access('manage all subscriptions')) {
      unset($status[Notifications_Subscription::STATUS_BLOCKED]);
    }
    $form['status'] = array(
      '#type' => 'radios',
      '#title' => t('Status'),
      '#options' => $status,
      '#default_value' => $subscription->status,
      '#description' => t('You can temporarily disable this subscription for not getting notifications.')
    );
  }
  // Fields to be saved on submit. If error we just show the 'Cancel' button.
  $form['subscription_fields'] = array('#type' => 'value', '#value' => $save_fields);
  
  // Add form buttons depending on what we are doing
  if ($subscription->is_instance()) {
    $form['actions']['save'] = array('#type' => 'submit', '#value' => t('Save'));
    $form['actions']['delete'] = array('#type' => 'submit', '#value' => t('Delete'));
  }
  elseif ($subscription->is_editable()) {
    $form['actions']['subscribe'] = array('#type' => 'submit', '#value' => t('Create subscription'));
  }
  else {
    // Subscription parameters are fixed so this is just a confirmation form
    $form['actions']['subscribe'] = array('#type' => 'submit', '#value' => t('Subscribe'));
  }
  if (isset($destination_path)) {
    $form['actions']['cancel'] = array('#value' => l(t('Cancel'), $destination_path));
  }
  // Add the right callbacks
  $form['#submit'][] = 'notifications_subscription_form_submit';
  $form['#validate'][] = 'notifications_subscription_form_validate';
  return $form;  
}

/**
 * Validate subscription
 */
function notifications_subscription_base_form_validate($form, &$form_state) {
  Notifications_Subscription::validate_submission($form_state);
}

/**
 * Save edited subscription
 */
function notifications_subscription_base_form_submit($form, &$form_state) {
  $subscription = Notifications_Subscription::build_submission($form_state);
  switch ($form_state['values']['op']) {
    case t('Save'):
    case t('Subscribe'):
    case t('Create subscription'):
      // We are updating or creating a new subscription
      notifications_submit_subscription($subscription);
      break;
    case t('Delete'):
      notifications_subscription_delete($subscription->sid);
      drupal_set_message(t('Your subscription has been deleted.'));
      $form_state['redirect'] = 'user/'. $subscription->uid .'/notifications';
      break;
  }
}

/**
 * Submit subscription printing out the results
 */
function notifications_submit_subscription($subscription) {
  global $user;
  
  $instance = $subscription->is_instance();
  // If a new subscription we may need to set current user
  if (!isset($subscription->uid)) {
    $subscription->set_account($user);
  }
  // Check some parameters are ok (checking fills in some missing values too)
  if (!notifications_check_subscription($subscription)) {
    drupal_set_message($subscription->error_message, 'error');
    return FALSE;
  }

  // Check permissions for current user to create this subscription
  if (!$subscription->check_access($user)) {
    if (!empty($subscription->error_message)) {
      drupal_set_message($subscription->error_message);
    }
    else {
      drupal_set_message(t("You are not allowed to create this type of subscription."), 'error');
    }
    return FALSE;
  }

  // Actually update or create subscription, skipping checks that we've already done.
  $result = notifications_save_subscription($subscription, FALSE);

  switch ($result) {
    case SAVED_NEW:
      drupal_set_message(t('Your subscription has been created.'));
      return TRUE;
    case SAVED_UPDATED:
      if ($instance) {
        drupal_set_message(t('Your subscription has been updated.'));
      }
      else {
        drupal_set_message(t('Your subscription has been created updating an existing one of the same type.'));
      }
      return TRUE;
    default:
      if ($instance) {
        drupal_set_message(t('Your subscription cannot be updated.'), 'error');
      }
      else {
        drupal_set_message(t('Your subscription cannot be created.'), 'error');
      }
      return FALSE;
  }
}

/**
 * Bulk update all user subscriptions, confirmation form
 * 
 * @param $account
 *   User account
 * @param $operation
 *   Operation to perform: enable, disable
 */
function notifications_update_user_subscriptions($form_state, $account, $operation) {
  $form['account'] = array('#type' => 'value', '#value' => $account);
  $form['operation'] = array('#type' => 'value', '#value' => $operation);  
  $destination = 'user/' . $account->uid . '/notifications';
  $form['#redirect'] = $destination;
  if ($operation == 'enable') {
    return confirm_form($form,
                      t('Are you sure you want to enable all your subscriptions?'),
                      $destination, t('You will get notifications again for all of them.'),
                      t('Enable'), t('Cancel'));
  }
  elseif ($operation == 'disable') {
    return confirm_form($form,
                      t('Are you sure you want to disable all your subscriptions?'),
                      $destination, t('You will stop getting notifications until you enable them again.'),
                      t('Disable'), t('Cancel'));    
  }
  else {
    drupal_access_denied();
  }
}

/**
 * Mass update user subscriptions
 */
function notifications_update_user_subscriptions_submit($form, $form_state) {
  $account = $form_state['values']['account'];
  
  switch ($form_state['values']['operation']) {
    case 'enable':
      // Enable also blocked subscriptions just in case there are some
      db_query('UPDATE {notifications} SET status = %d WHERE uid = %d', Notifications_Subscription::STATUS_ACTIVE, $account->uid);
      drupal_set_message(t('All your subscriptions have been enabled.'));
      break;
    case 'disable':
      db_query('UPDATE {notifications} SET status = %d WHERE status = %d AND uid = %d', Notifications_Subscription::STATUS_INACTIVE, Notifications_Subscription::STATUS_ACTIVE, $account->uid);
      notifications_queue()->queue_clean(array('uid' => $account->uid));
      drupal_set_message(t('All your subscriptions have been disabled.'));
      break;
  }
}

/**
 * List form for subscriptions of the same type for a user
 * 
 * @todo Add enable /disable operations
 * @todo Add pager
 * 
 * @param $type
 *   Subscription type
 * @param $account
 *   User account, owner of these subscriptions
 * @param $subscriptions
 *   Predefined array of subscriptions to display, so we don't search more nor allow new ones.
 * @param $fields
 *   Predefined field values for new subscriptions, as type => value array
 */
function notifications_subscription_list_form($form_state, $subscription_type, $account, $subscriptions = NULL, $fields = array()) {
  if (isset($form_state['values']['op']) && $form_state['values']['op'] == t('Delete')) {
    if ($selected = array_filter($form_state['values']['subscriptions']['select'])) {
      $form = notifications_multiple_delete_confirm($form_state, $selected);
      return $form;      
    }
  }
  notifications_include('object.inc');
  $account = messaging_user_object($account);
  $form['account'] = array('#type' => 'value', '#value' => $account);

  // Build a template subscription. These subscriptions should have a unique subscription variable field
  $template = notifications_build_subscription($subscription_type);
  $template->set_account($account);
  $template->set_fields($fields);
  $form['template'] = array('#type' => 'value', '#value' => $template); 
  // Print template information: subscription type, description, fixed fields
  //$form['info'] = $template->form_info();
  $form['info'] = array(
    '#type' => 'item',
    '#title' => t('@type subscriptions', array('@type' => $template->get_title())),
    '#description' => $template->get_description(),
  );

  // Just allow one field of each type to be editable
  $editable_fields = array();
  foreach ($template->get_editable_fields() as $field) {
    $editable_fields[$field->field] = $field;
  }
  $form['editable_fields'] = array('#type' => 'value', '#value' => $editable_fields);

  if (!isset($subscriptions)) {
    // We get all subscriptions that at least have the given fields, but may have more
    $subscriptions = notifications_get_subscriptions(array('type' => $subscription_type, 'uid' => $account->uid), $fields, FALSE);
  }
  // If we don't have subscriptions and we cannot create new ones, just display a message
  if (!$subscriptions) {
    $form['warning']['#value'] = t("There are no subscriptions of this type.");
  }
  $form['current_subscriptions'] = array('#type' => 'value', '#value' => $subscriptions);
  $subscription_list = $subscriptions; 
  // Add our template to the list, but not to the values in form
  if ($editable_fields && notifications_access_user_add($account, $subscription_type)) {
    $subscription_list['new'] = $template;
  }

  // Prepare other parameters to build the list
  $send_methods = notifications_send_methods($account);
  $send_intervals = notifications_send_intervals($account);

  // Build header with selector, fields, send interval, send method if apply
  $header = array(theme('table_select_header_cell'));
  $table_fields = array('select');
  foreach ($editable_fields as $type => $field) {
    $header[] = notifications_field_format_name($field->field);
    $table_fields[] = 'field_' . $type;
  }
  // Hide Send method, send interval columns if only one
  if (count($send_intervals)) {
    $header[] = t('Send interval');
    $table_fields[] = 'send_interval';
  }
  else {
    $send_intervals = NULL;
  }
  if (count($send_methods) > 1) {
    $header[] = t('Send method');
    $table_fields[] = 'send_method';
  }
  else {
    $send_methods = NULL;
  }

  $form['subscriptions'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#theme' => 'notifications_table_form',
    '#table_header' => $header,
    '#table_fields' => $table_fields,
  );
  // Add our template to the subscription list and
  $elements = array();
  foreach ($subscription_list as $key => $subscription) {
    if ($subscription->is_instance()) {
      $form['subscriptions']['select'][$key] = array(
        '#type' => 'checkbox',
        '#default_value' => FALSE, // All unselected by default
      );
    }
    else {
      $form['subscriptions']['select']['new'] = array(
        '#type' => 'submit',
        '#value' => t('Add'),
      );
    }
    foreach ($editable_fields as $type => $field) {
      if ($subscription->is_instance()) {
        $conditions = $subscription->get_conditions();
        $value = $conditions[$type];
        $form['subscriptions']['field_' . $type][$key] = array('#value' => notifications_field_format_value($type, $value));
      }
      else {
        // Add new subscription, editable fields
        $form['subscriptions']['field_' . $type][$key] = notifications_field_form_element($type, NULL, $subscription, FALSE, FALSE, 30);
      }
    }
    if ($send_intervals)
      $form['subscriptions']['send_interval'][$key] = array(
      '#type' => 'select',
      '#options' => $send_intervals,
      '#default_value' => $subscription->send_interval,
    );
    if ($send_methods) {
      $form['subscriptions']['send_method'][$key] = array(
        '#type' => 'select',
        '#options' => $send_methods,
        '#default_value' => $subscription->send_method,
      );
    }
  }
  if ($subscriptions) {
    $form['operations'] = array(
      '#type' => 'fieldset',
    );
    $form['operations']['update'] = array('#type' => 'submit', '#value' => t('Update'));
    $form['operations']['delete'] = array('#type' => 'submit', '#value' => t('Delete'));
    //$form['operations']['enable'] = array('#type' => 'submit', '#value' => t('Enable'));
    //$form['operations']['disable'] = array('#type' => 'submit', '#value' => t('Disable'));
  }
  return $form;
}

/**
 * Submit list of subscriptions
 */
function notifications_subscription_list_form_validate($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : NULL;
  switch ($op) {
    case t('Add'):
      $save_fields = array();
      foreach ($form_state['values']['editable_fields'] as $field) {
        $type = $field->field;
        $key = 'field_' . $type;
        $value = isset($form_state['values']['subscriptions'][$key]['new']) ? $form_state['values']['subscriptions'][$key]['new'] : NULL;
        if ($real_value = notifications_field_real_value($type, $value)) {
          $save_fields[] = array('type' => $type, 'value' => $real_value);
        }
        else {
          form_set_error("subscriptions][$key][new", t('You must set a valid value for this field.'));
        }
      }
      $form_state['values']['parsed_fields'] = $save_fields;
      break;
    case t('Delete'):
    case t('Enable'):
    case t('Disable'):
      // For this cases check only that we have selected some subscriptions
      $selected = array_filter($form_state['values']['subscriptions']['select']);
      if (isset($selected['new'])) {
        unset($selected['new']);
        unset($form_state['values']['subscriptions']['select']['new']);
      }
      if (!$selected) { //count($selected) < 1) {
        form_set_error('', t('You must select some subscriptions for that operation.'));
      }
      break;
  }
}

/**
 * Submit list of subscriptions
 */
function notifications_subscription_list_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : NULL;
  $updated = 0;
  switch ($op) {
    case t('Add'):
      $subscription = notifications_build_subscription($form_state['values']['template']);
      $subscription->add_fields($form_state['values']['parsed_fields']);
      notifications_subscription_list_form_values($form_state['values']['subscriptions'], $subscription, 'new');
      notifications_submit_subscription($subscription);
      break;
    case t('Update');
      $updated = 0;
      foreach ($form_state['values']['current_subscriptions'] as $key => $subscription) {
        if (notifications_subscription_list_form_values($form_state['values']['subscriptions'], $subscription)) {
          $subscription->save();
          $updated++;
        }
      }
      break;
    case t('Delete');
      $form_state['rebuild'] = TRUE;
      break;

  }
  if ($updated) {
    drupal_set_message(format_plural($updated, 'One subscription has been updated.', '@count subscriptions have been updated.'));
  }
}

/**
 * Extract values from subscription list form
 */
function notifications_subscription_list_form_values($form_values, $subscription, $key = NULL) {
  $key = isset($key) ? $key : $subscription->sid;
  $values = array();
  foreach (array('send_method', 'send_interval', 'status') as $field) {
    if (isset($form_values[$field][$key]) && (!isset($subscription->$field) || $subscription->$field != $form_values[$field][$key])) {
      // We just set the values that have changed and return them if any
      $subscription->$field = $values[$field] = $form_values[$field][$key];
    }         
  }
  return $values;
}
/**
 * Theme function for fields in form
 */
function theme_notifications_subscription_fields($elements) {
  $header = array(t('Field type'), t('Value'));
  $rows = array();
  foreach (element_children($elements['name']) as $key) {
    $rows[] = array(
      drupal_render($elements['name'][$key]),
      drupal_render($elements['value'][$key]),
    );
  }
  $output = theme('table', $header, $rows);
  $output .= drupal_render($elements);
  return $output;
}


/**
 * Generic table formatting for forms
 */
function theme_notifications_table_form($form) {
  $output = '';

  // Get table information from special form properties
  $index = !empty($form['#table_index']) ? $form['#table_index'] : $form['#table_fields'][0];
  $header = !empty($form['#table_header']) ? $form['#table_header'] : array();
 
  foreach (element_children($form[$index]) as $key) {
    $row = array();
    foreach ($form['#table_fields'] as $field) {
      $row[] = drupal_render($form[$field][$key]);
    }
    $rows[] = $row;
  }

  if ($rows) {
    $output .= theme('table', $header, $rows);
  } else {
    $output .= '<p>' . t('No elements') . '</p>';
  }
  $output .= drupal_render($form);
  return $output;
}
