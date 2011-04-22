<?php

function spambot_settings_form($form_state) {
  $form = array();

  $numbers = array(0 => t('Never'), 1 => 1, 2 => 2, 3 => 3, 4 => 4, 5 => 5, 6 => 6, 7 => 7, 8 => 8, 9 => 9, 10 => 10, 15 => 15, 20 => 20, 30 => 30, 40 => 40, 50 => 50, 60 => 60, 70 => 70, 80 => 80, 90 => 90, 100 => 100, 150 => 150, 200 => 200);

  $form['criteria'] = array(
    '#type' => 'fieldset',
    '#title' => t('Spammer criteria'),
    '#description' => t('A user account or an attempted user registration will be deemed a spammer if the email, username, or IP address has been reported to www.stopforumspam.com more times than the following thresholds.'),
    '#collapsible' => TRUE,
  );
  $form['criteria']['spambot_criteria_email'] = array(
    '#type' => 'select',
    '#title' => t('Number of times the email has been reported is equal to or more than'),
    '#description' => t('If the email address for a user or user registration has been reported to www.stopforumspam.com this many times, then it is deemed as a spammer.'),
    '#options' => array(0 => t('Don\'t use email as a criteria')) + $numbers,
    '#default_value' => variable_get('spambot_criteria_email', 1),
  );
  $form['criteria']['spambot_criteria_username'] = array(
    '#type' => 'select',
    '#title' => t('Number of times the username has been reported is equal to or more than'),
    '#description' => t('If the username for a user or user registration has been reported to www.stopforumspam.com this many times, then it is deemed as a spammer. Be careful about using this option as you may accidentally block genuine users who happen to choose the same username as a known spammer.'),
    '#options' => array(0 => t('Don\'t use username as a criteria')) + $numbers,
    '#default_value' => variable_get('spambot_criteria_username', 0),
  );
  $form['criteria']['spambot_criteria_ip'] = array(
    '#type' => 'select',
    '#title' => t('Number of times the IP address has been reported is equal to or more than'),
    '#description' => t('If the email address for a user or user registration has been reported to www.stopforumspam.com this many times, then it is deemed as a spammer. Be careful about setting this threshold too low as IP addresses can change.'),
    '#options' => array(0 => t('Don\'t use IP address as a criteria')) + $numbers,
    '#default_value' => variable_get('spambot_criteria_ip', 20),
  );

  $form['register'] = array(
    '#type' => 'fieldset',
    '#title' => t('User registration'),
    '#collapsible' => TRUE,
  );
  $form['register']['spambot_user_register_protect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Protect the user registration form'),
    '#description' => t('If ticked, new user registrations will be tested if they match any known spammers and blacklisted.'),
    '#default_value' => variable_get('spambot_user_register_protect', TRUE),
  );
  $form['register']['spambot_blocked_message'] = array(
    '#type' => 'textfield',
    '#title' => t('User registration blocked message'),
    '#default_value' => variable_get('spambot_blocked_message', t('Your email address or username is blacklisted.')),
    '#description' => t('Message to display when user registration is blocked.'),
  );
  $form['register']['spambot_user_register_report'] = array(
    '#type' => 'checkbox',
    '#title' => t('Report blocked user registrations as spammers'),
    '#description' => t('If ticked, new user registrations which were blocked as spammers are also reported to www.stopforumspam.com as spammers. Requires an API key.'),
    '#default_value' => variable_get('spambot_user_register_report', FALSE),
  );

  $form['existing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Scan existing accounts'),
    '#description' => t('This module can also check existing user accounts to see if they are known spammers. It works by checking user accounts with increasing uid\'s ie. user id 2, 3, 4 etc during cron. Any existing users found to be known spammers are logged. Blocked user accounts are not checked.'),
    '#collapsible' => TRUE,
  );
  $form['existing']['spambot_cron_user_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum number of user accounts to check per cron'),
    '#description' => t('Enter the number of user accounts to check for each cron. If you do not want to check existing user accounts, leave this as 0.<br />Be careful not to make this value too large, as it will slow your cron execution down and may cause your site to query www.stopforumspam.com more times than allowed each day.'),
    '#size' => 10,
    '#default_value' => variable_get('spambot_cron_user_limit', 0),
  );
  $form['existing']['spambot_spam_account_action'] = array(
    '#type' => 'select',
    '#title' => t('Action to take'),
    '#description' => t('Please select what action to take for user accounts which are found to be spammers.'),
    '#options' => array(
      SPAMBOT_ACTION_NONE => t('None, just log it.'),
      SPAMBOT_ACTION_BLOCK => t('Block user account'),
      SPAMBOT_ACTION_DELETE => t('Delete user account'),
    ),
    '#default_value' => variable_get('spambot_spam_account_action', SPAMBOT_ACTION_NONE),
  );

  // Show scan status
  $last_uid = variable_get('spambot_last_checked_uid', 0);
  $suffix = '';
  if ($last_uid) {
    $num_checked = db_result(db_query("SELECT COUNT(uid) FROM {users} WHERE uid > 1 AND uid <= %d", $last_uid));
    $num_left = db_result(db_query("SELECT COUNT(uid) FROM {users} WHERE uid > 1 AND uid > %d", $last_uid));
    $last_uid = db_result(db_query("SELECT uid FROM {users} WHERE uid > 1 AND uid <= %d ORDER BY uid DESC LIMIT 1", $last_uid));   

    $account = user_load($last_uid);
    $suffix = '<br />' . t('The last checked user account is: !account (uid @uid)', array('!account' => l(check_plain($account->name), 'user/' . $account->uid), '@uid' => $account->uid));
  }
  else {
    $num_checked = 0;
    $num_left = db_result(db_query("SELECT COUNT(uid) FROM {users} WHERE uid > 1"));
  }

  $text = t('Accounts checked: @checked, Accounts remaining: @remaining', array('@checked' => $num_checked, '@remaining' => $num_left));
  $form['existing']['message'] = array(
    '#type' => 'fieldset',
    '#title' => t('Scan status'),
    '#description' => $text . $suffix,
  );

  $form['existing']['spambot_last_checked_uid'] = array(
    '#type' => 'textfield',
    '#title' => t('Continue scanning after this user id'),
    '#size' => 10,
    '#description' => t('Scanning of existing user accounts has progressed to, and including, user id @uid and will continue by scanning accounts after user id @uid. If you wish to change where the scan continues scanning from, enter a different user id here. If you wish to scan all users again, enter a value of 0.', array('@uid' => $last_uid)),
    '#default_value' => $last_uid,
  );

  $form['spambot_sfs_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('www.stopforumspam.com API key'),
    '#description' => t('If you wish to report spammers to Stop Forum Spam, you need to register for an API key at the <a href="http://www.stopforumspam.com">Stop Forum Spam</a> website.'),
    '#default_value' => variable_get('spambot_sfs_api_key', FALSE),
  );

  return system_settings_form($form);
}
