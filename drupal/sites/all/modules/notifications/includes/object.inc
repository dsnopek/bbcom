<?php
/**
 * @file
 * Notifications object and fields
 * 
 * This is a library for handling objects and fields defined by notifications hooks
 */

/**
 * Invoke a callback for a field type. 
 * 
 * If the field has no callback it will default to the field object type callback
 * 
 * @param $type
 *   Subscription type
 * @param $name
 *   Callback name
 * @param $arg1, $arg2...
 *   Variable arguments
 */
function _notifications_field_callback() {
  $args = func_get_args();
  $type = array_shift($args);
  $name = array_shift($args);
  // We try first the field callback, then the field object type callback
  $info = notifications_subscription_fields($type);
  if (isset($info[$name])) {
    return _notifications_info_callback($info, $name, $args);
  }
  elseif (!empty($info['object_type']) && ($object_info = notifications_object_type($info['object_type'])) && isset($object_info[$name])) {
    return _notifications_info_callback($object_info, $name, $args);
  }
  else {
    return NULL;
  }
}

/**
 * Check access to an object for user account
 * 
 * @param $type
 *   Object type
 * @param $object
 *   Object or object id
 * @param $account
 *   User account to check access to the object
 */
function notifications_object_access($type, $object, $account) {
  $object = notifications_object_load($type, $object);
  // If object not properly loaded, always false
  if (!$object) {
    return FALSE;
  }
  elseif (($info = notifications_object_type($type)) && ($key = $info['key_field']) && isset($object->$key)) {
    $access = &messaging_static(__FUNCTION__);
    if (!isset($access[$type][$account->uid][$object->$key])) {
      if (isset($info['access callback'])) {
        $access[$type][$account->uid][$object->$key] = _notifications_object_callback($type, 'access callback', $object, $account);
      }
      elseif (isset($info['access'])) {
        $access[$type][$account->uid][$object->$key] = user_access($info['access'], $account);
      }
      else {
        // Not defined, so we allow user access
        $access[$type][$account->uid][$object->$key] = TRUE;
      }
    }
    return $access[$type][$account->uid][$object->$key];
  }
  // If not object information we cannot determine anything 
}

/**
 * Get subscription options for object, account. Only enabled subscription types
 */
function notifications_object_subscribe_options($type, $object, $account = NULL) {
  $account = $account ? $account : $GLOBALS['user'];
  $object = notifications_object_load($type, $object);
  $subscriptions = module_invoke_all('notifications_object_' . $type, 'subscriptions', $object, $account);
  // Filter out subscription types that are disabled
  foreach ($subscriptions as $key => $subs) {
    $type = is_object($subs) ? $subs->type : $subs['type'];
    if (!notifications_subscription_type_enabled($type)) {
      unset($subscriptions[$key]);
    }
  }
  return $subscriptions;
}

/**
 * Build subscribe / unsubscribe options for object
 */
function notifications_object_subscribe_links($type, $object, $account = NULL, $subscribe_options = array(), $unsubscribe_options = array()) {
  $links = array();
  if ($subscriptions = notifications_object_user_subscriptions($type, $object, $account)) {
    foreach ($subscriptions as $index => $subscription) {
      $options = $subscription->is_instance() ? $unsubscribe_options : $subscribe_options;
      if ($link = $subscription->build_link($options)) {
        $links['notifications_' . $index] = $link;
      }
    }
  }
  return $links;
}

/**
 * Get field conditions for this specific object
 */
function notifications_object_condition_fields($type, $object) {
  if ($object = notifications_object_load($type, $object)) {
    // As this does an array_merge_recursive() we get grouped field => array(value1, value2..)
    $fields = module_invoke_all('notifications_object_' .$type, 'conditions', $object);
    // Now we just need to filter out duplicate values
    foreach ($fields as $key => $value) {
      if (is_array($value)) {
        $fields[$key] = array_unique($value);
      }
    }
    return $fields;
  }
}

/**
 * Get list of possible and existing subscriptions for user/object
 * 
 * @param $type
 *   Subscription type to get options: 'user', 'node'
 * @param $object
 *   The object to subscribe. It may be $node or $user
 * @param $account
 *   User account to get options/subscriptions for
 * 
 * @return
 *   Array of subscription options
 *   The enabled ones will have a 'subscriptions' element loaded
 */
