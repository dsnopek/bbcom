<?php
/**
 * Notifications module. Queue processing.
 * 
 * Callbacks for queue processing. They may be implemented by other modules
 *    _load_user()
 *    _user_allowed()
 *    _process_send()
 * 
 * @ TO DO: Support different languages for message localization
 */

// Number of users to process for each step
define('NOTIFICATIONS_STEP_ROWS', variable_get('notifications_step_rows', 1000));
define('NOTIFICATIONS_STEP_USERS', variable_get('notifications_step_users', 200));

// Minimum amount of seconds the process will need for clean-up tasks
// Just to make sure that after exhausting cron assigned time we'll have a few spare seconds for some cleanup
// Change this time for allowing more cron time to modules running cron after Notifications
define('NOTIFICATIONS_TIME_MARGIN', variable_get('notifications_time_margin', 5));

/**
 * Queue management and processing
 * 
 * Tables used here: notifications_queue, notifications_sent, notifications_event
 *
 */
class Notifications_Queue {
  // Process control variables
  public $process_limit;
  public $process_current;
  public $process_options;
  public $process_max_sqid;
  // Optional language to process only rows in this language. Not used yet.
  // This can be used to run different languages in different crons and prent static caching issues.
  public $process_language = NULL;

  /**
   * Callback for module dependent data
   * 
   * Some data stored in the notifications system is meant to be processed by other modules and
   * this is indicated by a module column in the data.
   * 
   * This function calls the module function if available, defaulting to the notifications provided
   * function when not. The arguments are passed as is
   * 
   * @param $module
   *   Module name
   * @param $function
   *   Function name in module
   */
   protected function process_callback() {
     $args = func_get_args();
     $module = array_shift($args);
     $function = array_shift($args);
     if ($module && function_exists($module .'_notifications_'. $function)) {
       $callback = $module .'_notifications_'. $function;
     }
     else {
       $callback = array($this, $function);
     }
     return call_user_func_array($callback, $args);
   }
   
  /**
   * Callback for Drupal cron
   */
  function process_cron() {
    $this->process_clean();
    $this->process_prepare();
    $this->process_control('cron');
    $this->process_run();
  }

  /**
   * Function to be called on cron by the main notifications_cron
   * 
   * It will call each subscription_process for each interval a number of times
   * 
   * This should send out messages starting with immediate delivery. We send first immediate delivery
   * because the other ones can be added up for each period. 
   * Assumption: The bigger the interval, the longer delay it may admit (?) I.e. sending hourly email 
   * after 1 hour 15 mins may be ok if the system is overloaded.
   * 
   * @return int
   *   Number of rows processed
   */
  function process_run() {
    notifications_log('Starting notifications process');

    $count = 0;
    $stop = FALSE;
    $send_intervals = _notifications_send_intervals();
    unset($send_intervals[-1]);
    
    if ($max_sqid = $this->process_prepare()) {
      foreach ($send_intervals as $interval => $name) {
        notifications_log('Processing queue', array('send interval' => $name));
        while ($rows = $this->process_queue($interval, $max_sqid)) {
          $count += $rows;
          $stop = !$this->process_control('check');
        }
        if ($stop) {
          notifications_log('Process stopped, reached processing limits');
          break;
        } else {
          notifications_log('Process finished', array('send interval' => $name));
        }
      }
    } else {
      notifications_log('No rows in queue');
    }
    $this->process_control('stop');
    return $count;
  }
  
  /**
   * Prepare subscriptions queue
   * 
   * This is intended to avoid race conditions where new rows are added while the process is running
   * 
   * @return
   *   Max $sqid that will be processed this cron
   */
  function process_prepare() {
    if (!isset($this->process_max_sqid)) {
      $this->process_control('start');
      // This will get the latest notification in queue so we don't mess with new ones being created during cron run
      // It will also prevent clashes with the immediate sending feature
      $this->process_max_sqid = db_result(db_query("SELECT max(sqid) FROM {notifications_queue}"));
    }
    return $this->process_max_sqid;
  }
  
  /**
   * Clean up queue and events before starting process
   */
  public static function process_clean() {
    // Clean up expired logs from queue if logging enabled
    if ($keep = variable_get('notifications_log', 0)) {
      db_query("DELETE FROM {notifications_queue} WHERE cron = 0 AND sent < %d", time() - $keep);
    }
    // Clean up event table
    self::event_clean();   
  }
  
