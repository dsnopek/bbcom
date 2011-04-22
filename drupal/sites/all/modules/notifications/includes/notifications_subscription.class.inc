<?php
/**
 * @file
 * Drupal Notifications Framework - Default class file
 */

/**
 * Message destination class
 */
class Notifications_Subscription extends Messaging_Object {
  // Database properties
  const DB_TABLE = 'notifications';
  const DB_KEY = 'sid';
  // Blocked subscriptions, for blocked users
  const STATUS_BLOCKED = 0;
  // Enabled ones, will produce notifications
  const STATUS_ACTIVE = 1;
  // Temporarily disabled ones, maybe user on holidays
  const STATUS_INACTIVE = 2;
  // Disabled because the subscription type is disabled
  const STATUS_DISABLED = -1;

  // Format as plaintext. Note it evaluates to false.
  const FORMAT_PLAIN = 0;
  // Format as html. Note it evaluates to true
  const FORMAT_HTML = 1;
  // Format inline, as a string of csv
  const FORMAT_INLINE = 2;
  // Format as HTML table (4 +1)
  const FORMAT_TABLE = 5;
  // Format as item list (8 + 2(inline) + 1 (html))
  const FORMAT_LIST = 10;
  
  // This class inherits some properties from Messaging_Object
  // public $uid;
  // public $language;
  
  // Unique subscription id
  public $sid = 0;
  // Destination id
  public $mdid;
  // Subscription type
  public $type;
  // Event that triggers this subscription
  public $event_type;
  // Number of conditions that must be met
  public $conditions;
  // Values to pass on to the queue for composition
  public $send_interval;
  public $send_method;
  public $cron = 1;
  public $module = 'notifications';
  // Subscription status, defaults to active
  public $status = 1;
  // The address for the destination (Unused, just for logging. To be obsoleted)
  public $destination = '';
  // Subscription fields
  protected $fields;

  // Temporary variables, not saved to the DB
  protected $destination_object;
  protected $format;
  protected $objects;
  // Name for this subscription that will be quite dependent on context
  public $name;
  // Mark if incomplete loading of objects
  public $incomplete;
  // Mark when editable
  public $editable;
  // Temporary error message, to display when validation fails
  public $error_message;
  
  /**
   * Class constructor
   */
  function __construct($object = NULL) {
    $this->status = self::STATUS_ACTIVE;
    if ($object) {
      $properties = (array)$object;
      foreach ($properties as $key => $value) {
        // Set old fields format (array of field => value)
        if ($key == 'fields' || $key == 'destination') {
          $this->__set($key, $value);
        }
        else {
          $this->$key = $value;
        }
      }
    }
    // Default values for new objects
    if (empty($this->sid)) {
      $this->created = time();
      // Set some properties from subscription type
      if (!empty($this->type) && ($defaults = notifications_subscription_types($this->type))) {
        foreach (array('event_type', 'module', 'cron', 'conditions') as $field) {
          $value = notifications_subscription_types($this->type, $field);
          if (!isset($properties[$field]) && isset($defaults[$field])) {
            $this->$field =  $defaults[$field];
          }          
        }
      }

    }
  }
  /**
   * Build for a subscription type
   * 
   * @param $template
   *   Template object or subscription type
   */
  public static function build($template, $default_class = 'Notifications_Subscription') {
    if (is_array($template)) {
      $type = NULL;
      $template = (object)$template;
    }
    if (is_object($template)) {
      $object = $template;
      $type = $template->type;
    } 
    else {
      $type = $template;
      $object = array('type' => $type);
    }
    // The type may define its own class
    if ($type && ($class = notifications_subscription_types($type, 'class'))) {
      return new $class($object);
    }
    else {
      return new $default_class($object);
    }
  }