function notifications_object_user_subscriptions($type, $object, $account = NULL) {
  $cache = &messaging_static(__FUNCTION__);
  $account = $account ? $account : $GLOBALS['user'];
  $object = notifications_object_load($type, $object);
  
  // Get allowed subscription options for this account to this object
  $subscribe_options = notifications_object_subscribe_options($type, $object, $account);
  $allowed_options = array();
  foreach ($subscribe_options as $option) {
    // So far this is not a subscription but a subscription template
    $subscription = notifications_build_subscription($option);
    $type_key = $subscription->serialize_type();
    // If we have this type cached we don't search more
    if (!isset($cache[$account->uid][$type_key])) {
      if (notifications_user_allowed_subscription($account, $subscription)) {
        $subscription->set_account($account);
        // If anonymous user we don't search more because we cannot find by uid
        if ($account->uid) {
          $find = notifications_get_subscriptions(
            array('uid' => $account->uid, 'type' => $subscription->type),
            $subscription->get_conditions()
          );
          // Allowed subscription type, we store the subscription or the template
          if ($find) {
            $usersubs = current($find);
            $usersubs->name = $subscription->name;
            $subscription = $usersubs;
          }
        }
        $cache[$account->uid][$type_key] = $subscription;
      }
      else {
        // Not allowed subscription type for this user
        $cache[$account->uid][$type_key] = FALSE;
      }
    }
    if ($cache[$account->uid][$type_key]) {
      $allowed_options[] = $cache[$account->uid][$type_key];
    }
  }
  return $allowed_options;
}


/**
 * Get full info array for field type that has the property.
 * 
 * It will return the info array for the field it it has the property or the info for the object type
 * 
 * @param $type
 *   Field type
 * @param $property
 *   Property we are looking for
 *   
 * @return array()
 *   Info array from field or from object type
 */
function notifications_field_get_info($type, $property) {
  if ($info = notifications_field_type($type)) {
    if (isset($info[$property])) {
      return $info;
    }
    elseif (!empty($info['object_type']) && notifications_object_type($info['object_type'], $property)) {
      return notifications_object_type($info['object_type']);
    }
  }
}

/**
 * Format field type name
 */
function notifications_field_format_name($type) {
  $name = notifications_field_type($type, 'name');
  return $name ? $name : t('Unknown');
}

/**
 * Format field value
 * 
 * @param $type
 *   Field type
 * @param $value
 *   Field value
 * @param $html
 *   Whether to format the field as HTML (if FALSE will return plaintext format)
 * @param $subscription
 *   Subscription instance or template for which we want to format this field
 */
function notifications_field_format_value($type, $value, $html = TRUE, $subscription = NULL) {
  if ($format_info = notifications_field_get_info($type, 'format callback')) {
    $format_value = _notifications_info_callback($format_info, 'format callback', array($value, $html));
  }
  elseif ($options = notifications_field_subscription_options($type)) {
    // If not we try options callback, we can get the name from the array of options
    $format_value = isset($options[$value]) ? $options[$value] : t('Not available');
  }

  // If nothing got, we return the value
  if (!isset($format_value)) {
    $format_value = check_plain($value);
  }
  return $format_value;
}

/**
 * Collect submitted fields and parse new values
 */
function notifications_field_parse_submitted(&$form_state, $element_name = 'fields') {
  $fields = array();
  if (!empty($form_state['values'][$element_name]['type'])) {
    $field_values = &$form_state['values'][$element_name];
    foreach ($field_values['type'] as $key => $type) { 
      // If marked for deletion we just keep it there, don't return field
      if (empty($field_values['delete'][$key])) {
        // First collect all field values from the form   
        $field = array('type' => $type, 'value' => $field_values['value'][$key], 'edit' => $field_values['edit'][$key]);  
        // Complete field edit value, depending on field definition.
        if (empty($field_values['parsed'][$key])) {
          $value = notifications_field_real_value($type, $field['edit']);
          if (isset($value)) {
            $field['value'] = $value;
            $field_values['value'][$key] = $value;
            $field['parsed'] = TRUE;
            $field_values['parsed'][$key] = TRUE;
          }
          // Otherwise we let the field keep its value
        }
        // Add field to the list and mark as formatted so we can use this value for the form
        $fields[] = $field;
      }
    }
    
  }
  return $fields;
}

/**
 * Validate submitted field values and set the new ones as valid array of values
 */
