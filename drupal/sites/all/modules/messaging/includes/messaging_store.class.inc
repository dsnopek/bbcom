<?php
/**
 * @file
 *   Database storage for the messaging framework
 */

// Max number of rows to process for each step before clean up
define('MESSAGING_STEP_ROWS', variable_get('messaging_step_rows', 1000));

// Minimum amount of seconds the process will need for clean-up tasks
// Just to make sure that after exhausting cron assigned time we'll have a few spare seconds for some cleanup
define('MESSAGING_TIME_MARGIN', variable_get('messaging_time_margin', 5));

/**
 * Default storage and queueing system for Messaging
 * 
 * This class has only static methods that will be invoked through 'messaing_store' function
 */
class Messaging_Store {
  const STEP_ROWS = MESSAGING_STEP_ROWS;
  const TIME_MARGIN = MESSAGING_TIME_MARGIN;
  // Storage parameters
  const DB_TABLE = 'messaging_store';
  const DB_KEY = 'mqid';
  
  /**
   * Capabilities: whether this Queue can expire old messages
   */
  public static function can_expire() {
    return TRUE;
  }
  /**
   * Process messages on cron
   */
  public static function cron_process() {
    // Pass on a time out condition, that will be based on 'max_execution_time'
    $limit['timeout'] = variable_get('cron_semaphore', 0) + ini_get('max_execution_time') - self::TIME_MARGIN;
    self::queue_process($limit);
    self::queue_expire_messages();
    self::queue_expire_logs();
  }

  /**
   * Process and send messages in queue, to be called from cron
   * 
   * It will check for predefined limits and repeat the cycle
   *   [fetch] -> [send] -> [check]
   * until the queue is empty or any of the limits are met
   * 
   * The limits array may contain any of these conditions:
   * - time, absolute max execution time
   * - timeout, calculated time out (like for cron, based on the time we've been already running)
   * - message, max number of messages sent
   * - percent, max % of page execution time that can be spent on cron processing
   * 
   * @param $limits
   *   Optional limits for queue processing
   * @return
   *   Array of results indexed by message id
   */
  public static function queue_process($limits = array()) {
    $results = array();
    $limit = self::process_limits($limits);
    
    // Processing loop. Will stop when we run out of rows or reach time / messages limit
    $count = 0;
    $max = !empty($limit['message']) ? $limit['message'] : 0;
    do {
      $step = $max ? min(self::STEP_ROWS, $max - $count) : self::STEP_ROWS;
      $result = self::queue_process_step($step, $limit['timeout']);
      $number = count($result);
      $count += $number;
      $results = array_merge($results, $result);
    } 
    while ($number == $step && (!$limit['timeout'] || time() <= $limit['timeout']) && (!$max || $max > $count));
  
    return $results;
  }
  
  /**
   * Calculate limits for queue processing
   */
  public static function process_limits($limits = array()) {
    $limits += variable_get('messaging_process_limit', array('message' => 0, 'time' => 0, 'percent' => MESSAGING_DEFAULT_CRON_PERCENT));
    // Calculate time limit. We get the smaller of all these times in seconds
    if (!empty($limit['timeout'])) {
      $times[] = $limit['timeout'];
    }
    if (!empty($limit['time'])) {
      $times[] = time() + $limit['time'];
    }
    if (!empty($limit['percent'])) {
      $times[] = time() + ini_get('max_execution_time') * $limit['percent'] / 100;
    }
    $limits['timeout'] = !empty($times) ? min($times) : 0; 
    return $limits;  
  }

  /**
   * Retrieve and send queued messages
   * 
   * @param $limit
   *   Maximum number of queued messages to process for this step
   * @param $timeout
   *   Optional time limit for processing, will return when if reached during processing
   * @return
   *   Array of sending results indexed by message id
   */
  protected static function queue_process_step($limit, $timeout = 0) {
    $count = 0;
    $sent = $unsent = $processed = array();
    $result = self::select_query('*', array('queue' => 1, 'cron' => 1), array('order' => array(self::DB_KEY), 'limit' => $limit));
    while ($object = db_fetch_object($result)) {
      $message = self::message_unpack($object, TRUE);
      $success = self::queue_process_message($message);
      $processed[$message->mqid] = $success;
      if ($success) {
        $sent[] = $message->mqid;
        messaging_debug('Processed message from queue', array('message' => $message, 'success' => $success));
      } else {
        $unsent[] = $message->mqid;
        watchdog('messaging', 'Failed queue processing for @message', array('@message' => (string)$message), WATCHDOG_WARNING);
      }
      $count++;
      
      // Check timeout after each message
      if ($timeout && time() > $timeout) break;
    }
    if ($sent) {
      self::message_sent($sent);
    }
    if ($unsent) {
      self::message_sent($unsent, TRUE);
    }
    return $processed;
  }
  
  /**
   * Process single messag from queue
   */
  protected static function queue_process_message($message) {
    // Do not queue again but send out
    $message->queue = 0;
    if (!empty($message->destinations)) {
      $success = $message->send_multiple();
    }
    else {
      $success = $message->send();
    }
    return $success; 
  }