  /**
  * Validate form submission
  */
  public static function validate_submission(&$form_state) {
    // The object may be already created ir it may be an array so we use this function that handles that
    $subscription = notifications_build_subscription($form_state['values']['subscription']);
    // There may be optional fields to add
    if (!empty($form_state['values']['subscription_fields'])) {
      foreach ($form_state['values']['subscription_fields'] as $field) {
        if (isset($form_state['values'][$field])) {
          $field_value = $form_state['values'][$field];
          switch ($field) {
            case 'fields':
              notifications_include('object.inc');
              foreach ($field_value as $key => $field_values) {
                if ($value = notifications_field_real_value($field_values['type'], $field_values['edit'])) {
                  $form_state['values']['parsed_fields'][$key] = array('type' => $field_values['type'], 'value' => $value);
                }
                else {
                  form_set_error("$field]['edit", t('Invalid value for this field.'));
                  break;
                }
              }
              break;
            case 'send_method':
              // There may be other destination validation running that sets the destination object before
              if (empty($form_state['values']['destination']) && (!$subscription->mdid || $subscription->send_method != $field_value)) {
                if ($subscription->validate_destination($field_value)) {
                  $form_state['values']['subscription_fields'][] = 'destination';
                }
                else {
                  form_set_error('send_method', t('Cannot create a valid destination.'));
                }
              }
              break;
          }   
        }
      }
    }
  }

  /**
   * Build from form submission
   */
  public static function build_submission(&$form_state) {
    // The object may be already created ir it may be an array so we use this function that handles that
    $subscription = notifications_build_subscription($form_state['values']['subscription']);
    // There may be optional fields to add
    if (!empty($form_state['values']['subscription_fields'])) {
      $subscription->set_properties($form_state['values'], array_unique($form_state['values']['subscription_fields']));
    }
    return $subscription;
  }
  
  /**
   * Save fields from form submission
   */
  public function set_properties($values, $fields) {
    foreach ($fields as $key) {
      if (isset($values[$key])) {
        switch ($key) {
          case 'fields':
            if (!empty($values['parsed_fields'])) {
              $this->set_fields($values['parsed_fields']);
            }
            else {
              $this->set_fields($values['fields']);
            }
            break;
          case 'destination':
            $this->set_destination($values[$key]);
            break;
          default:
            $this->$key = $values[$key];
            break;         
        }
      }
    }
  }
  