  /**
   * Controls and checks limits for queue processing
   * It can be used by other modules to add their own limits here, like number of sms sent, etc...
   * @param $op
   *   'start' => Start the counters
   *   'cron' => Special time adjustment for cron operations
   *   'init' => Start a new counter with $value limit
   *   'option' => Sets /gets options
   *      - debug
   *      - output Enables output for admin page
   * @return
   *   TRUE if we are yet under the processing limits
   */
  function process_control($op = 'check', $name = NULL, $value = NULL) {
    switch ($op) {
      case 'start':
        $this->process_current = array('message' => 0, 'step' => 0, 'timer' => 0, 'row' => 0);
        $defaults = variable_get('notifications_process_limit', array('time' => 0, 'message' => 0, 'row' => 0, 'percent' => MESSAGING_DEFAULT_CRON_PERCENT));
        foreach ($defaults as $name => $value) {
          if ($value && !isset($this->process_limit[$name])) {
            $this->process_limit[$name] = $value;
          }
        }
        timer_start('notifications_process');
        break;
 
      case 'stop':
        $timer = timer_stop('notifications_process');
        $this->process_current['timer'] = $timer['time'];
        return $timer;

      case 'cron':
        // Calculate time limit. We get the smaller of all these times in seconds
        // There's an issue with poormanscron not setting the cron semaphore so it will default to current time
        $timelimit = array();
        $cronstart = variable_get('cron_semaphore', time());     
        // Max execution time may be zero meaning no limit, then no limits based on this
        if ($maxtime = ini_get('max_execution_time')) {
          $timelimit[] =  $cronstart + $maxtime - NOTIFICATIONS_TIME_MARGIN;
          if (!empty($this->process_limit['percent'])) {
            $timelimit[] = time() + $maxtime * $this->process_limit['percent'] / 100;
            unset($this->process_limit['percent']);
          }
        }
        // This is an absolute limit, applies always if set
        if (!empty($this->process_limit['time'])) {
          $timelimit[] = time() + $this->process_limit['time'];
        }
        if ($timelimit) {
          $this->process_limit['time'] = min($timelimit);
        }
        break;

      case 'init':
        $this->process_current[$name] = 0;
        $this->process_limit[$name] = $value;
        break;

      case 'current':
        // Return current value for counter
        return isset($this->process_current[$name]) ? $this->process_current[$name] : 0;

      case 'count':
        $value =  $value ? $value : 1;
        isset($this->process_current[$name]) ? ($this->process_current[$name] += $value) : $this->process_current[$name] = $value;
        break;

      case 'check':
        // Check all limits till we find a false one
        $this->process_current['time'] = time();
        if (isset($this->process_limit)) {
          foreach ($this->process_limit as $name => $value) {
            if ($value && !empty($this->process_current[$name]) && $this->process_current[$name] >= $value) {
              watchdog('notifications', 'Reached processing limit on queue processing: %name = %value', array('%name' => $name, '%value' => $value));
              return FALSE;
            }
          }
        }
        return TRUE;

      case 'results':
        // Return array of variables needed to print out some messages
        return array(
          '@rows' => $this->process_current['row'],
          '@messages' => $this->process_current['message'],
          '@time' => $this->process_current['timer'],
        );
    } 
  }
  
  /**
   * Get / set limit value for counter
   */
  function process_limit($name, $value = NULL) {
    if (isset($value)) {
      $this->process_limit[$name] = $value;
    }
    else {
      return isset($this->process_limit[$name]) ? $this->process_limit[$name] : 0;
    }    
  }
  
  /**
   * Get / set process option
   */
  function process_option($name, $value = NULL) {
    // Get / set value for option
    if (isset($value)) {
      $this->process_options[$name] = $value;
    }
    else {
      return isset($this->process_options[$name]) ? $this->process_options[$name] : FALSE;
    }    
  }