function notifications_field_validate_submitted(&$form_state, $element_name = 'fields', $require_one = TRUE, $require_all = TRUE) {
  $checked_values = array();
  if ($field_values = notifications_field_parse_submitted($form_state, $element_name)) {
    foreach ($field_values as $key => $field) {
      $string_id = "$element_name][edit][$key";
      // We validate the field, type included
      if (notifications_field_valid_value($field['edit'])) {
        if (empty($field['parsed']) || !notifications_field_valid_value($field['value'], $field['type'])) {
          form_set_error($string_id, t('The value for this field is not valid.'));
          continue;
        }
      }
      elseif ($require_all) {
        form_set_error($string_id, t('You must set a value for this field.'));
        continue;
      }
      $checked_values[] = array('type' => $field['type'], 'value' => $field['value']);
    }
  }
  elseif ($require_one) {
    form_set_error(NULL, t('You must set at least one field for this subscription type.'));
  }
  return $checked_values;
}

/**
 * Convert field value from submission into its real value
 */
function notifications_field_real_value($type, $value) {  
  if (!notifications_field_valid_value($value)) {
    return NULL;
  }
  elseif ($info = notifications_field_get_info($type, 'value callback')) {
    // We have a value callback for field or object so use it
    return _notifications_info_callback($info, 'value callback', array($value));
  }
  else {
    // As we have nothing better, return the value itself
    return $value;
  }
}

/**
 * Get type information for field. For now its just subscription fields
 */
function notifications_field_type($type = NULL, $property = NULL) {
  return notifications_subscription_fields($type, $property);
}

/**
 * Get options for fields with options callback, may depend on subscription type
 * 
 * - First tries 'subscription_type options callback'
 * - If not found try generic 'options callback'
 */
function notifications_field_subscription_options($type, $subscription = NULL) {
  // First try specific options for this subscription type if any
  if ($subscription && ($info = notifications_field_get_info($type, "$subscription->type options callback"))) {
    return _notifications_info_callback($info, "$subscription->type options callback", array($subscription));
  }
  elseif ($info = notifications_field_get_info($type, 'options callback')) {
    return _notifications_info_callback($info, 'options callback', array($subscription));
  }
}

/**
 * Check if the field has a valid value
 */
function notifications_field_valid_value($value, $type = NULL) {
  // A numeric value of zero is possible too, that's why the is_numeric()
  if (!is_numeric($value) && empty($value)) {
    // The field has no value at all, no go
    return FALSE;
  }
  elseif ($type) {
    // We want aditional field type validation
    switch (notifications_field_type($type, 'type')) {
      case 'int':
        // @todo Better integer validation, is_int not working for strings
        return is_numeric($value);
      case 'float':
        return is_numeric($value);
      case 'string':
      default:
        return is_string($value);
    }
  }
  else {
    return TRUE;
  }
}

/**
 * Build a form element to edit a field
 * 
 * @param $type
 *   Field type
 * @param $value
 *   Field value
 * @param $subscription
 *   Subscription template or instance we are adding this field to
 */
function notifications_field_form_element($type, $value, $subscription = NULL, $title = FALSE, $required = FALSE, $size = 40) {
  $subscription_type = $subscription ? $subscription->type : NULL;
  $field_info = notifications_subscription_fields($type);
  $object_info = isset($field_info['object_type']) ? notifications_object_type($field_info['object_type']) : array();
  $merged_info = $field_info + $object_info;

  if (isset($merged_info['options callback'])) {
    $element['#type'] = 'select';
    $element['#options'] = notifications_field_subscription_options($type, $subscription);
  }
  elseif (!empty($merged_info['autocomplete path'])) {
    $element['#type'] = 'textfield';
    $element['#size'] = $size;
    $element['#autocomplete_path'] = $merged_info['autocomplete path'];
    if ($value) {     
      if (!empty($merged_info['autocomplete callback'])) {
        $value = _notifications_field_callback($type, 'autocomplete callback', $value, $subscription);
      }
      elseif (!empty($merged_info['format callback']))  {
        $value = _notifications_field_callback($type, 'format callback', $value, FALSE);
      }      
    }
  }
  else {
    $element['#type'] = 'textfield';
    if ($value) {
      $value = check_plain($value);
    }
  }
  if ($value) {
    $element['#default_value'] = $value;
  }
  if ($title) {
    $element['#title'] = notifications_field_format_name($type);
  }
  $element['#required'] = $required;
  return $element;
}