  /**
   * Produce fieldset to edit field values
   */
  function fields_subform() {
    $elements = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#description' => $this->get_description(),
    );
    foreach ($this->get_type_fields() as $index => $field) {
      $elements[$index]['type'] = array('#type' => 'value', '#value' => $field->field);
      $elements[$index]['value'] = array('#type' => 'value', '#value' => $field->value);
      $elements[$index]['edit'] = $this->field_element($field->field, $field->value, TRUE, TRUE);
    }
    return $elements;
  }
  
  /**
   * Build subscribe /unsubscribe link
   */
  function build_link($options = array()) {
    if ($this->is_instance()) {
      // Unsubscribe link
      $title = t('Unsubscribe from: @name', array('@name' => $this->get_name()));
      $options += array('sid' => $this->sid);
      $props = notifications_subscription_get_link('unsubscribe', $this, $options);
    }
    else {
      // Subscribe link
      $title = t('Subscribe to: @name', array('@name' => $this->get_name()));
      $options += array('type' => $this->type, 'fields' => $this->get_conditions());
      $props = notifications_subscription_get_link('subscribe', $this, $options);
    }
    return $props ? array(
      'title' => $title,
      'html' => TRUE,
      'href' => $props['href'],
    ) + $props['options'] : NULL;
  }

  /**
   * Set user account as the owner of this subscription and take care of defaults for this account.
   * 
   * @param $account
   *   User account or user uid
   */
  function set_account($account) {
    $account = messaging_user_object($account);
    $this->set_user($account);
    if (!isset($this->send_interval)) {
      $this->send_interval = notifications_user_setting('send_interval', $account, 0);
    }
    if (!isset($this->send_method)) {
      $this->send_method = notifications_user_setting('send_method', $account);
    }
    if (empty($this->language)) {
      $this->set_language(); 
    }
    // Return TRUE if all parameters set
    return isset($this->uid) && isset($this->send_interval) && isset($this->send_method);
  }

  /**
   * Get user account
   */
  function get_account() {
    return $this->get_user();
  }
  /**
   * Load from db
   * 
   * @param $sid
   *   Subscription id 
   * @param $full
   *   Whether to load all fields in the same operation
   */
  public static function load($sid) {
    return self::object_load(self::DB_TABLE, self::DB_KEY, $sid, 'Notifications_Subscription');
  }
  /**
   * Load condition fields from db
   */
  function load_fields() {
    $this->fields = array();
    if (!empty($this->sid)) {
      $result = db_query("SELECT * FROM {notifications_fields} WHERE sid = %d", $this->sid);
      while ($field = db_fetch_object($result)) {
        $this->set_field($field);
      }      
    }
  }
  /**
   * Save to db
   */
  function insert() {
    $result = parent::insert();
    $this->save_fields(FALSE);
    return $result; 
  }
  /**
   * Update db
   */
  function update() {
    $this->save_fields(TRUE);
    return parent::update();
  }
  /**
   * Delete from db
   */
  function delete() {
    if ($this->is_instance()) {
      module_invoke_all('notifications_subscription', 'delete', $this);
      db_query("DELETE FROM {notifications_fields} WHERE sid = %d", $this->sid);
      return parent::delete();
    }
  }
  /**
   * Check subscription user account and related parameters
   */
  function check_account() {
    if (!isset($this->uid)) {
      return FALSE;
    }
    elseif (!isset($this->send_method) || !isset($this->send_interval)) {
      return $this->set_account($this->get_user());
    }
    else {
      return TRUE;
    }
  }
  
  /**
   * Check destination or create it if new
   */
  function check_destination() {
    if (empty($this->mdid)) {
      return $this->create_destination();
    }
    elseif ($destination = $this->get_destination()) {
      // We may need to create a new destination if this address is not valid for method has changed
      if (!empty($this->send_method) && messaging_method_info($this->send_method, 'address_type') != $destination->type) {
        return $this->create_destination();
      }
      else {
        return $destination;
      }
    }
    else {
      // Last try, figure out a destination
      return $this->create_destination();
    }
  }
  /**
   * Check permission for user account
   */
  function check_access($account = NULL) {
    $account = $account ? $account : $this->get_account();
    return notifications_user_allowed_subscription($account, $this);
  }
  /**
   * Create destination for this subscription
   */
  function create_destination($method = NULL, $address = NULL) {
    return $this->validate_destination($method, $address, TRUE);
  }
  /**
   * Validate destination for this subscription
   */
  function validate_destination($method = NULL, $address = NULL, $create = FALSE) {
    $account = $this->get_user();
    if ($method && $address) {
      if ($create) {
        $destination = Messaging_Destination::create_method($method, $address, $account->uid);
        $this->set_destination($destination);
        return $destination;
      }
      else {
        // Just validate
        return Messaging_Destination::validate_method($method, $address, $account->uid);
      }
    }
    elseif ($account->uid) {
      // For registered users we have other ways to create a destination
      if ($method) {
        if ($address = messaging_user_destination($account, $method)) {
          // We have a method and address for this user account, fine
          return $this->validate_destination($method, $address, $create);
        }
      }
      else {
        // We still can have a send method or get it from account
        $method = !empty($this->send_method) ? $this->send_method  : messaging_method_default($account);
        if ($method) {
          return $this->validate_destination($method, NULL, $create);
        }
      }
    }    
  }

  /**
   * Save condition fields to db
   * 
   * @param $update
   *   Whether this is an old subscription being created
   */
  function save_fields($update = FALSE) {
    if (isset($this->fields)) {
      if ($update) {
        db_query("DELETE FROM {notifications_fields} WHERE sid = %d", $this->sid);
      }
      foreach ($this->fields as $field) {
        db_query("INSERT INTO {notifications_fields} (sid, field, value, intval) VALUES(%d, '%s', '%s', %d)", $this->sid, $field->field, $field->value, (int)$field->value);
      }
    }
  }
  /**
   * Add a new condition, update counter
   */
  function add_condition($name, $value) {
    $this->add_field($name, $value);
    $this->conditions++;
  }
  /**
   * Get fields as array of field => value pairs
   * 
   * Duplicate fields are returned as field => array(value1, value2...)
   * 
   * @param $type
   *   Optional to just return the values for some field type
   */
  function get_conditions($type = NULL) {
    $list = array();
    foreach ($this->get_fields() as $field) {
      // We cannot simply use isset() because the value may be NULL
      if (!array_key_exists($field->field, $list)) {
        $list[$field->field] = $field->value;
      }
      elseif (is_array($list[$field->field])) {
        $list[$field->field][] = $field->value;
      }
      else {
        $list[$field->field] = array($list[$field->field], $field->value);
      }
    }
    if (isset($type)) {
      return isset($list[$type]) ? $list[$type] : NULL;
    }
    else {
      return $list;
    }
  }
  /**
   * Check whether we have a given condition
   */
  function has_condition($type, $value) {
    $conds = $this->get_conditions($type);
    return isset($conds) && (is_array($conds) && in_array($value, $conds) || !is_array($conds) && $conds === $value);
  }

  /**
   * Order and serialize fields so we can get a unique signature for this subscription fields
   */
  function serialize_fields() {
    return notifications_array_serialize($this->get_conditions());
  }
  /**
   * Serialize type and conditions
   */
  function serialize_type() {
    return implode('/', array($this->type, $this->serialize_fields()));
  }
  /**
   * Add a new field with name, value
   */
  function add_field($type, $value = NULL) {
    $this->set_field($this->build_field($type, $value));
  }
  /**
   * Add field arguments from url
   * 
   * @param $fields
   *   String of field names separated by commas
   * @param $values
   *   String of field values separated by commas
   */
  function add_field_args($fields, $values) {
    $names = explode(',', $fields);
    $params = explode(',', $values);
    foreach ($names as $index => $type) {
      $this->add_field($type, isset($params[$index]) ? $params[$index] : NULL);
    }    
  }
  
  /**
   * Set a field. 
   * - If the field has already a key (index) it will replace an existing one with the same key
   * - If not key, it will be added to the field list with an ordinary key generated
   * 
   * @param $field
   *   Field object with one or more of these properties: key, type, field, value
   */
  function set_field($field, $replace = FALSE) {
    if (!isset($field->key) || !$replace) {
      $field->key = isset($this->fields) ? count($this->fields) : 0;
    }
    // Add to the fields array, may replace an old one
    $this->fields[$field->key] = $field;
  }
  /**
   * Get fields as array of field objects
   */
  function get_fields() {
    if (!isset($this->fields)) {
      if ($this->is_instance()) {
        $this->load_fields();
      }
      else {
        $this->set_fields();
      }
    }
    return $this->fields;
  }
  /**
   * Check all fields are there and optinally that they have a value
   */
  function check_fields($check_values = TRUE) {
    $this->set_fields();
    $instance_fields = array();
    foreach ($this->get_fields() as $field) {
      if ($check_values && !isset($field->value)) {
        return FALSE;
      }
      else {
        $instance_fields[] = $field->field;
      }
    }
    $type_fields = array();
    foreach ($this->get_type_fields() as $field) {
      $type_fields[] = $field->field;
    }
    if (count($instance_fields) != count($type_fields)) {
      return FALSE;
    }
    else {
      // Set the number of conditions if not set 
      if (!isset($this->conditions)) {
        $this->conditions = count($instance_fields);
      }      
      // Order the arrays so we can compare them. Note that we can have the same field name more than once
      asort($instance_fields);
      asort($type_fields);
      return array_values($instance_fields) == array_values($type_fields);
    }
  }
  /**
   * Set field values, all at a time
   * 
   * Each field item may have different formats:
   * - Numeric key => field type. Like array(nid, tid)
   * - Numeric key => array('type' => type, 'value' => value)
   * - Numeric key => Object ($field object with field, value properties)
   * @param $fields
   *   Fields is an array of field items, each of them with many possible formats:
   *   NULL to set the fields from the subscription type or template if not set
   */
  function set_fields($fields = NULL, $normalize = TRUE) {
    if (isset($fields)) {
      $this->fields = array();
      $this->add_fields($fields, $normalize);
    }
    elseif (!isset($this->fields)) {
      // Set the fields from the subscription type or template
      $this->fields = array();
      $this->add_fields($this->get_type_fields(), FALSE);
    }
  }
  /**
   * Add field values to existing ones
   */
  function add_fields($fields, $normalize = TRUE) {
    if ($normalize) {
      $fields = $this->normalize_fields($fields);
    }
    foreach ($fields as $key => $value) {
      $this->set_field($value);
    }    
  }
  /**
   * Get fields from subscription type as normalized array of objects
   */
  function get_type_fields() {
    return $this->normalize_fields($this->get_type('fields'));
  }
  /**
   * Convert fields to a standard format: arrray of object fields
   */
  public static function normalize_fields($fields) {
    $normal = array();
    if ($fields && is_array($fields)) {
      foreach ($fields as $key => $value) {
        if (is_object($value)) {
          $normal[] = $value;
        }
        elseif (is_array($value)) {
          // Object or Array with type, value keys
          $normal[] = self::build_field($value['type'], $value['value']);
        }
        elseif (is_numeric($key) && is_string($value)) {
          // In this case the value is the field type
          $normal[] = self::build_field($value, NULL);
        }
        else {
          // Default, key should be field type, value may be NULL
          $normal[] = self::build_field($key, $value);
        }
      }
    }
    return $normal;    
  }
  /**
   * Build field from type, value
   */
  public static function build_field($type, $value) {
    return (object)array(
      'field' => $type,
      'value' => $value,
    );
  }
  /**
   * Get destination object
   */
  function get_destination() {
    if (!isset($this->destination_object)) {
      $this->destination_object = !empty($this->mdid) ? Messaging_Destination::load($this->mdid) : FALSE;
    }
    return $this->destination_object;
  }

  /**
   * Get language object
   */
  function get_language() {
    if (!isset($this->_language)) {
      if (!empty($this->language) && ($languages = language_list()) && isset($languages[$this->language])) {
        $this->_language = $languages[$this->language];
      }
      else {
        $this->_language = user_preferred_language($this->get_account());
        $this->language = $this->_language->language;
      }
    }
    return $this->_language;
  }

  /**
   * Set destination object
   */
  function set_destination($destination) {
    if (empty($destination)) {
      $this->mdid = 0;
      $this->destination = '';
      $this->destination_object = FALSE;
    }
    elseif (is_object($destination)) {
      $this->uid = $destination->uid;
      $this->mdid = $destination->mdid;
      $this->destination = $destination->address;
      $this->destination_object = $destination;
    }
    elseif (is_numeric($destination)) {
      $this->mdid = $destination;
    }
  }
  /**
   * Get subscription type data
   */
  function get_type($property = NULL) {
    return empty($this->type) ? NULL : notifications_subscription_types($this->type, $property);
  }

  /**
   * Whether this subscription's fields are editable or not
   * 
   * Unless preset the 'editable' property, this is how it works:
   * - Once we have an instance we don't allow changing the fields, which may cause some consistency problems
   * - Also if the subscription type has no fields, this is not editable
   * - When it has fields and they've been all preset, not editable either
   */
  function is_editable() {
    if (!isset($this->editable)) {
      if (!$this->is_instance() && ($type_fields = $this->get_type_fields())) {
        // It is editable if not all fields are set
        $this->editable = count($type_fields) > count($this->get_instance_fields());
      }
      else {
        // It is instance or the type has no fields
        $this->editable = FALSE; 
      }
    }
    return $this->editable;
  }
  /**
   * Get editable fields. They're the ones in the subscription type that have no value
   */
  function get_editable_fields() {
    return $this->filter_fields($this->get_type_fields(), FALSE);
  }
  /**
   * Get instance fields that have a value set
   */
  function get_instance_fields() {
    return $this->filter_fields($this->get_fields());
  }
  /**
   * Filter out fields that have no value set ($isset = FALSE to get the ones that are set)
   */
  public static function filter_fields($fields, $isset = TRUE) {
    foreach ($fields as $key => $field) {
      if ($isset && !isset($field->value) || !$isset && isset($field->value)) {
        unset($fields[$key]);
      }
    }
    return $fields;    
  }

  /**
   * Get instance of this one for certain conditions
   * 
   * @param $params
   *   Parameters to add to the subscription type to get an instance of itself
   * @param $return_self
   *   Return itself if no instance found
   */
  function get_instance($params = array(), $return_self = FALSE) {
    $params += array('type' => $this->type);
    $cache = &messaging_static('Notifications_Subscription_instances');
    $cache_key = notifications_array_serialize($params);
    if (!isset($cache[$cache_key])) {
      $subs = notifications_get_subscriptions($params);
      $cache[$cache_key] = $subs ? current($subs) : FALSE;      
    }
    return $cache[$cache_key] ? $cache[$cache_key] : ($return_self ? $this : NULL);
  }

  /**
   * Load subscription objects
   */
  function load_objects() {
    if (!isset($this->objects)) {
      $this->objects = array();
      foreach ($this->get_fields() as $field) {
        if ($type = notifications_subscription_fields($field->field, 'object_type')) {
          if ($object = notifications_object_load($type, $field->value)) {
            if (!isset($this->objects[$type])) {
              $this->objects[$type] = $object;
            }
            elseif (is_array($this->objects[$type])) {
              // Was an array, just add
              $this->objects[$type][] = $object;
            }
            else {
              // Was single element, make into an array with this new object
              $this->objects[$type] = array($this->objects[$type], $object);
            }
          }
          else {
            // Object cannot be loaded, mark as incomplete
            $this->incomplete = TRUE;
          }
        }
      } 
    }
    return empty($this->incomplete);
  }
  /**
   * Get objects
   */
  function get_objects() {
    $this->load_objects();
    return $this->objects;
  }

  /**
   * Format as short text
   */
  function format_short($format = self::FORMAT_HTML) {
    return t('@type: !values', array('@type' => $this->get_type('title'), '!values' => $this->format_name($format | self::FORMAT_INLINE)));
  }
  /**
   * Format as long text
   */
  function format_long($format = self::FORMAT_HTML) {
    return t('Subscription %id of type %type to: !values', array('%id' => $this->sid, '%type' => $this->get_type('title'), '!values' => $this->format_name($format | self::FORMAT_INLINE)));
  }
  
  /**
   * Get subscription short name.
   */
  function get_name($format = self::FORMAT_HTML) {
    if (!isset($this->name)) {
      // This subscription doesn't have a name, we'll find something
      $this->name = $this->format_name($format | self::FORMAT_INLINE);
    }
    return $this->name;
  }
  /**
   * Get description
   */
  function get_description($format = self::FORMAT_HTML) {
    return $this->get_type('description');
  }
  // Get title
  function get_title() {
    return $this->get_type('title');
  }   
  /**
   * If the subscription type has a name, like custom subscriptions have, that will be the name
   * Otherwise we build the name using fields and values
   */
  function format_name($format = self::FORMAT_PLAIN) {
    if ($name = $this->get_type('name')) {
      return $name;
    }
    else {
      return $this->format_fields($format);
    }   
  }
  
  /**
   * Format all fields
   *
   * @return array();
   *   Array of arrrays with (name, value)
   */
  function format_fields($format = self::FORMAT_HTML) {
    if (!isset($this->format[$format]['fields'])) {
      // Get field names and values formatting each field
      $items = array();
      foreach ($this->get_fields() as $field) {
        $items[] = $this->format_field($field, $format);
      }
      $this->format[$format]['fields'] = $this->format_items($items, $format);
    }
    return $this->format[$format]['fields'];
  }
  
  /**
   * Format items
   *
   * @param $items
   *   Array of arrays with 'name' and 'value' elements
   */
  function format_items($items, $format = self::FORMAT_INLINE) {
    // If no items the output will be always an empty string
    if (!$items)  {
      return '';
    }
    // Some formats need each item to be a string first 
    if ($format & self::FORMAT_INLINE) {
      foreach ($items as $key => $value) {
        if (is_array($value)) {
          $items[$key] = implode(': ', $value);
        }
      }
    }
    switch (TRUE) {
      case $format & self::FORMAT_INLINE:
        return implode(',', $items);
      case $format & self::FORMAT_LIST:
        return theme('item_list', $items);
      case $format & self::FORMAT_TABLE:
        return theme('table', array(), $items);

      default:
        // Items not formatted, return as array
        return $items;
    }
  }
  /**
   * Format subscriptions field for display and get some more information
   * 
   * @return array()
   *   Array with 'name' and 'value' elements
   */
   function format_field($field, $format = self::FORMAT_HTML) {
    notifications_include('object.inc');

    $type = $field->field;
    $value = $field->value;
    
    $format_name = notifications_field_format_name($type);
    $format_value = notifications_field_format_value($type, $value, $format & self::FORMAT_HTML, NULL);
    
    return $format & self::FORMAT_INLINE ? $format_name . ': ' . $format_value : array('name' => $format_name, 'value' => $format_value);
  }

  /**
   * Subscription information field for several forms
   * 
   * @return Forms API field structure
   */
  function form_info() {
    $info = $this->get_type();
    // Get fields formatted as array of items
    $fields = $this->get_fields();
    if (!empty($info['name'])) {
      // This subscription type already has a name
      $value = $info['name'];
    }
    elseif (empty($fields)) {
      // No name, maybe no fields it should be enough with the title
      $value = '';
    }
    elseif (count($fields) == 1) {
      // If the field is unique, we don't need a table nor a name for it
      $value = $this->format_fields(self::FORMAT_HTML | self::FORMAT_INLINE);
    }
    else {
      // Multiple fields, format as a table
      $value = $this->format_fields(self::FORMAT_TABLE);
    }
    // Build a form field with all these values
    $field = array(
      '#type' => 'item',
      '#title' => t('@type subscription', array('@type' => $this->get_type('title'))),
      '#value' => $value,
    );
    if (!empty($info['description'])) {
      $field['#description'] = $info['description'];
    }
    return $field;
  }

  /**
   * Display a form field for a notifications_field
   */
  public function field_element($type, $value = NULL, $title = FALSE, $required = FALSE ) {
    notifications_include('object.inc');
    return notifications_field_form_element($type, $value, $this, $title, $required);
  }

  /**
   * Status list
   */
  public static function status_list() {
    return array(
      self::STATUS_ACTIVE => t('active'),
      self::STATUS_BLOCKED => t('blocked'),
      self::STATUS_INACTIVE => t('inactive'),
      self::STATUS_DISABLED => t('disabled'),
   );
  }

  /**
   * Magic method, set protected properties
   * 
   * Warning: This also runs for unset properties
   */
  public function __set($name, $value) {
    switch ($name) {
      case 'fields':
        $this->set_fields($this->normalize_fields($value));
        break;
      case 'destination':
        if (is_object($value)) {
          $this->set_destination($value);
        }
        else {
          $this->destination = $value;
        }
        break;
      default:
        parent::__set($name, $value);
    }
  }
  /**
   * Magic method, get protected properties
   */
  public function __get($name) {
    switch ($name) {
      case 'fields':
        return $this->get_conditions();
        break;
    }
  }
  // Get table name for storage
  public static function db_table() {
    return self::DB_TABLE;
  }
  // Get key field name for storage
  public static function db_key() {
    return self::DB_KEY;
  }
}