  /**
   * Process rows given query conditions
   * 
   * This is the main notifications queue processing function, fetching queued notifications,
   * loading all related objects and sending out messages.
   * 
   * @see queue_query()
   * 
   * @param $conditions
   *   Array of query conditions
   * @param $limit
   *   Optional, limit the number of rows to process
   * @param $update
   *   Optional, update queue rows and event counter after processing
   * 
   * @return int
   *   Number of rows processed
   */
  function process_rows($conditions, $limit = 0, $update = TRUE) {
    notifications_log('Processing queue rows', $conditions + array('limit' => $limit));
    $test = $this->process_option('test');
    $count = 0;
    // Build query and fetch rows from queue
    $query = $this->queue_query($conditions);
    $sql = "SELECT * FROM {notifications_queue}";
    $sql .= " WHERE ". implode(' AND ', $query['where']);
    $sql .= " ORDER BY module, mdid, send_method, send_interval";
    if ($limit) {
      $result = db_query_range($sql, $query['args'], 0, $limit);
    }
    else {
      $result = db_query($sql, $query['args']);
    }
  
    // Group rows by module, destination, send_interval before composing and sending
    // This loop has to run a final time after all rows have been fetched
    $last = $pending = NULL;
    while (($queue = db_fetch_object($result)) || $pending) {
      if (!$queue || $last && ($queue->module != $last->module || $queue->mdid != $last->mdid || $queue->send_method != $last->send_method || $queue->send_interval != $last->send_interval || $queue->language != $last->language)) {
        // New destination, send if not the first row and reset
        $count += $this->process_group($pending, $update, $last);
        $pending = NULL;
      }
      if ($queue) {
        $this->process_control('count', 'row');
        // Add queue row to pending list
        $pending[$queue->sqid] = $queue; 
      }
      $last = $queue;
    }
    // Done with queue, update event tracker
    if ($update) {
      Notifications_Event::track_update();
    }
    // Return number of rows processed
    return $count;
  }
  
  /**
   * Process queued rows, send messages, etc, etc...
   * 
   * @param $group
   *   Array of queue rows indexed by destination id, send interval, queue id
   */
  protected function process_group($group, $update, $params) {
    $module = $params->module;
    $send_method = $params->send_method;
    $send_interval = $params->send_interval;
    $destination = Messaging_Destination::load($params->mdid);
    $account = $destination ? $destination->get_account() : NULL;
    
    $test = $this->process_option('test');
    $count = 0;
  
    // Start loading needed objects and process all group.
    $subscriptions = $events = $processed = array();
    if (!$destination) {
      notifications_log('Cannot load destination', (array)$params);
    }
    elseif (!$account) {
      notifications_log('Cannot load account', (array)$params);
    }
    elseif ($account->uid && !$account->status) {
      $account = NULL;
      notifications_log('User account blocked', (array)$params);
    }
    // Process every row, but if we don't have destination or account we just do event tracking
    foreach ($group as $sqid => $queue) {
      $count++;
      $processed[] = $sqid;
      $event = Notifications_Event::track_load($queue->eid);
      if ($destination && $account) {
        if (!$event) {
          notifications_log('Cannot load event', (array)$queue);
        }
        elseif (!$event->user_access($account)) {
          notifications_log('Access denied for event', (array)$queue);
        }
        else {
          // This will take care of duplicated events
          $events[$queue->eid] = $event;
          // We keep track also of subscriptions originating this event
          $subscriptions[$queue->eid][] = $queue->sid;           
        }
      }
    }
  
    if ($events) {
      $messages = $this->process_callback($module, 'process_compose', $send_method, $destination, $events, $subscriptions, $send_interval, $params->language);
      notifications_log('Composed messages', array('number' => count($messages), 'send_method' => $send_method));
      // Note that we pass the testing parameter to notifications_process_send
      if ($messages) {
        $this->process_callback($module, 'process_send', $send_method, $destination, $messages, $test);
      }
      if (!$test) {          
        $this->queue_update_sent($destination->mdid, $send_interval, time(), count($messages));
      }
    }
    if ($processed && $update) {
      $this->queue_done(array('sqid' => $processed));        
    }
    return $count;
  }
  
