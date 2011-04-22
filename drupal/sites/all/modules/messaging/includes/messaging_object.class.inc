<?php

/**
 * @file
 * Drupal Messaging Framework - Base classes
 */

/**
 * Messaging user's object
 * 
 * This will be the base class for all objects that:
 * - Have a user account associated
 * - Have language
 */
abstract class Messaging_User_Object {
  // User id and user object
  public $uid;
  protected $_user;
  // Optional user name, when we are loading multiple rows ready for display
  public $user_name;
 
  // Language and language object
  public $language;
  protected $_language;

  /**
   * Constructor, with predefined array of data
   */ 
  public function __construct($template = NULL) {
    if ($template) {
      foreach ($template as $key => $value) {
        $this->$key = $value;
      }
    }
    // Make sure all objects have language
    if (!isset($this->language)) {
      $this->set_language();
    }
  }
  
  /**
   * Magic function. Set protected properties
   *
   * We have a problem when creating records. Serial fields are unset() by drupal_write_record
   * For these fields, though they're public, when there's a __set function it is called too
   */
  public function __set($name, $value) {
    if ($name == 'uid' || $name == 'language') {
      $this->$name = $value;
    }
  }

  /**
   * Get destination user
   */
  public function get_user() {
    if (!isset($this->_user) && isset($this->uid)) {
      $this->_user = messaging_load_user($this->uid);
    }
    return isset($this->_user) ? $this->_user : NULL;
  }

  /**
   * Get destination user name, either from user object or from a user_name property.
   * 
   * When loading a listing of objects, we can save some queries by setting the user_name property
   */
  public function get_user_name() {
    if (!empty($this->_user)) {
      $user = $this->_user;
    }
    elseif (!empty($this->user_name)) {
      $user = new Stdclass();
      $user->uid = $this->uid;
      $user->name = $this->user_name;
    }
    else {
      // Last resource, we do user loading
      $user = $this->get_user();
    }
    return $user ? theme('username', $user) : t('none');
  }

  /**
   * Set user account
   * 
   * @param $account
   *   User account or user id
   */
  public function set_user($account) {
    if (is_numeric($account)) {
      $this->uid = $account;
      $this->_user = NULL;
    }
    elseif (is_object($account)) {
      $this->uid = $account->uid;
      $this->_user = $account;
    }
    else {
      $this->uid = 0;
      $this->_user = $account;   
    }
  }

  /**
   * Get language object
   */
  public function get_language() {
    if (!isset($this->_language)) {
      $this->set_language();
    }
    return $this->_language;
  }

  /**
   * Set language object / property
   */
  public function set_language($language = NULL) {
    if ($language) {
      $this->language = $language->language;
      $this->_language = $language;
    }
    elseif (!empty($this->language) && ($languages = language_list()) && isset($languages[$this->language])) {
      $this->_language = $languages[$this->language];
    }
    elseif ($account = $this->get_user()) {
      // Fallback to account language
      return $this->set_language(user_preferred_language($account));
    }
    else {
      // Fallback to default language
      return $this->set_language(language_default());
    }
  }
}

/**
 * Messaging persistent object
 * 
 * This will be the base class for all objects that:
 * - Are persistent (db storage)
 * - Have a user account associated and language (Messaging_User_Object)
 */
abstract class Messaging_Object extends Messaging_User_Object {
  // Database properties
  const DB_TABLE = '';
  const DB_KEY = '';

  // Deletion mark. Deleted objects will keep their id, may be needed to remove related data, but will be marked
  protected $deleted = FALSE;

  /**
   * Constructor, with predefined array of data
   */ 
  public function __construct($template = NULL) {
    if ($template) {
      foreach ($template as $key => $value) {
        $this->$key = $value;
      }
    }
    // If loaded from db, may need unserialize
    if ($this->key()) {
      $this->unserialize();
    }
    // Make sure all objects have language
    if (!isset($this->language)) {
      $this->set_language();
    }
  }
  /**
   * Build object from template
   */
  public static function object_build($template, $class) {
    if (is_object($template) && is_a($template, $class)) {
      return $template;
    }
    else {
      // This seems to be the only way of invoking the build method of the class
      return call_user_func(array($class, 'build'), $template);
    }
  }

