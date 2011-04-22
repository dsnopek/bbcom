<?php
/**
 * @file
 * Drupal Messaging Framework - Default class file
 */

/**
 * Message destination class
 */
class Messaging_Destination extends Messaging_Cached_Object {
  // Database properties
  const DB_TABLE = 'messaging_destination';
  const DB_KEY = 'mdid';

  // Destination status
  const STATUS_PENDING = 0;
  const STATUS_ACTIVE = 1;

  // Object unique id
  public $mdid = 0;
  // Destination type
  public $type;
  // User id. Public parent protected variable
  public $uid;
  // Address for this sending method
  public $address;
  // Date sent
  public $sent = 0;
  // Status
  public $status = 0;
  
  /**
   * Constructor
   */
  function __construct($template = NULL) {
    parent::__construct($template);
  }

  /**
   * Get from db using conditions
   */
  public static function get($params) {
    if ($object = db_fetch_object(self::db_query("SELECT *", $params))) {
      $dest = self::build($object);
      $dest->cache_save();
      return $dest;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Build destination with partial parameters
   * 
   * @param $template
   *   Object template or address type
   */
  public static function build($template) {
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
    if ($type && ($class = messaging_address_info($type, 'destination_class'))) {
      return new $class($object);
    }
    else {
      return new Messaging_Destination($object);
    }    
  }
  /**
   * Load object from database
   */
  public static function load($key) {
    return self::object_load(self::DB_TABLE, self::DB_KEY, $key, 'Messaging_Destination');
  }

  /**
   * Validate values to create a destination
   * 
   * @param $method
   *   Send method
   * @param $address
   *   Address value
   * @param $account
   *   Optional user id or account object the destination is intended for
   */
  public static function validate_method($method, $address, $account = NULL) {
    // First validate address and check permission
    $send_method = messaging_send_method($method);
    if (!$send_method || !$send_method->address_validate($address)) {
      return FALSE;
    }
    if (isset($account)) {
      $account = messaging_user_object($account);
      if (!$account || !$send_method->user_access($account)) {
        return FALSE;
      }
    }
    if ($type = messaging_method_info($method, 'address_type')) {
      return self::validate_type($type, $address, $account);
    }
  }
  /**
   * Validate values to create a destination
   * 
   * @param $type
   *   Address type
   * @param $address
   *   Address value
   * @param $account
   *   Optional user id or account object
   */
  public static function validate_type($type, $address, $account = NULL) {
    // First try validate callback for this address type
    if (!self::validate_address($type, $address)) {
      return FALSE;
    }
    elseif (isset($account)) {
      $uid = messaging_user_uid($account);
      if ($existing = self::get_by_address($type, $address)) {
        // There's already a destination with these parameters, check user
        // It will be ok if users match or it was anonymous before
        return !isset($account) || !$existing->uid || $existing->uid == $uid;
      }
      elseif ($address_uid = self::address2uid($type, $address)) {
        // Ok if this address belongs to the same user or to no user 
        return !$address_uid || $address_uid == $uid;
      }
    }
    return TRUE;
  }

  /**
   * Validate address format
   */
  public static function validate_address($type, $address) {
    if (empty($address)) {
      // So far, no method takes an address that evaluates to zero
      return FALSE;
    }
    elseif ($callback = messaging_address_info($type, 'validate callback')) {
      return call_user_func($callback, $address);
    }
    else {
      return TRUE;
    }
  }
  /**
   * Map address to uid
   */
  public static function address2uid($type, $address) {
    if ($type == 'user') {
      return (int)$address;
    }
    elseif ($function = messaging_address_info($type, 'address2uid callback')) {
      return $function($address);
    }    
  }
  
  /**
   * Create from array data
   */
  public static function create($data) {
    // If no more clues, we create it for anonymous user
    $data += array('uid' => 0, 'method' => NULL, 'type' => NULL, 'address' => NULL);
    if ($data['type'] && $data['address']) {
      return self::create_type($data['type'], $data['address'], $data['uid']);
    }
    else {
      return self::create_method($data['method'], $data['address'], $data['uid']);
    }
  }

  /**
   * Create for sending method
   */
  public static function create_method($send_method, $address, $uid) {
    if ($type = messaging_method_info($send_method, 'address_type')) {
      return self::create_type($type, $address, $uid);
    }
  }
  /**
   * Create with parameters
   */
  public static function create_type($type, $address, $uid) {
    if ($existing = self::get_by_address($type, $address)) {
      if ($existing->uid != $uid) {
        $existing->uid = $uid;    
        $existing->save();
      }
      return $existing;
    }
    // Transitional case, row for user with no address, update it
    elseif ($uid && ($existing = self::get(array('uid' => $uid, 'type' => $type, 'address' => '')))) {
      $existing->address = $address;
      $existing->save();
      return $existing;
    }
    else {
      $destination = self::build(array('type' => $type, 'address' => $address, 'uid' => $uid));
      $destination->save();
      return $destination;
    }
  }
  /**
   * Get destination by method and address. This allows advanced caching.
   */
  public static function get_by_address($type, $address) {
    $cached = self::cache_by_address($type, $address);
    if (isset($cached)) {
      return $cached;
    }
    else {
      return self::get(array('type' => $type, 'address' => $address));
    }
  }

  /**
   * Get unique index for this destination
   */
  function index() {
    return $this->uid . ':' . $this->type . ':' . $this->address;
  }

  /**
   * Get address type information
   */
  function address_info($property = NULL) {
    if (!empty($this->type)) {
      return messaging_address_info($this->type, $property);
    }
  }
  /**
   * Get address name
   */
  function address_name() {
    return $this->address_info('name');
  }

  /**
   * Format address
   */
  function format_address($html = FALSE) {
    if ($callback = $this->address_info('format callback')) {
      $address = $callback($this->address, $html);
      return $address ? $address : t('Unknown');
    }
    else {
      return check_plain($this->address);
    }
  }
  /**
   * Get user account
   */
  function get_account() {
    return $this->get_user();
  }
  /**
   * Delete from db and cache
   */
  
  /**
   * Delete messaging destination object/s
   */
  public static function delete_multiple($params) {
    return self::db_query("DELETE", $params);
  }
  // Magic function, format as string
  public function __toString() {
    return 'Destination: ' . $this->index();
  }
  /**
   * Db query for destinations table
   */
  protected static function db_query($sql, $params) {
    $query = _messaging_query_conditions('messaging_destination', $params);
    return db_query($sql . ' FROM {messaging_destination} WHERE ' . $query['where'], $query['args']);
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
   * Save object to cache
   */
  public function cache_save() {
    parent::cache_save();
    $this->cache_by_address($this->type, $this->address, $this);
  }
  /**
   * Save object to cache
   */
  public function cache_delete() {
    parent::cache_delete();
    $this->cache_by_address($this->type, $this->address, FALSE);
  }
  // Store into cache
  public static function cache_set($key, $object) {
    return self::object_cache_set('Messaging_Destination', $key, $object);
  }
  // Get from cache
  public static function cache_get($key) {
    return self::object_cache_get('Messaging_Destination', $key);
  }
  /**
   * Cache get/set by method and address
   */
  public static function cache_by_address($type, $address, $object = NULL) {
    if (isset($object)) {
      return self::cache_set("$type:$address", $object);
    }
    else {
      return self::cache_get("$type:$address");
    }
  }
}