  /**
   * Process subscriptions queue
   * 
   * The subscriptions queue has the following fields
   * sqid, uid, eid, sid, digest
   * 
   * This function should be able of splitting the whole processing in several steps.
   * It will be called multiple times for each send interval
   * 
   * Messages will be processed for each send interval, send_method, user
   * 
   * @param $send_interval
   *   Send interval to process
   * @param $max_sqid
   *   Max queue id to process
   * 
   * @return Number of rows processed
   */
  function process_queue($send_interval, $max_sqid= NULL) {
    $max_sqid = isset($max_sqid) ? $max_sqid : $this->process_prepare();
    $language = $this->process_language;
    
    notifications_log('Starting queue processing', array('send interval' => $send_interval, 'max sqid' => $max_sqid));
    // Option for test running, marking messages as test, nor updating not sending
    $test = $this->process_option('test');
    // Option for normal running but without updating the queue records
    $keep = $this->process_option('keep');

    // Count processed rows
    $count = 0;
    // For scheduled notifications, send just rows after this time
    $send_time = time();
    // This is the time from which stored rows will be sent
    $timelimit = $send_time - $send_interval;

    // Check remaining rows to process to adjust query limits for both users and rows
    $step_users = NOTIFICATIONS_STEP_USERS;
    $step_rows = NOTIFICATIONS_STEP_ROWS;
    if ($row_limit = $this->process_limit('row')) {
      $remaining_rows = $row_limit - $this->process_control('current', 'row');
      if ($remaining_rows > 0) {
        $step_users = min($remaining_rows, $step_users);
        $step_rows = min($remaining_rows, $step_rows);
      }    
    }
    // Common batch parts for processing rows
    $default_batch = array(
      'cron' => 1,
      'max_sqid' => $max_sqid,
      'send_interval' => $send_interval,
      'send_time_after' => $send_time,
    );
    // Get users to process messages for, with this time interval and ordered by squid
    // Order by last sent for this send interval
    // Note: If we get the users with more messages pending first this may save some time
    $sql_select = "SELECT q.mdid, q.send_method, q.module, COUNT(q.sqid) AS count_rows FROM {notifications_queue} q ";
    $sql_select .= " LEFT JOIN {notifications_sent} su ON q.mdid = su.mdid AND q.send_interval = su.send_interval ";
    $sql_select .= " WHERE q.cron = 1 AND q.send_interval = '%d' AND q.send_time < %d AND q.sqid <= %d";
    $sql_select .= " AND (su.mdid IS NULL OR su.sent < %d) ";
    // Note: the group by su.sent seems to be needed by pgsql
    $sql_group = " GROUP BY q.mdid, q.send_method, q.module, su.sent ORDER BY su.sent";
    // If processing by language some things change
    if ($language) {
      $sql_select .= " AND q.language = '%s' ";
      $default_batch['language'] = $language->language;
      $result = db_query_range($sql_select . $sql_group, $send_interval, $send_time, $max_sqid, $timelimit, $language->language, 0, $step_users);
    } else {
      $result = db_query_range($sql_select . $sql_group, $send_interval, $send_time, $max_sqid, $timelimit, 0, $step_users);
    }
    // We create a bach for each mdid (user, destination, method) and handle it to notifications_process_rows()
    while (($queue = db_fetch_object($result)) && $this->process_control('check')) {
      $module = $queue->module;
      $processed = array();    
      // Process all rows for this user. With some hard limit to prevent process lock ups.
      // In case we have too many rows, we go updating step by step
      if ($queue->count_rows > $step_rows) {
        $limit = $step_rows;
        // Still if we want to keep data, we don't update as we go
        $update = !$keep;
      }
      else {
        $limit = $queue->count_rows;
        $update = FALSE;
      }
      // Prepare batch query for actual row processing
      $batch = $default_batch + array(
        'mdid' => $queue->mdid,
        'send_method' => $queue->send_method,
        'module' => $queue->module,   
      );
  
      notifications_log('Queue processing', $batch);
      // These rows may be processed by a different module. Defaults to notifications_process_rows()
      $processed = $this->process_callback($queue->module, 'process_rows', $batch, $limit, $update);
      $count += $processed;
  
      if ($processed && !$test && !$update && !$keep) {
        $this->queue_done($batch);      
      }
    }
  
    // If not doing a test run, update event counter and return count
    // If doing a test run, return 0 so we don't go through this again
    if (!$test && !$keep) {
      Notifications_Event::track_update();
      return $count;
    }
    else {
      return 0;
    }
  }
  