  /**
   * Queue clean up
   * - Remove expired logs (include errors)
   * - Remove expired queued messages
   */
  public static function queue_clean() {
    $count = self::queue_expire_messages();
    $count += self::queue_expire_logs(TRUE);
    return $count;
  }
  
  /**
   * Count queued messages
   */
  public static function queue_count() {
    return db_result(self::select_query('COUNT(*)', array('queue' => 1, 'cron' => 1)));
  }

  /**
   * Remove expired logs from queue
   * 
   * @param $force
   *   Whether to force removal of logs and errors (even if logging not set)
   */
  public static function queue_expire_logs($force = FALSE) {
    if (($expire_logs = variable_get('messaging_log', 0)) || $force) {
      $time = time() - $expire_logs;  
      db_query('DELETE FROM {' . self::DB_TABLE . '} WHERE log = 1 AND queue = 0 AND sent < %d OR error = 1 AND created < %d', $time, $time);
      return db_affected_rows();
    }
    else {
      return 0;
    }
  }

  /**
   * Remove expired messages from queue
   */
  public static function queue_expire_messages() {
    if ($expire_messages = variable_get('messaging_queue_expire', 0)) {
      $time = time() - $expire_messages;
      db_query('DELETE FROM {' . self::DB_TABLE . '} WHERE created < %d', $time);
      return db_affected_rows();
    }
    else {
      return 0;
    }
  }

  /**
   * Retrieve from messaging database storage
   * 
   * @param $params
   *   Array of field value pairs
   * @param $order
   *   Optional array of field names to order by
   * @param $limit
   *   Optional maximum number of rows to retrieve
   * @param $pager
   *   Optional pager element for pager queries
   * @param $full
   *   Wehther to fully unpack the message and load related objects
   */
  public static function get_messages($params, $order = NULL, $limit = NULL, $pager = NULL, $full = TRUE) {
    $messages = array();
    $result = self::select_query('*', $params, array('limit' => $limit, 'order' => $order, 'pager' => $pager));
    while ($msg = db_fetch_object($result)) {
      $messages[$msg->mqid] = self::message_unpack($msg, $full);
    }
    return $messages;
  }
  
  /**
   * Build select query from main store table
   */
  protected static function query_select($fields = '*', $conditions = NULL, $query_params = array()) {
    $fields = is_array($fields) ? $fields : array($fields);
    $query['select'] = $fields;
    $query['from'][] = '{' . self::DB_TABLE . '}';
    if ($conditions) {
      $query += self::query_fields($conditions);
    }
    $query += $query_params;
    return $query;
  }

  /**
   * Run select query with given parameters
   * 
   * @param $fields
   *   Array of field names for SELECT
   * @param $conditions
   *   Array of query conditions
   * @param $query_params
   *   Mixed query parameters (limit, pager, group....)
   */
  public static function select_query($fields = '*', $conditions = NULL, $query_params = array()) {
    $query = self::query_select($fields, $conditions, $query_params);
    return messaging_query_sql($query, TRUE);
  }
  /**
   * Get status summary
   * 
   * @param $fields
   *   Fields to queue and group by
   * @param $conditions
   *   Array of field conditions to restrict the query
   *   
   * @return array
   *   Array of arrays with the status fields and a 'total' counter for each row
   */
  public static function get_status($fields, $conditions = array()) {
    $status = array();
    $group = $fields;
    $fields[] = 'count(*) AS total';
    $result = self::select_query($fields, $conditions, array('group' => $group));  
    while ($data = db_fetch_array($result)) {
      $status[] = $data;
    }
    return $status;
  }

  /**
   * Load single message from store / static cache
   */
  public static function message_load($mqid, $refresh = FALSE) {
    if (!$refresh) {
      $cached = self::cache_get($mqid);
    }
    if (isset($cached)) {
      return $cached;
    } 
    else {
      $message = self::_message_load($mqid);
      self::cache_set($mqid, $message ? $message : FALSE);
      return $message;
    }
  }
  
  /**
   * Load single message from store. No static cache.
   */
  protected static function _message_load($key) {
    if ($message = db_fetch_object(self::select_query('*', array(self::DB_KEY => $key), array('limit' => 1)))) {
      return self::message_unpack($message, TRUE);
    }
  }
  
  /**
   * Build parameters for where clause
   * 
   * @param $fields
   *   Array of field conditions (name => value/s)
   */
  protected static function query_fields($fields) {
    $query =  _messaging_query_where(self::DB_TABLE, $fields);
    // Handle special case 'max_mqid'
    if (isset($fields['max_mqid'])) {
      $query['where'][] = 'mqid <= %d';
      $query['args'][] = $fields['max_mqid'];
    }
    return $query;
  }
  /**
   * Unpack stored messages
   * 
   * @param $message
   *   Array as retrieved from the db store
   * @param $full
   *   True for loading the account data if this message is intended for a user
   *   And loading the file objects associated too
   */
  protected static function message_unpack($message, $full = FALSE) {
    // Unserialize stored parameters
    if ($message->params) {
      $message->params = unserialize($message->params);
    }
    // Unserialize data field
    drupal_unpack($message);
    // Saved messages are prepared and rendered
    $message->prepared = $message->rendered = TRUE;
    // And they may be already marked for queue / log
    $message->queued = $message->queue;
    $message->logged = $message->log;
    return messaging_message_build($message);
  }
  
