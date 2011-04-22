<?php
// $Id: remember_me.inc,v 1.1.2.1 2009/05/25 15:09:03 nickl Exp $

/**
 * @file
 * Functions that are not needed at all times can be included when required.
 */

/**
 * Manage phantom sessions.
 *
 * When a user closes their browser their session cookie expires. If the user
 * chose to be forgotten they will not be allowed to use the old session.
 * These phantom sessions still appear in Who's online block and will show duplicate
 * names. If configured to do so, via settings page, set phantom session's timeout period
 * outside of the Who's online block's display period.
 */
function remember_me_manage_phantom_sessions() {
  global $user;
  $interval = time() - variable_get('user_block_seconds_online', 900);
  $count = db_result(
    db_query("SELECT COUNT(uid) FROM {sessions} WHERE uid = %d AND timestamp >= %d",
      $user->uid,
      $interval
    )
  );
  if ($count > 0) {
    watchdog('remember_me',
      t('Phantom session managed for %user, session trapped within %time timeout.'),
      array(
        '%user' => $user->name,
        '%id'   => $user-uid,
        '%time' => format_interval(variable_get('user_block_seconds_online', 900))
      ),
      WATCHDOG_NOTICE
    );
    db_query("UPDATE {sessions} SET timestamp = %d WHERE uid = %d AND timestamp >= %d",
      $interval - 1,
      $user->uid,
      $interval
    );
  }
}

/**
 * Try to insert 'Remember me' checkbox between 'Username', 'Password' and
 * 'Log in' form elements.
 */
function remember_me_insert_checkbox(&$form) {
  $form1['remember_me'] = array(
    '#title'         => t('Remember me'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('remember_me_checkbox', 0),
  );
  _remember_me_array_insert_after_key('pass', $form, $form1);
  $form['name']['#attributes']['tabindex']        = '1';
  $form['pass']['#attributes']['tabindex']        = '2';
  $form['remember_me']['#attributes']['tabindex'] = '3';
  $form['submit']['#attributes']['tabindex']      = '4';
}

/**
 * Insert elements into array after the element with $searck_key key and
 * maintain index association.
 *
 * @param $search_key
 *   Key of array element to insert elements after.
 * @param $array
 *   Given array.
 * @param $elements_to_insert
 *   Array of elements to be inserted.
 */
function _remember_me_array_insert_after_key($search_key, &$array, $elements_to_insert) {
  // Split array into two arrays: prior $search_key element including
  // $search_key element and after $search_key element.
  $array1     = array();
  $array2     = array();
  $curr_array = &$array1;
  foreach ($array as $key => $value) {
    $curr_array[$key] = $value;
    if ($search_key == $key) {
      $curr_array = &$array2;
    }
  }

  // Add $elements_to_insert to the end of the first array.
  $array1 = $array1 + $elements_to_insert;
  $array  = $array1 + $array2;
}