  /**
   * Update user last time sent for each destination / interval
   * 
   * @param $mdid
   *   Destination id
   * @param $interval
   *   Send interval
   * @param $time
   *   Timestamp, when notifications were sent
   * @param $counter
   *   How many messages were sent
   */
  function queue_update_sent($mdid, $interval, $time, $counter = 1) {
    db_query("UPDATE {notifications_sent} SET sent = %d, counter = counter + %d WHERE mdid = %d AND send_interval = '%d'", $time, $counter, $mdid, $interval);
    if (!db_affected_rows()) {
      $this->queue_init_interval($mdid, $interval, $time, $counter);
    }
  }
  
  /**
   * Init sent time when creating/updating a new subscription. 
   * 
   * This is to avoid delayed notifications (i.e. once a week) to be sent right away
   */
  function queue_init_subscription($subscription) {
    $mdid = $subscription->get_destination()->mdid;
    $interval = $subscription->send_interval;
    // We don't need this for 'immediate' notifications
    if ($interval && !db_result(db_query("SELECT sent FROM {notifications_sent} WHERE mdid = %d AND send_interval = %d", $mdid, $interval))) {
      $this->queue_init_interval($mdid, $interval, time());
    }
  }
  
  /**
   * Init user last time sent for destination, interval
   */
  protected function queue_init_interval($mdid, $interval, $time, $counter = 0) {
    db_query("INSERT INTO {notifications_sent}(mdid, send_interval, sent, counter) VALUES(%d, '%d', %d, %d)", $mdid, $interval, $time, $counter);
  }
  
  
  /**
   * Message composition.
   * 
   * Processes everything, included templating and digestion and sends message/s.
   * 
   * Adds some more information into $message['notifications'] that may be used by other modules
   *
   * @param $destination
   *   User account to send the notification to
   * @param $events
   *   Array of loaded event objects to be processed
   * @param $subscriptions
   *   Array of arrays of subscription ids (sids) for each event(eid)
   * 
   * @return array()
   *   Array of messages ready for sending out
   */
  function process_compose($send_method, $destination, $events, $subscriptions, $send_interval, $langcode = NULL, $module = 'notifications') {
    notifications_log('Processing for sending', array('method' => $send_method, 'interval' => $send_interval, 'module' => $module, 'events' => count($events)));
    // Build message template that will be used for all produced messages
    $template = $this->message_template(array(
      'language' => $langcode,
      'method' => $send_method,
      'send_interval' => $send_interval,
    ));
    $template->set_destination($destination);

    
    // Find build method for this interval. Check the function is there in case some module has been disabld
    $build_method = $this->build_method($template, array('events' => $events, 'subscriptions' => $subscriptions, 'module' => $module));
    if ($build_method && !empty($build_method['build callback'])) {
      $build_function = $build_method['build callback'];
      $template->digest = !empty($build_method['digest']);
      $template->build_method = $build_method['type'];
    }
    else {
      // Default building function
      $build_function = array($template, 'build_simple');
      $template->build_method = 'simple';
    }
  
    // Invoke building function that will return an array of messages  
    $messages = call_user_func($build_function, $template, $events, $subscriptions, $module);
    return $messages;
  }
  
  /**
   * Get message template for notifications
   * 
   * This is a hidden variable that can be overridden to use a different class: notifications_message_template
   */
  public static function message_template($params = NULL) {
    $class = variable_get('notifications_message_template', 'Notifications_Message');
    return new $class($params);
  }
  
  /**
   * Information about digesting method for composing a message
   * 
   * This just calls notifications_process_build_method() with the send_interval, though its purpose 
   * is just to allow overriding and finding a suitable build method having all the information.
   * 
   * @param $template
   *   Message template
   * @param $params
   *   Other parameters for these message: 'events', 'subscriptions', 'module'
   * 
   * @return array()
   *   Ditest information for that interval, or all the information if no interval
   */
  function build_method($template, $params = array()) {
    return notifications_build_method($template->send_interval);
  }
  
  /**
   * Send array of messages through messaging module
   * 
   * @param $destination
   *   Messaging destination to send to
   * @param $messages
   *   Array of messages prepared for sending
   * @param $test
   *   Optional just test composition and formating but do not send
   */
  protected function process_send($method, $destination, $messages) {
    notifications_log('Sending out notifications', array('method' => $method, 'address' => $destination->address, 'messages' => count($messages)));
    $test = $this->process_option('test');
    foreach ($messages as $message) {      
      $this->message_send($method, $destination, $message, $test);
    }
    return $messages;
  }
  
