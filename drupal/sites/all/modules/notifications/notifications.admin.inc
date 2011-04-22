<?php

// Library for managing subscriptions, pages and forms
require_once drupal_get_path('module', 'notifications') . '/notifications.manage.inc';

/**
 * Implementation of hook_help()
 * 
 * This file will be included only for Notifications admin pages
 */
function notifications_help($path, $arg) {
  $pages = array(
    '@admin-events' => url( 'admin/messaging/notifications/events'),
    '@admin-intervals' => url( 'admin/messaging/notifications/interval'),
    '@admin-subscriptions' => url( 'admin/messaging/notifications/subscriptions'),
    '@admin-messaging' => url('admin/messaging/settings'),
    '@admin-messaging' => url('admin/messaging/settings/method'),
  );
  switch ($path) {
    case 'admin/messaging/notifications':
      // Try to clarify some concepts, tell the long story short.
      $output = '<p>' . t('Users can subscribe to different objects (nodes, users, tags) by creating <strong><a href="@admin-subscriptions">Subscriptions</a></strong> for that objects. The subscription types available and the options to display will depend on additional modules enabled.', $pages) . '</p>';
      $output .= '<p>' . t('When an <a href="@admin-events">Event</a> happens (node update, comment) and it matches an existing subscription, that triggers a <strong>Notification</strong> which is a <strong>Message</strong> that will be sent to the user through one of the available <a href="@admin-messaging-methods">Sending Methods</a>.', $pages) . '</p>';
      $output .= '<p>' . t('These <strong>Notifications</strong> can be sent right away or queued to be processed on <i>cron</i>. Optionally Notifications can be digested and sent out every some <a href="@admin-intervals">time interval</a>.', $pages) . '</p>';
      return $output;
    case 'admin/messaging/notifications/subscriptions':
      $output = '<p>' . t('On this page you can define which subscription types are enabled or disabled. <strong>Disabled subscription types will not be available for users</strong>.') . '</p>';
      $output .= '<p>' . t('<strong>Existing subscriptions will be updated accordingly</strong>. They\'ll be disabled when their type is disabled or re-enabled when they were disabled and the type is enabled.') . '</p>';
      return $output;
      
  }
}

/**
 * Menu callback: subscriptions administration.
 */
function notifications_admin_manage_subscriptions($form_state) {
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    return notifications_multiple_delete_confirm($form_state, array_filter($form_state['values']['subscriptions']));
  }
  $form = notifications_subscriptions_filter_form(TRUE);

  $form['#theme'] = 'notifications_subscriptions_filter_form';
  $form['admin']  = notifications_manage_subscriptions_form();

  return $form;
}

/**
 * Current subscriptions page
 */
function notifications_admin_status_page() {
  $output = '';
  
  // Subscriptions summary by type
  $header = array(t('Type'), t('Number'));
  $result = db_query("SELECT type, count(*) AS count FROM {notifications} GROUP BY type");
  $count = 0;
  $types = notifications_subscription_types();
  $rows = array();
  while ($stype = db_fetch_object($result)) {
    $rows[] = array(
      !empty($types[$stype->type]['title']) ? $types[$stype->type]['title'] : '<strong>' . $stype->type . '</strong>' , 
      $stype->count);
    $count += $stype->count;
  }
  $summary = theme('table', $header, $rows);
  $summary .= t('Total: %number', array('%number' => $count));
  $output .= theme('box', t('Subscriptions by type'), $summary);
  
  // Summary by sending method
  $header = array(t('Method'), t('Number'));
  $result = db_query("SELECT send_method, count(*) AS count FROM {notifications} GROUP BY send_method");
  $rows = array();
  while ($subs = db_fetch_object($result)) {
    $name = messaging_method_info($subs->send_method, 'title');
    $rows[] = array(
      $name ? $name : '<strong>' . $subs->send_method . '</strong>' , 
      $subs->count);
  }
  $summary = theme('table', $header, $rows);
  $output .= theme('box', t('Subscriptions by send method'), $summary); 

  return $output;  
}

/**
 * Admin settings
 */