  /**
   * Mark messages as sent, either deleting them, or keeping logs
   * 
   * @param $mqid
   *   Single message id or array of message ids
   * @param $error
   *   Optional, just mark as error move queue messages to log, for messages on which sending failed 
   */
  public static function message_sent($mqid, $error = FALSE) {
    $mqid = is_array($mqid) ? $mqid : array($mqid);
    $where = self::query_fields(array('mqid' => $mqid));
    if ($error) {
      // Error, log them all, sent = 0
      $sent = 0;
    } else {
      // First delete the ones that are not for logging, then mark as sent
      db_query("DELETE FROM {messaging_store} WHERE log = 0 AND ".implode(' AND ', $where['where']) , $where['args']);
      $sent = time();
    }
  
    // Now unmark the rest for queue processing, as logs
    $args = array_merge(array($sent), $where['args']);
    db_query("UPDATE {messaging_store} SET queue = 0, cron = 0, log = 1, sent = %d WHERE ".implode(' AND ', $where['where']), $args);
  }
  
  /**
   * Delete multiple messages from queue
   */
  public static function delete_multiple($params) {
    $where = self::query_fields($params);
    db_query('DELETE FROM {' . self::DB_TABLE . '} WHERE '.implode(' AND ', $where['where']), $where['args']);
  }
 
  /**
   * Delete all message from queue and logs
   */
  public static function delete_all() {
    db_query('DELETE FROM {' . self::DB_TABLE . '}');
    return db_affected_rows();
  }

  /**
   * Delete single message from store
   */
  public static function message_delete($message) {
    $mqid = self::message_key($message);
    self::cache_set($mqid, FALSE);
    db_query('DELETE FROM {' . self::DB_TABLE . '} WHERE ' . self::DB_KEY . ' = %d', $mqid);
  }
  
  /**
   * Get storage key from message
   */
  protected static function message_key($message) {
    return is_object($message) ? $message->mqid : $message;
  }
  /**
   * Put into database storage, create one line for each destination
   * 
   * If there are many destinations they will be stored as 'multiple'
   * 
   * @param $message
   *   Message object
   * @return int
   *   Result from drupal_write_record: SAVED_NEW, SAVED_UPDATED, FALSE
   */
  public static function message_save($message) {
    // Check we have a message object
    $message = messaging_message_build($message);
    self::message_prepare($message);
    $update = empty($message->mqid) ? array() : 'mqid';
    $result = drupal_write_record(self::DB_TABLE, $message, $update);
    if ($result) {
      // For this store, queue and log are the same so we mark the messages accordingly
      $message->updated = FALSE;
      $message->queued = $message->queue;
      $message->logged = $message->log;
      // Store it into the cache for further reference
      self::cache_set(self::message_key($message), $message);
    }
    messaging_debug('Saved message to store', array('message' => $message));
    return $result;
  }

  /**
   * Log message for further reference
   */
  public static function message_log($message) {
    $message->log = 1;
    $message->queue = $message->cron = 0;
    return self::message_save($message);
  }

  /**
   * Queue message for next delivery
   */
  public static function message_queue($message) {
    $message->queue = 1;
    return self::message_save($message);
  }
  /**
   * Prepare for storage
   */
  protected static function message_prepare($message) {
    // On this store we can just save messages prepared and rendered
    $message->prepare();
    $message->render();
  
    // Set created date if not previously set
    if (empty($message->created)) {
      $message->created = time();
    }
 
    // Normalize some values. Boolean parameters must be 0|1
    foreach (array('queue', 'log', 'cron') as $field) {
      $message->$field = empty($message->$field) ? 0 : 1;
    }
    // Add fields to serialize. All persistent properties that are not in the schema
    $schema = drupal_get_schema(self::DB_TABLE);
    $serialize_fields = array_diff($message->data_fields(), array_keys($schema['fields'])); 
    foreach ($serialize_fields as $field) {
      if (isset($message->$field)) {
        $data[$field] = $message->$field;
      }
    }
    $message->data = !empty($data) ? $data : NULL;
  }
  
  /**
   * Save message to static cache
   */
  protected static function cache_set($key, $value) {
    messaging_static_cache_set('messaging_store', $key, $value);
  }
  /**
   * Get message from static cache
   */
  protected static function cache_get($key) {
    return messaging_static_cache_get('messaging_store', $key);
  }
  
  /**
   * Get help for admin pages
   */
  public static function admin_help() {
    return array(
      'name' => t('Messaging Store (built-in)'),
      'queue' => t('Queued messages will be processed on cron.'),
    );
  }

  /**
   * Get more settings for Admin page
   */
  public static function admin_settings() {
  }
}