  /**** Retrieving and replacing text parts, interfacing with tokens and messaging module ****/
  
  /**
   * Message sending, pass the message to Messaging back end
   * 
   * @param $account
   *   User account to send the message to
   * @param $message
   *   Message array, will be converted to object
   * @param $send_method
   *   Send method
   * @param $test
   *   Optional, set to TRUE if doing a test run (messages not to be actually sent)
   * 
   * @return boolean
   *   TRUE if sending was successfull
   */
  protected function message_send($method, $destination, $message, $test = FALSE) {
    notifications_debug('Preparing user notification for messaging', array('message' => $message, 'destination' => $destination));
    $message = messaging_message_build($message);
    $message->type = 'notifications';
    $message->test = $test;
    $this->process_control('count', 'message');
    return messaging_message_send_destination($method, $destination, $message);
  }
  
  /**
   * Save item straight to the queue
   */
  function queue_item($item) {
    return drupal_write_record('notifications_queue', $item);
  }

  /**
   * Mark queue rows as done
   * 
   * Either log, if logging enabled, or delete
   */
  function queue_done($params) {
    if (variable_get('notifications_log', 0)) {
      $this->queue_update($params, array('cron' => 0, 'sent' => time()));
    } else {
      $this->queue_delete($params);
    }
  }
  
  /**
   * Update queue rows with defined values
   * 
   * @arg $params
   *   Parameters to select the queue rows for updating. Array of field => value pairs
   * @arg $update
   *   Fields values to update. Array of field => value pairs
   */
  public static function queue_update($params, $updates) {
    $values =  _messaging_query_conditions('notifications_queue', $updates);
    $where = self::queue_query($params);
    $args = array_merge($values['args'], $where['args']);
    return db_query('UPDATE {notifications_queue} SET ' . implode(', ', $values['conditions']) . ' WHERE ' . implode(' AND ', $where['where']), $args);
  }
  
  /**
   * Delete rows from subscriptions queue
   * 
   * Note: Handle with care if wrong params it may delete all rows
   * 
   * @param $params
   *   Array of conditions. If none, all rows for disabled subscriptions will be deleted
   */
  function queue_delete($params) {
    if ($params) {
      $query = $this->queue_query($params);
      db_query("DELETE FROM {notifications_queue} WHERE ". implode(' AND ', $query['where']), $query['args']);
    }
    else {
      // Delete all queued notifications for subscriptions not active
      // Note queue rows without subscription will be kept (like the ones form notifications_lite)
      db_query("DELETE FROM {notifications_queue} WHERE sid IN (SELECT sid FROM {notifications} WHERE status <> %d)", NOTITICATIONS_SUBSCRIPTION_ACTIVE);
    }
    return db_affected_rows();
  }
  
  /**
   * Clean queue for a user and update event tracker
   * 
   * @param $params
   *   Array of conditions. If none, all rows for disabled subscriptions will be deleted
   */
  function queue_clean($params) {
    $this->queue_delete($params);
    $this->event_clean(TRUE);
  }

  /**
   * Reset queue, clean everything
   */
  public static function queue_reset() {
    db_query("DELETE FROM {notifications_queue}");
    db_query("DELETE FROM {notifications_event}");
  }
  
  /**
   * Count the number of rows in queue
   */
  public static function queue_count($params = NULL) {
    if ($params) {
      $query = self::queue_query($params);
      return db_result(db_query('SELECT COUNT(*) FROM {notifications_queue} WHERE ' . implode(' AND ', $query['where']), $query['args']));
    } else {
      return db_result(db_query('SELECT COUNT(*) FROM {notifications_queue}'));
    }
  }
  
  /**
   * Get queue status count grouping by some fields
   */
  public static function queue_status($fields) {
    $field_names = implode(', ', $fields);
    $result = db_query("SELECT $field_names, count(*) AS count FROM {notifications_queue} WHERE cron = 1 GROUP BY $field_names");    
    // Get all rows and compile data
    $data = array();
    $count = 0;
    while ($item = db_fetch_object($result)) {
      $data[] = $item;
    }
    return $data;
  }

