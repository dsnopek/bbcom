<?php
/**
 * @file
 * Notifications query API - Helper functions to build monster queries
 */

/**
 * Query builder for subscriptions
 * 
 * This adds up query elements into a big array so they can be later rendered as SQL
 * 
 * @see notifications_query_sql()
 * 
 * Both the $params and the $query can have some special values
 *   - fields, array of field => value conditions
 *   - fields args, array of field arguments, to be added into the query after 'where args'
 *   - fields sql, sql condition for fields to be added as is
 *   - fields index, if present, current index to alias and join the fields table
 * @param $params
 *   Array of conditions to add.
 *   
 * @param $query
 *   Base query to build upon
 */
function notifications_query_build($params, $query = array()) {
  foreach ($params as $name => $elements) {
    if ($name == 'fields') {
      // Fields elements have some special handling, they have the form: field => value
      foreach ($elements as $field => $value ) {
        if (is_numeric($field)) {
          $field = $value['field'];
          $value = $value['value'];
        }
        // Use field definition provided by hook_notifications('subscription fields') and handle array values with IN conditions
        // Workaround to have a valid one because not all modules provide the information yet (og?)
        if (notifications_subscription_fields($field, 'type') == 'int') {
          $type = 'int';
          $fieldval = 'intval';
        }
        else {
          $type = 'char';
          $fieldval = 'value';
        }
        if (isset($query['fields index'])) {
          $alias = 'f' . $query['fields index']++;
          $query['join'][] = "INNER JOIN {notifications_fields} $alias ON $alias.sid = s.sid";
        }
        else {
          $alias = 'f';
        }
        if (is_array($value)) {
          $query['fields'][] = "$alias.field = '%s' AND $alias.$fieldval IN (". db_placeholders($value, $type) .")";
          $query['fields args'][] = $field;
          $query['fields args'] = empty($query['fields args']) ? $value : array_merge($query['fields args'], $value);
        }
        else {
          $query['fields'][] = "$alias.field = '%s' AND $alias.$fieldval = " . db_type_placeholder($type);
          $query['fields args'][] = $field;
          $query['fields args'][] = $value;
        }
      }
    }
    else {
      if ($name == 'fields sql') {
        // These are added as 'fields' parameters without further parsing
        $name = 'fields';
      }
      if (is_array($elements)) {
        $query[$name] = empty($query[$name]) ? $elements : array_merge($query[$name], $elements);
      }
      else {
        $query[$name][] = $elements;
      }
    }
  }
 
  return $query;
}

/**
 * Build the SQL statement from query elements
 * 
 * It will build INSERT + SELECT or SELECT queries from its elements
 * 
 * This is a wrapper around messaging_query_sql() that will handle special 'fields' parameters
 * 
 * @return array()
 *   list($sql, $args);
 */
function notifications_query_sql($query, $execute = FALSE) {
  // Where conditions come from 'where' and 'fields' elements
  // Field conditions are OR'd or AND'd and added into the other conditions
  $where = !empty($query['where']) ? $query['where'] : array();
  $where_args = !empty($query['where args']) ? $query['where args'] : array();
  if (!empty($query['fields'])) {
    $operator = !empty($query['fields operator']) ? $query['fields operator'] : 'OR';
    $where[] = '('. implode(") $operator (", $query['fields']) .')';
  }
  if (!empty($query['fields args'])) {
    $where_args = array_merge($where_args, $query['fields args']);
  }
  $query['where'] = $where;
  $query['where args'] = $where_args;

  return messaging_query_sql($query, $execute);
}

/**
 * Build a query skeleton for inserting an event into the queue. 
 * 
 * This can be reused by other modules to build their custom queue queries
 * 
 * @param $event
 *   Event object
 * @return array()
 *   Query skeleton
 */