/**
 * Get objects to which we can subscribe on current page
 */
function notifications_object_page_objects() {
  $objects = &messaging_static(__FUNCTION__);
  if (!isset($objects)) {
    $objects = messaging_module_invoke_all('notifications_subscription', 'page objects');
  }
  return $objects;
}

/**
 * Build subscriptions for current user to an array of objects
 */
function notifications_object_page_subscriptions($objects) {
  global $user;
  $subscriptions = array();
  foreach ($objects as $type => $object) {
    if ($more = notifications_object_user_subscriptions($type, $object, $user)) {
      $subscriptions = array_merge($subscriptions, $more);
    }
  }
  return $subscriptions;
}

/**
 * Form for object (node, user, term...) subscriptions
 *
 * @param $subscriptions
 *   Array of subscription options
 */
function notifications_object_options_form($form_state, $subscriptions) {
  $form['subscriptions'] =  notifications_object_options_fieldset($subscriptions, FALSE);  
  $form['submit'] = array('#type' => 'submit', '#value' => t('Update'));

  // If full form, redirect so the full page which may have subscription links is updated
  $form['#redirect'] = $_GET['q'];
  // Add always submit callback because the form may have a different name
  $form['#submit'][] = 'notifications_subscriptions_options_form_submit';
  return $form;
}

/**
 * Process submission
 */
function notifications_object_options_form_submit($form, $form_state) {
  $enabled = $disabled = 0;
  // We may have also send method and destination in this form, like on forms from anonymous users
  $send_method = isset($form_state['values']['send_method']) ? $form_state['values']['send_method'] : NULL;
  $destination = isset($form_state['values']['destination']) ? $form_state['values']['destination'] : NULL;
  foreach ($form_state['values']['subscriptions']['options'] as $index => $value) {
    $subscription = $form_state['values']['subscriptions']['params'][$index];   
    if ($value && !$subscription->is_instance()) {
      // We checked a disabled subscription
      if ($send_method) {
        $subscription->send_method = $send_method;
      }
      if ($destination) {
        $subscription->set_destination($destination);
      }
      notifications_save_subscription($subscription);
      $enabled++;
    }
    elseif (!$value && $subscription->is_instance()) {
      // we unchecked an enabled subscription
      notifications_subscription_delete($subscription->sid);
      $disabled++;
    }
  }

  if ($enabled) {
    drupal_set_message(format_plural($enabled, 'A subscription has been created', '@count subscriptions have been created'));
  }
  if ($disabled) {
    drupal_set_message(format_plural($disabled, 'A subscription has been deleted', '@count subscriptions have been deleted'));
  }  
}

/**
 * Subform with subscription options so it can be reused for a fieldset on a bigger form
 * 
 * @param $subscriptions
 *   List of subscription objects
 * @param $buttons
 *   Whether to add buttons to the fieldset
 */
function notifications_object_options_subform($subscriptions, $buttons = TRUE) {
  $form['subscriptions'] = notifications_object_options_fieldset($subscriptions, TRUE);
  $form['subscriptions'] += array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,  
  );
  if ($buttons) {
    $form['subscriptions']['submit'] = array('#type' => 'submit', '#value' => t('Update'));
    // If full form, redirect so the full page which may have subscription links is updated
    $form['#redirect'] = $_GET['q'];
  }
  $form['#submit'][] = 'notifications_subscriptions_options_form_submit';
  return $form;
}

/**
 * Build fieldset with subscription options
 */
function notifications_object_options_fieldset($subscriptions, $title = FALSE) {
  $elements = array(
    '#tree' => TRUE,
  );
  // Process all options building the array of indexed params for each
  $options = $params = $defaults = array();
  $index = 1; // Index to map checkboxes to subscriptions
  $number = 0; // Number of active subscriptions
  foreach ($subscriptions as $subscription) {
    $options[$index] = $subscription->get_name();
    $params[$index] = $subscription;
    // Check wether user is subscribed
    if ($subscription->is_instance()) {
      $defaults[] = $index;
      $number++;
    }
    $index++;    
  }
  $elements['params'] = array('#type' => 'value', '#value' => $params);
  $elements['options'] = array(
    '#type' => 'checkboxes',
    '#default_value' => $defaults,
    '#options' => $options,
  );
  if ($title) {
    $elements['#title'] = t('Subscriptions (@number)', array('@number' => $number));
  }
  return $elements;
}