function notifications_settings_form() {
  $form['general'] = array(
      '#type' => 'fieldset',
      '#title' => t('General settings'),
      '#weight' => -10,
  );
  $form['general']['notifications_sendself'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Notify poster of own posts'),
    '#default_value' => variable_get('notifications_sendself', 1),
    '#description'   => t("Notifies a node poster about their own posts.  Useful principally during testing.  Default is OFF."),
  );
  $form['general']['notifications_send_immediate'] = array(
    '#title' => t('Immediate sending'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('notifications_send_immediate', 0),
    '#description' => t('Notifications are usually queued to be sent on cron process later. Checking this option will cause immediate notifications to be sent right away, instead of being queued. This will produce more timely notifications for sites with a small number of users. Not recommended for sites with a large number of users. This may not work for AJAX forms and notifications will be queued anyway.'),
  );
  $form['general']['notifications_sender'] = array(
    '#title' => t('Notifications Sender'),
    '#type' => 'radios',
    '#options' => array(
      t('No one (All notifications will appear as coming from the web site)'),
      t('User name, site data (Only the user name will be used)'),
      t('Full user data (User name and available user information)'),
    ),
    '#default_value' => variable_get('notifications_sender', 0),
    '#description' => t('Use the site information as the sender for notification messages or use account data from the user causing the event. WARNING: Selecting the last option (Full user data) may disclose private information to subscribers like the user e-mail address.'),
  );

  // Default options
  $form['defaults'] = array(
      '#type' => 'fieldset',
      '#title' => t('Default settings'),
  );
  $form['defaults']['notifications_default_send_interval'] = array(
    '#type' => 'select',
    '#title' => t('Default send interval'),
    '#options' => _notifications_send_intervals(),
    '#default_value' => variable_get('notifications_default_send_interval', 0),
  );

  return system_settings_form($form);
}

/**
 * Send intervals administration
 */
function notifications_send_intervals_form() {
  // Collect information about digesting methods and print out some help
  // Collect information about building methods and print out some help
  $form['build'] = array(
    '#title' => t('Build formats'),
    '#type' => 'fieldset',
    '#description' => t('These are the building formats available for each interval.')
  );
  foreach (notifications_build_method() as $type => $method) {
    $build_methods[$type] = $method['name'];
    $rows[] = array($method['name'], $method['description']);    
  }
  $form['build']['info'] = array('#value' => theme('table', array(), $rows));
  
  // Build options. Need some complex formatting.
  $units = array(
    60 => t('Minutes'),
    60*60 => t('Hours'), 
    24*60*60 => t('Days'),
  );
  $form['intervals'] = array(
    '#type' => 'fieldset',
    '#title' => t('Send intervals'),
    '#tree' => TRUE,
    //'#theme' => 'notifications_send_intervals',
    '#description' => t('To delete an interval, set the time unit and the name empty. If the time is negative, notifications will never be sent.'),
  );
  $intervals = _notifications_send_intervals() + array('new1' => '', 'new2' => '');
  // We need to use different indexes (not time value) as negative ones won't work
  $index = 0;
  $index_name = $time_index = array();
  foreach ($intervals as $time => $name) {
    // Store index => name mapping
    $index_options[$index] = '';
    $time_index[$time] = $index;
    // Calculate value and unit
    $current = $number = 0; 
    if (!is_numeric($time)) {
      $number = $unit = '';
    }
    elseif ($time >= 0) {
      foreach (array_reverse(array_keys($units)) as $unit) {
        if ($time % $unit == 0) {
          $current = $unit;
          $number = $time / $unit;
          break;
        }
      }
    }
    else {
      // May be -1 for 'Never'
      $number = $time;
      $unit = '';
    }
    $form['intervals'][$index]['time'] = array(
      '#default_value' => $number,
      '#type' => 'textfield',
      '#size' => 2,
    );
    $form['intervals'][$index]['unit'] = array(
      '#type' => 'select',
      '#options' => array('' => '') + $units,
      '#default_value' => $unit,
    );
    $form['intervals'][$index]['name'] = array(
      '#type' => 'textfield',
      '#default_value' => $name,
      '#size' => 40,
    );
    $build = notifications_build_method($time);
    $form['intervals'][$index]['build'] = array(
      '#type' => 'select',
      '#disabled' => ($number < 0),
      '#default_value' => $build ? $build['type'] : 'simple',
      '#options' => $build_methods,
    );
    // Store also index -> time mapping
    $form['intervals'][$index]['key'] = array('#type' => 'value', '#value' => $time);
    $index++;
  }
  $form['default'] = array('#type' => 'radios',
    '#options' => $index_options,
    '#default_value' => $time_index[variable_get('notifications_default_send_interval', 0)],
  );
  // New row
  $form['submit'] = array('#type' => 'submit', '#value' => t('Update'));
  $form['reset'] = array('#type' => 'submit', '#value' => t('Reset to defaults'));

  return $form;
}

/**
 * Form validate for time intervals
 */
function notifications_send_intervals_form_validate($form, &$form_state) {
  // Check the default is a valid one, not one to be deleted
  $default = $form_state['values']['default'];
  if (empty($form_state['values']['intervals'][$default]['unit']) || empty($form_state['values']['intervals'][$default]['name'])) {
    form_set_error('default', t('Invalid default send interval.'));
  }
}

/**
 * Form submit for time intervals
 */
function notifications_send_intervals_form_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  if ($form_values['op'] == t('Reset to defaults')) {
    variable_del('notifications_send_intervals');
    variable_del('notifications_digest_methods');
    variable_del('notifications_default_send_interval');
  }
  else {
    $intervals = $digest = array();
    foreach ($form_values['intervals'] as $index => $values) {
      if (is_numeric($values['time']) && $values['name']) {
        $unit = $values['unit'] ? (int)$values['unit'] : 1;
        $time = (int)$values['time'] * $unit;
        $intervals[$time] = $values['name'];
        $build_methods[$time] = $values['build'];
        if ($index == $form_values['default']) {
          variable_set('notifications_default_send_interval', $time);
        }
      }
    }
    ksort($intervals);
    variable_set('notifications_send_intervals', $intervals);
    variable_set('notifications_digest_methods', $build_methods);
  }
  drupal_set_message(t('The time intervals for your subscriptions have been updated'));
  // Update orphaned notifications with invalid send interval
  $valid = array_keys($intervals);
  $params = array_merge(array(variable_get('notifications_default_send_interval', 0)), $valid);
  db_query('UPDATE {notifications} SET send_interval = %d WHERE send_interval NOT IN (' . db_placeholders($valid) . ')', $params);
  if ($updated = db_affected_rows()) {
    drupal_set_message(format_plural($updated, 'Updated a subscription with invalid interval.', 'Updated @count subscriptions with invalid intervals.'));
  }
  // Refresh strings after update if translation enabled
  if (module_exists('i18nstrings')) {
    notifications_locale_refresh();
  }
}

