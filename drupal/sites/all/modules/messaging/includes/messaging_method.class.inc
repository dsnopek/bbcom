<?php
/**
 * @file
 * Drupal Messaging Framework - Send_Method class file
 */

/**
 * Base class for all Incoming and Sending methods
 */
abstract class Messaging_Method {
  // Method with push delivery. Messages will be pushed to the user using messaging sending methods.
  const TYPE_PUSH = 1;
  // Method type with pull delivery. Messages will be pulled using messaging pull methods
  const TYPE_PULL = 2;
  // Outgoing method
  const TYPE_OUTGOING = 4;
  // Incoming method
  const TYPE_INCOMING = 8;
  // Disable queueing (sending is more expensive than queueing)
  const TYPE_NOQUEUE = MESSAGING_TYPE_NOQUEUE;
  // Shorthand type: Push + Outgoing
  const TYPE_SEND = MESSAGING_TYPE_SEND;
  // Common properties
  // Method key
  public $method;
  public $type;
  public $title;
  public $name;
  public $description;
  public $group;
  // All sending methods are enabled by default
  public $enabled = TRUE;
  // Remaining info array
  public $info = array();
  
  /**
   * Build send method from info array
   *
   * Some of the array values will be set as properties for the object. Some others won't as they're just
   * for formatting, so they'll be kept only in the $object->info array
   */
  function __construct($method, $info = array()) {
    $this->method = $method;
    $this->info = $info;
    $this->set_properties($info,'type', 'title', 'name', 'group', 'description', 'enabled'); 
  }
  
  /**
   * Set multiple properties from array
   * 
   * @param $data
   *   Array of properties
   * @param $name1, $name2...
   *   Names of properties to set if they're in the array
   */
  function set_properties() {
    $args = func_get_args();
    if ($data = array_shift($args)) {
      // If no parameters passed, set them all
      $properties = $args ? $args : array_keys($data);
      while ($name = array_shift($properties)) {
        if (isset($data[$name])) {
          $this->$name = $data[$name];
        }
      }
    }
  }
  /**
   * Get info property
   */
  function get_info($property = NULL) {
    if ($property) {
      return isset($this->info[$property]) ? $this->info[$property] : NULL;
    }
    else {
      return $this->info;
    }
  }
  /**
   * Renders full message with header and body
   * 
   * @param $message
   *   Message object
   * @param $info
   *   Sending method info for rendering (glue and filter options)
   */
  static function default_render($message, $info = array()) {
    messaging_include('text.inc');
    messaging_debug('Rendering message', array('message' => $message, 'info' => $info));
    // Apply footer prefix if provided and the message has a footer element. 
    // Note: If message body is a string the final isset($message['body']['footer']) will be true
    if (!empty($info['footer']) && is_array($message->body) && isset($message->body['footer'])) {
      $message->body['footer'] = array('#prefix' => $info['footer'], '#text' => $message->body['footer']);
    }
    // Render separately subject and body info, adding default parameters
    $info += array('glue' => ' ', 'subject_glue' => ' ', 'body_format' => NULL, 'filter' => NULL);
    $message->subject = self::check_subject(self::text_render($message->subject, $info['subject_glue']));
    $message->body = self::text_render($message->body, $info['glue'], $info['body_format'], $info['filter']); 
    $message->rendered = TRUE;  
    messaging_debug('Rendered message', array('message' => array('subject' => $message->subject, 'body' => $message->body), 'info' => $info));
    return $message;
  }

  /**
   * Converts strings to plain utf-8 single line
   */
  static function check_subject($text) {
    $text = messaging_text_check_plain($text, NULL);
    // taken from _sanitizeHeaders() in PEAR mail() : http://pear.php.net/package/Mail
    $text = preg_replace('=((0x0A/%0A|0x0D/%0D|\\n|\\r)\S).*=i', NULL, $text);
    return $text;
  }
  
  /**
   * Composes message from different parts, recursively and applies filter
   * 
   * Filter is applied now only once
   * 
   * @param $text
   *   Simple string or array of message parts
   *   It may have named elements like #prefix and #text
   *   or it may be single strings to render straight forward
   * @param $glue
   *   Text to glue all lines together
   * @param $filter
   *   Input format to apply to the results
   */
  static function text_render($text, $glue = '', $format = NULL, $filter = NULL) {
    return messaging_text_render($text, $glue, $format, $filter);
  }
  
  /**
   * Clean text of HTML stuff and optionally of line endings
   * 
   * @param $text
   *   Dirty HTML text to be filtered
   * @param $newline
   *   Optional string to be used as line ending
   */
  static function text_clean($text, $newline = NULL) {
    return messaging_text_clean($text, $newline);
  }
  
  /**
   * Truncate messages to given length.  Adapted from node_teaser() in node.module
   */
  static function text_truncate($text, $length) {
    return messaging_text_truncate($text, $length);
  }
}

/**
 * Sending method, implements all specific method functionality
 * 
 * Old callback functions are
 * - send
 * - destination
 */
class Messaging_Send_Method extends Messaging_Method {
  // Default type is send (Outgoing + Push)
  public $type = Messaging_Method::TYPE_SEND;
  // Address type for this method
  public $address_type = 'user';
  // Queue and log settings
  public $queue = FALSE;
  public $log = FALSE;
  // Suitable for anonymous users
  public $anonymous = FALSE;
  
  /**
   * Build send method from info array
   *
   * Some of the array values will be set as properties for the object. Some others won't as they're just
   * for formatting, so they'll be kept only in the $object->info array
   */
  function __construct($method, $info = array()) {
    parent::__construct($method, $info);
    $this->set_properties($info, 'address_type', 'anonymous', 'queue', 'log');
    // Pull methods will have always queueing enabled
    $this->queue = $this->queue || $this->type & self::TYPE_PULL;
  }
 