  /**
   * Queue events for notifications adding query conditions from plug-ins
   * 
   * This is an example of the resulting query
   *
   * INSERT INTO {notifications_queue} (uid, sid, module, eid, send_interval, send_method, cron, created, conditions)
   * SELECT DISTINCT s.uid, s.sid, s.module, 34, s.send_interval, s.send_method, s.cron, 1230578161, s.conditions FROM notifications s 
   * INNER JOIN notifications_fields f ON s.sid = f.sid 
   * WHERE s.status = 1 AND s.event_type = 'node' AND s.send_interval >= 0 
   * AND ((f.field = 'nid' AND f.value = '2') OR (f.field = 'type' AND f.value = 'story') OR (f.field = 'author' AND f.value = '1'))
   * GROUP BY s.uid, s.sid, s.module, s.send_interval, s.send_method, s.cron, s.conditions 
   * HAVING s.conditions = count(f.sid)
   *
   * @param $event
   *   Event object
   * 
   * @return int
   *   Number of queued rows
   */
  function queue_event($event) {
    notifications_include('object.inc');
    notifications_include('query.inc');
  
    $query = array();
    // Build big insert query using the query builder. The fields for this event type will be added by the plug-ins. 
    // If no arguments retrieved, skip this step
    if ($query_args = module_invoke_all('notifications_event', 'query', $event)) {
      // Build a query skeleton and add parameters for each module separately
      $query = notifications_query_event_queue($event);
      foreach ($query_args as $query_params) {
        $query = notifications_query_build($query_params, $query);
      }
      
      // Give a chance to other modules to alter the query or empty it so we don't throw it
      drupal_alter('notifications_query', $query, $event);
      
      // Finally we build the SELECT part of the query and glue it to the INSERT
      if ($query) {
        list($sql, $args) = notifications_query_sql($query);
        db_query($sql, $args);
      }
    }
    
    // Return number of queued rows
    return db_result(db_query('SELECT COUNT(*) FROM {notifications_queue} WHERE eid = %d', $event->eid));
  }
  
  /**
   * Clean up event table. Drop expired events for which we don't have pending rows.
   * 
   * @param $update
   *   Update event counter
   */
  public static function event_clean($update = FALSE) {
    // This expiretime will prevent some race condition that occurs when the event is saved but the subs queue not yet populated  
    $expiretime = time() - 60; 
    if ($update) {
      // Update event counter, which keeps the number of notifications pending for each event
      db_query("UPDATE {notifications_event} e SET counter = (SELECT COUNT(*) FROM {notifications_queue} q WHERE q.eid = e.eid ) WHERE e.created < %d", $expiretime);
    }
    db_query("DELETE FROM {notifications_event} WHERE counter = 0 AND created < %d", $expiretime);
    // Delete events with no pending notifications. As events are created sequentially, we use this fact to speed up the query
    db_query("DELETE FROM {notifications_event} WHERE created < %d AND eid < (SELECT MIN(eid) FROM {notifications_queue})", $expiretime);
  }
  
  /**
   * Build query conditions for queue queries
   * 
   * @param $params
   *   Array of parameters, field => value form
   *   Special parameters
   *     'max_squid' => max squid to delete
   *     'rows' => array of squid values to delte
   * @return
   *   Array with 'where' and 'args' elements. Each of them is an array
   */
  public static function queue_query($params, $table_alias = NULL) {
    $where = $args = array();
    
    // Special condition max_sqid
    if (isset($params['max_sqid'])) {
      $where[] = $table_alias ? "$table_alias.sqid <= %d" : "sqid <= %d";
      $args[] = $params['max_sqid'];
      unset ($params['max_sqid']);
    }
    // Special condition send_time_after
    if (isset($params['send_time_after'])) {
      $where[] = $table_alias ? "$table_alias.send_time < %d" : "send_time < %d";
      $args[] = $params['send_time_after'];
      unset ($params['send_time_after']);
    }  
    // User generic query builder for the rest of fields
    $values =  _messaging_query_conditions('notifications_queue', $params, $table_alias);  
    $where = array_merge($where, $values['conditions']);
    $args = array_merge($args, $values['args']);
   
    return array('where' => $where, 'args' => $args);
  }
}