/**
 * Subscription settings
 */
function notifications_admin_subscriptions_settings() {
  // Enabled subscription types
  $types = array_map('notifications_format_title_description', notifications_subscription_types());
  $enabled = notifications_subscription_type_enabled();
  
  $form['subscriptions'] = array(
    '#title' => t('Enabled Subscription types'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
  );
  $form['subscriptions']['notifications_subscription_types'] = array(
    '#type' => 'checkboxes',
    '#options' => $types,
    '#default_value' => $enabled,
    '#description' => t('Check the available subscription types that will be enabled globally'),
  );
  // Link options. These were UI settings that will become global now, so other modules can use them without Notifications UI
  $form['links'] = array(
    '#title' => t('Subscribe / unsubscribe links options'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
  );
  $options = array(
    t('<strong>Confirmation form</strong>. Links take the user through a confirmation form with some more options.'),
    t('<strong>Direct operation</strong>. Links create/delete a subscription without confirmation using default settings.'),
  );
  $form['links']['notifications_ui_subscribe_links'] = array(
    '#title' => t('Subscribe links'),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('notifications_ui_subscribe_links', 0),
  );
  $form['links']['notifications_ui_unsubscribe_links'] = array(
    '#title' => t('Unsubscribe links'),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('notifications_ui_unsubscribe_links', 0),
  );

  $form = system_settings_form($form);
  // We have our own processing to do after system settings
  $form['#submit'][] = 'notifications_admin_subscriptions_settings_submit';

  return $form;
}

/**
 * Subscription settings submit, disable all subscriptions not allowed
 */
function notifications_admin_subscriptions_settings_submit($form, &$form_state) {
  // Submission may have been Save or Reset to defaults, so we cannot really trust submitted values
  // Instead we do a cache refresh and re-check enabled / disabled types
  messaging_static_reset('notifications_subscription_type_enabled');
  $types = notifications_subscription_type_enabled();
  $sql_update = 'UPDATE {notifications} SET status = %d WHERE status = %d';
  $enabled = $disabled = 0;

  if ($types) {
    // Enable subscriptions of these types that were disabled before
    $placeholders = db_placeholders($types, 'varchar');
    $sql_where = ' AND type IN (' . $placeholders . ')';
    $params = array_merge(array(Notifications_Subscription::STATUS_ACTIVE, Notifications_Subscription::STATUS_DISABLED), $types);
    db_query($sql_update . $sql_where, $params);
    $enabled = db_affected_rows();

    $params = array_merge(array(Notifications_Subscription::STATUS_DISABLED, Notifications_Subscription::STATUS_ACTIVE), $types);
    $sql_where = ' AND type NOT IN (' . $placeholders . ')';
  }
  else {
    $params = array(Notifications_Subscription::STATUS_DISABLED, Notifications_Subscription::STATUS_ACTIVE);
    $sql_where = '';
  }
  db_query($sql_update . $sql_where, $params);
  $disabled = db_affected_rows();
  
  if ($enabled) {
    drupal_set_message(t('@count existing subscriptions have been enabled.', array('@count' => $enabled)), 'warning');
  }
  if ($disabled) {
    drupal_set_message(t('@count existing subscriptions have been disabled.', array('@count' => $disabled)), 'warning');
    // Delete rows from queue and do some clean up.
    notifications_queue()->queue_clean(FALSE);
  }
}

/**
 * Build a table with send intervals
 */
function theme_notifications_send_intervals_form($element) {
  $output = ''; //drupal_render($element['digest']);
  $header = array(array('data' => t('Time'), 'colspan' => 2), t('Display name'), t('Format'), t('Default'));
  foreach (element_children($element['intervals']) as $key) {
    $rows[] = array(
      drupal_render($element['intervals'][$key]['time']),
      drupal_render($element['intervals'][$key]['unit']),
      drupal_render($element['intervals'][$key]['name']),
      drupal_render($element['intervals'][$key]['build']),
      drupal_render($element['default'][$key]),
    );  
  }
  $element['intervals']['#value'] = theme('table', $header, $rows);
  //$output .= theme('table', $header, $rows);
  $output .= drupal_render($element);
  return $output;
}

/**
 * Event configuration administration
 * 
 * Will allow to define which events trigger a notification and which ones not
 */
function notifications_admin_events_form() {
  $form = array();
  // Compile array from plug-ins and settings
  if ($events = notifications_event_types()) {
    $form['notifications_event_enabled']['#tree'] = TRUE;
    $form['notifications_event_template']['#tree'] = TRUE;
    // Group event types when they have a parent
    $tree = notifications_admin_events_build_tree($events);
    notifications_admin_events_tree_form($tree, $form, 0);
    $form = system_settings_form($form);
    $form['#theme'] = 'notifications_table_form';
    $form['#table_fields'] = array('name', 'notifications_event_enabled', 'notifications_event_template');
    $form['#table_header'] = array(t('Event type'), t('Enabled'), t('Template'));
    return $form;
  } else {
    $form['warning'] = array('#value' => t('You need to enable some plug-ins to provide notifications events.'));
    return $form;
  }  
}

/**
 * Build event tree
 */
function notifications_admin_events_build_tree($events) {
  $tree = array();
  foreach ($events as $key => &$event) {
    if (empty($event['parent'])) {
      $tree[$key] = &$event;
    }
    else {
      $events[$event['parent']]['children'][$key] = &$event;
    }
  }
  return $tree;  
}

/**
 * Build tree of event types
 */
function notifications_admin_events_tree_form($tree, &$form, $depth = 0) {
  foreach ($tree as $key => $event) {
    $form['name'][$key] = array('#value' => theme('indentation', $depth) . $event['description']);
    $form['notifications_event_enabled'][$key] = array(
      '#type' => 'checkbox',
      '#default_value' => notifications_event_enabled($key),
    );
    $form['notifications_event_template'][$key] = array(
      '#type' => 'select',
      '#default_value' => Notifications_Event::template_map($key),
      '#options' => notifications_template_list(),
    );
    if (!empty($event['children'])) {
      notifications_admin_events_tree_form($event['children'], $form, $depth +1);
    }
  }  
}

/**
 * Plain list of template names
 */
function notifications_template_list() {
  $list = &messaging_static(__FUNCTION__);
  if (!isset($list)) {
    notifications_include('templates.inc');
    foreach (notifications_get_templates() as $key => $template) {
      $list[$key] = $template['name'];
    }
  }
  return $list;
}
/**
 * Theme subscriptions list
 */
function theme_notifications_form_table($element) {
  $output = '';
  if ($fields = element_children($element)) {
    $header = $element['#header'];
    $rows = array();
    // The first element determines the number of columns
    foreach (element_children($element[$fields[key($fields)]]) as $index) {
      $row = array();
      foreach ($fields as $key) {
        $row[] = isset($element[$key][$index]) ? drupal_render($element[$key][$index]) : '';
      }
      $rows[] = $row;
    }
    $output .= theme('table', $header, $rows);
  }
  $output .= drupal_render($element);
  return $output;
}
