<?php
/**
 * @file
 * Drupal Notifications Framework - Default class file
 */

/**
 * Message destination class
 */
class Notifications_Event extends Messaging_Cached_Object {
  // Database properties
  const DB_TABLE = 'notifications_event';
  const DB_KEY = 'eid';
  // Object unique id
  public $eid;
  public $module = 'notifications';
  public $type = '';
  public $action = '';
  // Event key that will define event behavior
  public $typekey;
  // Main object id
  public $oid;
  // Mixed array of parameters
  public $params = array();
  // Time the event was produced
  public $created;
  // Notifications in queue linked to this event
  public $counter = 0;
  // Loaded objects for this event
  public $objects;
  // Processing options, not saved
  public $save = TRUE;
  public $queue = TRUE;
  public $queued = FALSE;
  // Will be set if any of the objects cannot be loaded
  public $incomplete = FALSE;
  // Event text for composition
  public $text;
  // Template to use for this event
  protected $template;
  // Temporary storage for digesting information
  public $digest;
  // Temporary cache, user access to event
  protected $_access;
  // Shared template map for all events
  protected static $_templates;

  // Track events for immediate sending
  protected static $_send_immediate;
  // Track events for queue processing
  protected static $_event_tracker;

  /**
   * Constructor
   */
  function __construct($object = NULL) {
    parent::__construct($object);
    if (!isset($this->created)) {
      $this->created = time();
    }
    if (!isset($this->typekey)) {
      $this->typekey = $this->type . '-' . $this->action;
    }
  }
  /**
   * Building function
   */
  public static function build($template) {
    return new Notifications_Event($template);
  }
  /**
   * Get event type information
   */
  function get_type($property = NULL) {
    if ($info = notifications_event_types($this->typekey)) {
      if ($property) {
        return isset($info[$property]) ? $info[$property] : NULL;
      }
      else {
        return $info;
      }
    }
  }
  /**
   * Get template name
   */
  function get_template() {
    if (!isset($this->template)) {
      if ($template = self::template_map($this->typekey)) {
        $this->template = $template;
      }
      else {
        // Build default that will be like notifications-event-type-action
        $parts = array($this->module, 'event', $this->type, $this->action);
        $this->template = implode('-', array_filter($parts));
      }
    }
    return $this->template;
  }

  /**
   * Get message template for event
   * 
   * Replaces notifications_event_message_part()
   * 
   * @param $event
   *   Message type to send, either 'event' or 'digest'
   * @param $key
   *   Id of message part, ie 'header'
   * @param $method
   *   Method by which message will be sent. Normally 'mail'
   * @param $param
   *   Event data if we have a single event (type = event), none if we are digesting multiple events (type = digest)
   *   
   * @return
   *   Part of the message with tokens for replacement.
   */
  function message_part($key, $method, $language, $module = 'notifications') {
    // If event passed check for predefined text or get optional keys from event
    if ($text = $this->get_text($key)) {
      return $text;
    }
    elseif ($template = $this->get_template()) {
      $text = messaging_template_text_part($template, $key, $method, $language);
    }
    // Default, template not found, return some information if debugging
    return isset($text) ? $text : '';
  }
  
  /**
   * Get event text if available
   */
  function get_text($key) {
    if (isset($this->text[$key])) {
      return $this->text[$key];
    }
    elseif (isset($this->params['text'][$key])) {
      return $this->params['text'][$key];
    }
  }

  /**
   * Get template name for event type
   * 
   * Replaces notifications_event_get_template()
   * 
   * @param $event_type
   *   Event type key
   */
  public static function template_map($event_type) {
    if (!isset(self::$_templates)) {
      self::$_templates = variable_get('notifications_event_template', array());
    }

    if (!isset(self::$_templates[$event_type])) {
      if ($template = notifications_event_types($event_type, 'template')) {
        // Use default value from event definition
        self::$_templates[$event_type] = $template;
      }
      else {
        // Mark as false for not searching again
        self::$_templates[$event_type] = FALSE;
      }
    }

    return self::$_templates[$event_type];
  }