function notifications_query_event_queue($event) {
  $query['insert'] = array('uid', 'mdid', 'send_method', 'sid', 'module', 'eid', 'send_interval', 'language', 'cron', 'created', 'conditions');
  $query['into'] = '{notifications_queue}';
  $query['distinct'] = TRUE;
  $query['select'] = array('s.uid', 's.mdid', 's.send_method', 's.sid', 's.module', '%d', 's.send_interval', 's.language', 's.cron', '%d', 's.conditions');
  $query['from'] = array('{notifications} s');
  $query['select args'] = array($event->eid, $event->created);
  // We do a left join instead of inner join to allow subscriptions with no fields to work
  $query['join'] = array('LEFT JOIN {notifications_fields} f ON s.sid = f.sid');
  $query['where'] = array('s.status = 1', "s.event_type = '%s'", 's.send_interval >= 0');
  $query['where args'] = array($event->type);
  // Add one more condition if we don't send notifications on own posts
  if (!variable_get('notifications_sendself', 1) && !empty($event->uid)) {
    $query['where'][] = 's.uid <> %d';
    $query['where args'][] = $event->uid;
  }    
  // Some group by fields are not really needed but added for pgsql compatibility
  $query['group'] = array('s.uid', 's.mdid', 's.send_method', 's.sid', 's.module', 's.send_interval', 's.cron', 's.conditions', 's.language');
  
  // We throw in all the conditions and check the number of matching conditions
  // that must be equal to the subscription conditions number    
  $query['having'] = array('s.conditions = count(f.sid)');
  return $query;
}

/**
 * Run query for subscriptions table with field conditions
 * 
 * Note these queries are just for single type subscriptions all all field conditions 
 * are AND'd by default, unless set the 'fields operator' property
 * 
 * Builds queries for 'notifications' and 'notifications_fields' tables using schema
 * and fields (subscription fields) information.
 * 
 * @param array $main_conditions
 *   Array of multiple conditions in the notifications table. 
 * @param array $field_conditions
 *   Array of multiple conditions in the notifications_fields table. The array elements may be 
 *   - array of arrays that are field => value pairs
 *   - or key => array('type' => field, 'value' => value)
 *   If value is null, it just checks that a condition for the given field type exists
 * @param $query
 *   Optional query array to build upon
 * @param $full_loading
 *   Build the subscription objects and store them in the cache (We don't want this for deletion queries)
 * @return array()
 *   Resulting subscription objects indexed by sid
 */
function notifications_query_subscriptions($main_conditions, $field_conditions = array(), $query = array(), $full_loading = TRUE) {
  // Add default values to the query
  $query += array(
    'select' => "s.sid, s.type, s.event_type, s.send_interval, s.cron, s.module, s.status, s.destination, s.mdid, s.language, s.conditions",
    'distinct' => TRUE,
    'fields operator' => 'AND',
    'pager' => NULL,
    'where' => array(),
    'where args' => array(),
  );

  // The messaging query builder knows about table schemas
  $where = _messaging_query_where('notifications', $main_conditions, 's');
  $query['where'] = array_merge($query['where'], $where['where']);
  $query['where args'] = array_merge($query['where args'], $where['args']);
  $query['from'] = array('{notifications} s');
  // Add in field conditions. How the fields table is joined will depend on other parameters
  if ($field_conditions) {
    if ($query['fields operator'] == 'AND') {
      // We must multiple join fields table and the field conditions should be AND
      $query['fields index'] = 0;
    }
    else {
      // The field conditions will be OR'd so we can use a single join
       $query['join'][] = "LEFT JOIN {notifications_fields} f ON s.sid = f.sid";
       // And we need to group by all select fields to be able to use having
       $query['group'][] = implode(', ', $query['select']);
       $query['having'][] = 's.conditions = count(f.sid)';       
    }
    // Add in field conditions
    $query = notifications_query_build(array('fields' => $field_conditions), $query);
  }
  if ($query['pager'] && empty($query['order'])) {
    $query['order'][] = 's.sid';
  }
  // Build the query anhd throw it using simple query or pager query  
  $result = notifications_query_sql($query, TRUE);
  // Build list with results
  $subscriptions = array();
  while ($load = db_fetch_object($result)) {
    if ($full_loading) {
      // Using this loader will also place the subscriptions in the static cache
      $subs = notifications_load_subscription($load);
      $subscriptions[$subs->sid] = $subs;
    }
    else {
      $subscriptions[$load->sid] = $load;
    }
  }
  return $subscriptions;
}