  /**
   * Get default method parameters to be merged with the ones passed on with the message
   */
  static function default_params($message = NULL) {
    return array();
  }
  /**
   * Get address info property
   */
  function get_address_info($property = NULL) {
    return messaging_address_info($this->address_type, $property);
  }
  /**
   * Send message to address, use sending callback
   */
  function send_address($address, $message) {
    if ($send_callback = $this->get_info('send callback')) {
      // For this case, we render it before so it is compatible with old send methods
      $message->render();
      $params = $message->get_method_params($this->method) + $this->default_params();
      return call_user_func($send_callback, $address, $message, $params);
    }
    else {
      watchdog('messaging', 'Message could not be delivered for method %method', array('%method' => $this->method), WATCHDOG_ERROR);
    }
  }

  /**
   * Send message to destination, use sending callback
   */
  function send_destination($destination, $message) {
    return $this->send_address($destination->address, $message);
  }
 
  /**
   * Get address for user account
   * 
   * @param $account
   *   User account object or uid
   */
  function get_user_address($account) {
    // The method may have its own destination callback. If not, default to address type,
    if ($function = $this->get_info('destination callback')) {
      return $function(messaging_user_object($account));
    }
    elseif ($function = $this->get_address_info('user2address callback')) {
      return $function($account);
    }
    elseif ($property = $this->get_address_info('account_property')) {        
      return messaging_user_property($account, $property);
    }
  }

  /**
   * Check user access to this method
   */
  function user_access($account) {
    if (!$account->uid && !$this->anonymous) {
      return FALSE;
    }
    if ($permission = $this->get_info('access')) {
      return user_access($permission, $account);
    }
    else {
      return TRUE;
    }
  }

  /**
   * Get address name
   */
  function address_name() {
    if (empty($this->address_name)) {
      if ($name = $this->get_address_info('name')) {
        $this->address_name = $name;
      }
      else {
        $this->address_name = t('Address');
      }
    } 
    return $this->address_name; 
  }

  /**
   * Get uid for address
   */
  function get_address_uid($address) {
    if ($this->address_type == 'user') {
      return (int)$address;
    }
    elseif ($function = $this->get_address_info('address2uid callback')) {
      return $function($address);
    }
  }
  /**
   * Validate address
   */
  function address_validate($address) {
    if ($function = $this->get_address_info('validate callback')) {
      return $function($address);
    }
    else {
      // The default address will be valid if not empty
      return !empty($address);
    }
  }
  /**
   * Format address for display
   */
  function format_address($address, $html = FALSE) {
    if ($function = $this->get_address_info('format callback')) {
      return $function($address, $html);
    }
    else {
      return check_plain($address);
    }
  }

  /**
   * Prepare message for specific user. Check availability, redirect, etc..
   * 
   * Redirecting is only possible when we are sending to a user account, not for anonymous destinations
   */
  function message_user($message, $account = NULL) {
    if ($callback = $this->get_info('user callback')) {
      $account = $account ? $account : $message->get_user();
      call_user_func($callback, $message, $account);
    }
  }

  /**
   * Message processing: Decide on queue, log, cron and send options, prepare parameters
   * 
   * At this stage, the message can be still redirected through other sending method, or marked for discard
   */
  function message_prepare($message) {
    // If this method is disabled, mark for discard, log the error
    if (!$this->enabled) {
      $message->discard = TRUE;
      $message->queue = $message->cron = 0;
      $message->set_error(t('The sending method is disabled.'));
    }
    else {
      // It will be queued always for pull methods, cron disabled though so it will wait till it's pulled
      $message->queue = !($this->type & self::TYPE_NOQUEUE) && (($this->type & self::TYPE_PULL) || !$message->priority && ($message->queue || $this->queue));
      $message->log = $message->log || $this->log && variable_get('messaging_log', 0);
      // If the messaging method is of type push, cron processing will be enabled
      $message->cron = $message->cron || $message->queue && ($this->type & self::TYPE_PUSH);
    }
    // Aditionally we can have a prepare callback
    if ($function = $this->get_info('prepare callback')) {
      $function($message, $this->get_info());
    }
  }
  /**
   * Renders full message with header and body
   * 
   * @param $message
   *   Message object
   */  
  function message_render($message) {
    if ($function = $this->get_info('render callback')) {
      return $function($message, $this->get_info());
    }
    else {
      return $this->default_render($message, $this->get_info());
    }
  }

  /**
   * Send message to a single destination
   */
  function message_send($message) {
    return $this->send_address($message->destination, $message);
  }

  /**
   * Message default callback: send iterating over all destinations
   * 
   * This can be overridden by any sending method that can send multiple messages in a more efficient way.
   */
  function message_multisend($message) {
    $success = $fail = array();
    foreach ($message->get_addresses() as $to) {
      if ($this->send_address($to, $message)) {
        $success[] = $to;
      }
      else {
        $fail[] = $to;
      }
    }
    // If sent, set time. If failed set error
    if ($success) {
      $message->sent = time();
    }
    if ($fail) {
      $destinations = check_plain(implode(', ', $fail));
      $message->set_error("Sending ($this->method) to some destinations failed: $destinations.");
    }
    return $success && !$fail;
  }
 
  /**
   * The message has been sent
   */
  function message_aftersend($message) {
  }
 
  /**
   * Queue message for next delivery
   * 
   * By default it is saved to the store, though some sending methods like 'simple' may not consider queueing.
   */
  function message_queue($message) {
    return messaging_store()->message_queue($message);
  }
  
  /**
   * The message has been queued
   */
  function message_afterqueue($message) {
  }
  
}