  /**
   * Get set event object
   */
  function object($type, $object = NULL) {
    if (isset($object)) {
      $this->add_object($type, $object);
    }
    else {
      $this->load_objects();
      return isset($this->objects[$type]) ? $this->objects[$type] : NULL;
    }
  }
  /**
   * Add event object
   * 
   * @param $type
   *   Object type: 'user', 'node', etc...
   * @param $object
   *   Full object or key field value. I.e. it may be nid, uid
   */
  function add_object($type, $object) {
    $key = notifications_object_type($type, 'key_field');
    $value = is_object($object) ? $object->$key : $object;
    $this->params['objects'][$type] = $value;
    if (is_object($object)) {
      $this->set_object($type, $object);
    }
  }
  /**
   * Add event object to be serialized and stored with the event
   */
  function store_object($type, $object) {
    $this->params['objects'][$type] = $object;
    $this->set_object($type, $object);
  }
  /**
   * Set event object
   */
  function set_object($type, $object) {
    $this->objects[$type] = $object;
  }
  /**
   * Get event objects
   */
  function get_objects() {
    // Make sure objects are loaded
    $this->load_objects();
    return $this->objects;
  }
  /**
   * Get single object
   */
  function get_object($type) {
    $this->load_objects();
    return isset($this->objects[$type]) ? $this->objects[$type] : NULL;
  }
  /**
   * Load event objects
   * 
   * @return Boolean
   *   Object status, FALSE for incomplete (missing object)
   */
  function load_objects($refresh = FALSE) {
    if (!isset($this->objects) || $refresh) {
      $this->objects = array();
      if (!empty($this->params['objects'])) {
        foreach ($this->params['objects'] as $type => $value) {
          if (is_object($value)) {
            // This object was serialized along with the event, just set it
            $this->objects[$type] = $value;
          }
          elseif ($object = notifications_object_load($type, $value)) {
            $this->objects[$type] = $object;
          }
          else {
            // Cannot find object, mark as incomplete and return FALSE
            $this->incomplete = TRUE;
            return FALSE;
          }
        }
      }
    }
    // Return true unless marked as incomplete
    return $this->incomplete ? FALSE : TRUE;
  }

  /**
   * Trigger this event and process it
   */
  /**
   * Trigger event. Save, run queue query, etc...
   * 
   * Replaces notifications_event_trigger($event)
   */
  function trigger() {
    // Notify other modules we are about to trigger some subscriptions event
    // Modules can do cleanup operations or modify event properties
    module_invoke_all('notifications_event', 'trigger', $this);
    
    // Store event, unles marked not to be saved 
    if ($this->save) {
      $this->save();
      // Send event to queue for subscriptions, unless marked not to
      if ($this->queue) {
        $this->queued = TRUE;
        $this->counter = notifications_queue()->queue_event($this);
  
        // Modules can do cleanup operations or modify the queue or the event counter
        module_invoke_all('notifications_event', 'queued', $this);
        $count = $this->counter;
  
        // Now update event counter with rows in notifications_queue or delete if no rows
        if ($count) {
          $this->update_counter($count);
          // If immediate sending enabled, store it for sending on page exit.
          if (variable_get('notifications_send_immediate', 0)) {
            self::send_immediate($this);
          }
        }
        else {
          $this->delete();
        }
      }
    }
  }

  /**
   * Check user access to event's objects
   * 
   * Replaces notifications_event_user_access($event, $account);
   */
  function user_access($account) {
    if (!isset($this->_access[$account->uid])) {
      $this->_access[$account->uid] = notifications_user_allowed('event', $account, $this);
    }
    return $this->_access[$account->uid];
  }
  
  /**
   * Build new event object from array of data
   * 
   * Replaces notifications_event_build()
   * 
   * @param $params
   *   Event properties
   * @param $objects
   *   Array of event objects (type => $object)
   */
  public static function create($params, $objects = array()) {
    global $user, $language;
    
    // Fill in event with default values
    $params += array(
      'uid' => $user->uid,
      'language' => $language->language,
      'type' => 'default', // Object/event type
      'action' => 'default', // Action that happened to the object
    );
    $params += array('typekey' => $params['type'] . '-' . $params['action']);
    // Check whether we have to save and queue this event, defaults to yes if not set
    // If not enabled, do not store nor queue this event, can be changed by plug-in modules
    $enabled = notifications_event_enabled($params['typekey']);
    $params += array('save' => $enabled, 'queue' => $enabled);
    $event = new Notifications_Event($params);
    foreach ($objects as $type => $object) {
      $event->add_object($type, $object);
    }
    // Modules can do cleanup operations or modify event properties
    module_invoke_all('notifications_event', 'build', $event);
    return $event;
  }