  /**
   * Get unike id key
   */
  function key() {
    return isset ($this->{$this->db_key()}) ? $this->{$this->db_key()} : NULL;
  }
  /**
   * Save to database
   */
  public function save() {
    if ($this->key()) {
      return $this->update();
    }
    else {
      return $this->insert(); 
    }
  }
  /**
   * Create object in database
   */
  public function insert() {
    return drupal_write_record($this->db_table(), $this);
  }
  /**
   * Update object in database
   */
  public function update() {
    return drupal_write_record($this->db_table(), $this, $this->db_key());
  }
  /**
   * Delete from system database
   */ 
  public function delete() {
    if ($this->key()) {
      db_query('DELETE FROM {' . $this->db_table() . '} WHERE ' . $this->db_key() . ' = %d', $this->key());
      //unset($this->{$this->db_key()});
      $this->deleted = TRUE;
    }
  }
  /**
   * Check whether this object is an instance or just a template
   */
  public function is_instance() {
    return !$this->deleted && $this->key();
  }

  /**
   * Load object by unique key, no static caching
   */
  public static function object_load($table, $key, $value, $class = NULL) {
    if ($object = self::db_load($table, $key, $value)) {
      return $class ? self::object_build($object, $class) : $object;
    }
  }
  /**
   * Unserialize after loading. It does nothing but can be overridden
   */
  public function unserialize() {
  }
  /**
   * Load object from DB
   */
  public static function db_load($table, $key, $value) {
    return db_fetch_object(db_query("SELECT * FROM {" . $table . "} WHERE $key = %d", $value));
  }

  /**
   * Magic function. Set protected properties
   *
   * We have a problem when creating records. Serial fields are unset() by drupal_write_record
   * For these fields, though they're public, when there's a __set function it is called too
   */
  public function __set($name, $value) {
    if ($name == $this->db_key()) {
      $this->$name = $value;
    }
    else {
      parent::__set($name, $value);
    }
  }
  // Get table name for storage
  abstract public static function db_table();
  // Get key field name for storage
  abstract public static function db_key();
  // Load from key field
  abstract public static function load($key);
  // Build object form template
  abstract public static function build($template);

}

/**
 * Class with static caching
 */
abstract class Messaging_Cached_Object extends Messaging_Object {
  /**
   * Load object by unique key, may have static caching 
   */
  public static function object_load($table, $key, $value, $class) {
    $cached = self::object_cache_get($class, $value);
    if (isset($cached)) {
      return $cached;
    }
    elseif ($object = parent::object_load($table, $key, $value, $class)) {
      $object->cache_save();
      return $object;
    }
    else {
      self::object_cache_set($class, $value, FALSE);
    }
  }
  /**
   *   /**
   * Create object in database
   */
  public function insert() {
    $result = parent::insert();
    $this->cache_save();
    return $result;
  }
    /**
   * Delete from system and from static cache
   */ 
  function delete() {
    if ($this->key()) {
      $this->cache_delete();
      return parent::delete();
    }
  }
  /**
   * Save object to cache
   */
  public function cache_save() {
    $this->cache_set($this->key(), $this);
  }
  /**
   * Delete object from cache
   */
  public function cache_delete() {
    $this->cache_set($this->key(), FALSE);
  }
  
  /**
   * Set value into static cache
   */
  protected static function object_cache_set($class_name, $key, $value) {
    return messaging_static_cache_set($class_name, $key, $value);
  }
  /**
   * Get value from static cache
   */
  protected static function object_cache_get($class_name, $key) {
    return messaging_static_cache_get($class_name, $key);
  }
  
  abstract public static function cache_set($key, $object);
  abstract public static function cache_get($key);
}

/**
 * Dummy class, that takes any method and does nothing
 * 
 * @see messaging_store()
 */
class Messaging_None {
   public function __call($name, $arguments) {}
   public static function __callStatic ($name , $arguments) {}
}