  /**
   * Clean up queued events
   * 
   * Replaces notifications_event_clean()
   * 
   * @param $update
   *   Update event counter
   */
  public static function queue_clean($update = FALSE) {
    return notifications_queue()->event_clean($update);
  }
  
  /**
   * Load by id passing through notifications_event() hook
   * 
   * Replaces notifications_event_load($id)
   */
  public static function load($id) {
    $cached = self::cache_get($id);
    if (isset($cached)) {
      return $cached;
    }
    elseif ($event = self::object_load(self::DB_TABLE, self::DB_KEY, $id, 'Notifications_Event')) {
      // Inform other modules, give them the chance to alter
      module_invoke_all('notifications_event', 'load', $event);
      return $event;
    }
  }

  /**
   * Unserialize after db loading
   */
  public function unserialize() {
    $this->params = $this->params ? unserialize($this->params) : array();
  }
  /**
   * Track notifications queue row processed, decrease counter
   */
  function track_count() {
    return $this->counter ? --$this->counter : 0;
  }
  /**
   * Update event counter
   */
  function update_counter($value = NULL) {
    if (isset($value)) {
      $this->counter = $value;
    }
    db_query('UPDATE {notifications_event} SET counter = %d WHERE eid = %d', $this->counter, $this->eid);
  }
  // Store into cache
  public static function cache_set($key, $object) {
    return self::object_cache_set('Notifications_Event', $key, $object);
  }
  // Get from cache
  public static function cache_get($key) {
    return self::object_cache_get('Notifications_Event', $key);
  }
  // Get table name for storage
  public static function db_table() {
    return self::DB_TABLE;
  }
  // Get key field name for storage
  public static function db_key() {
    return self::DB_KEY;
  }

  /**
   * Track events and keep them updated
   * 
   * It will delete events when objects are not available anymore or when event counter reaches 0
   * 
   * Replaces notifications_event_tracker()
   */

  /**
   * Add event to be tracked
   */
  public function track() {
    self::$_event_tracker[$this->eid] = $this;
  }
  
  // Load and add to the tracker
  public static function track_load($eid) {
    if (($event = Notifications_Event::load($eid)) && $event->load_objects() && empty($event->delete)) {
      $event->track_count();
      $event->track();
      return $event;
    }    
  }

  // Delete event and all related rows
  public static function track_delete($event) {
    unset($this->tracker[$event->eid]);
    $event->delete();
  }

  // Update tracked events counter or delete if counter reached zero or marked for deletion.
  public static function track_update() {
    if (!empty(self::$_event_tracker)) {
      foreach (self::$_event_tracker as $eid => $event) {
        if (!$event->counter || $event->incomplete || !empty($event->delete)) {
          $event->delete();
        }
        else {
          $event->update_counter();
        }
        unset(self::$_event_tracker[$eid]);
      }
    }
  }
  // Reset tracked event list
  public function track_reset() {
    self::$_event_tracker = array();
  }
  
  /**
   * Store / return events for immediate sending
   */
  public static function send_immediate($event = NULL) {
    if ($event) {
      // Store events anyway so they can be accessed through the static variable
      // and other modules can have a peek on what has happened on this request
      self::$_send_immediate[$event->eid] = $event;
    }
    elseif (!empty(self::$_send_immediate)) {
      return self::$_send_immediate;
    }
  }
  
  /**
   * Process events stored for immediate sending
   * 
   * This will be called from notifications_exit() when the event API is loaded
   */
  public static function process_immediate() {
    if ($events = self::send_immediate()) {
      foreach ($events as $event) {
        notifications_queue()->process_rows(array('cron' => 1, 'eid' => $event->eid, 'send_interval' => 0));
      }    
    }
  }
}
