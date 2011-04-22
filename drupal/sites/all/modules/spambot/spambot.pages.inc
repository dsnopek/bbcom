<?php

function spambot_user_spam_admin_form($form_state, $account) {
  $node_count = db_result(db_query("SELECT COUNT(nid) FROM {node} WHERE uid = %d", $account->uid));
  if (module_exists('comment')) {
    $comment_count = db_result(db_query("SELECT COUNT(cid) FROM {comments} WHERE uid = %d", $account->uid));
    $status = t('This account has @n nodes and @c comments.', array('@n' => $node_count, '@c' => $comment_count));
  }
  else {
    $status = t('This account has @n nodes.', array('@n' => $node_count));
  }

  $form = array();
  $form['check'] = array(
    '#type' => 'submit',
    '#value' => t('Check if this account matches a known spammer'),
  );

  $form['action'] = array(
    '#type' => 'fieldset',
    '#title' => t('Take action against this account'),
    '#collapsible' => TRUE,
    '#description' => $status,
  );
  $form['action']['unpublish_content'] = array(
    '#type' => 'checkbox',
    '#title' => t('Unpublish nodes and comments by this account'),
    '#default_value' => TRUE,
  );
  $form['action']['delete_content'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete nodes and comments by this account'),
    '#default_value' => FALSE,
  );

  // Fetch and process IP address information
  $ips = spambot_account_ip_addresses($account);
  $options = array();
  $default = array();
  foreach ($ips as $ip) {
    $options[$ip] = $ip;
    $default[$ip] = $ip;
  }

  $form['action']['report_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Report this account to www.stopforumspam.com as a spammer'),
    '#description' => t('Requires that you have an API key from <a href="http://www.stopforumspam.com">www.stopforumspam.com</a> and at least one IP address selected below.'),
    '#default_value' => FALSE,
    '#disabled' => !count($options),
  );

  $form['action']['ip_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('IP Addresses'),
    '#description' => t('This is a list of IP addresses used by this account.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  if (count($options)) {
    $form['action']['ip_info']['ip_addresses'] = array(
      '#type' => 'checkboxes',
      '#title' => t('IP addresses to report'),
      '#description' => t('If you wish to report this account as a spammer, at least one IP address must be selected.'),
      '#options' => $options,
      '#default_value' => $default,
    );
  }
  else {
    $form['action']['ip_info']['ip_addresses'] = array(
      '#type' => 'value', '#value' => array(),
    );
    $form['action']['ip_info']['#description'] .= '<br />' . t('No IP addresses found. IP address information for this user may have expired from storage.');
  }
  
  $form['action']['block_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Block this account'),
    '#default_value' => TRUE,
  );
  $form['action']['delete_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete this account'),
    '#default_value' => FALSE,
  );  
  $form['action']['action'] = array(
    '#type' => 'submit',
    '#value' => t('Take action'),
  );
  $form['uid'] = array('#type' => 'value', '#value' => $account->uid);
  return $form;
}

function spambot_user_spam_admin_form_submit($form, &$form_state) {
  $account = user_load($form_state['values']['uid']);
  if ($form_state['values']['op'] == $form_state['values']['check']) {
    // This is a more comprehensive check than the automated criteria checks.
    // This tests everything.    
    $messages = array();
    $service_down = FALSE;

    // Check email and username
    $request = array('email' => $account->mail, 'username' => $account->name);
    $data = array();
    if (spambot_sfs_request($request, $data)) {
      if (!empty($data['email']['appears'])) {
        $messages[] = t('This account\'s email address matches @num times: @email', array('@email' => $request['email'], '@num' => $data['email']['frequency']));
      }

      if (!empty($data['username']['appears'])) {
        $messages[] = t('This account\'s username matches @num times: @username', array('@username' => $request['username'], '@num' => $data['username']['frequency']));
      }
    }
    else {
      drupal_set_message(t('Error contacting service.'), 'warning');
      $service_down = TRUE;
    }

    // Check IP addresses
    if (!$service_down) {
      $ips = spambot_account_ip_addresses($account);
      foreach ($ips as $ip) {
        // Skip the loopback interface
        if ($ip == '127.0.0.1') {
          continue;
        }
        
        $request = array('ip' => $ip);
        $data = array();
        if (spambot_sfs_request($request, $data)) {
          if (!empty($data['ip']['appears'])) {
            $messages[] = t('An IP address @ip used by this account matches @num times.', array('@ip' => $ip, '@num' => $data['ip']['frequency']));
          }
        }
        else {
          drupal_set_message(t('Error contacting service.'), 'warning');
          $service_down = TRUE;
          break;
        }
      }
    }

    if (count($messages)) {
      foreach ($messages as $message) {
        drupal_set_message($message);
      }
    }
    else {
      drupal_set_message(t('No matches against known spammers found.'));
    }
  }
  else if ($form_state['values']['op'] == $form_state['values']['action']) {
    if ($account->uid == 1) {
      drupal_set_message(t('Sorry, taking action against uid 1 is not allowed.'));
      return;
    }

    // Block account
    if (!empty($form_state['values']['block_user'])) {
      if ($account->status) {
        user_save($account, array('status' => 0));
        drupal_set_message(t('Account blocked.'));
      }
      else {
        drupal_set_message(t('This account is already blocked.'));
      }
    }

    // Report user as spammer
    if (!empty($form_state['values']['report_user'])) {
      if (variable_get('spambot_sfs_api_key', FALSE)) {
        $ips = array_filter($form_state['values']['ip_addresses']);
        if (count($ips) == 0) {
          drupal_set_message(t('This account cannot be reported without an IP address.'), 'warning');
        }
        else if (spambot_report_account($account, $ips)) {
          drupal_set_message(t('This account has been reported as a spammer.'));
        }
        else {
          drupal_set_message(t('There was an error reporting this account as a spammer.'), 'error');
        }
      }
      else {
        drupal_set_message(t('To report spammers to www.stopforumspam.com, you need to register for an API key at <a href="http://www.stopforumspam.com">www.stopforumspam.com</a> and enter it into the !page.', array('!page' => l('Stop forum spam settings', 'admin/settings/stopforumspam'))));
      }
    }

    // Prepare some data
    $nodes = array();
    $result = db_query("SELECT nid FROM {node} WHERE uid = %d ORDER BY nid", $account->uid);
    while ($object = db_fetch_object($result)) {
      $nodes[$object->nid] = $object->nid;
    }

    $comments = array();
    if (module_exists('comment')) {
      $result = db_query("SELECT cid FROM {comments} WHERE uid = %d ORDER BY cid", $account->uid);
      while ($object = db_fetch_object($result)) {
        $comments[$object->cid] = $object->cid;
      }
    }

    // Delete nodes and content
    if (!empty($form_state['values']['delete_content'])) {
      if (count($nodes)) {
        foreach ($nodes as $nid) {
          node_delete($nid);        
        }
      }

      if (count($comments)) {
        foreach ($comments as $cid) {
          $comment = _comment_load($cid);
          module_load_include('inc', 'comment', 'comment.admin');
          _comment_delete_thread($comment);
          _comment_update_node_statistics($comment->nid);
        }
        cache_clear_all();
      }

      drupal_set_message(t('Nodes and comments have been deleted.'));
    }
    else if (!empty($form_state['values']['unpublish_content'])) {
      // Unpublish nodes and content
      if (count($nodes)) {
        module_load_include('inc', 'node', 'node.admin');
        node_mass_update($nodes, array('status' => 0));
      }

      if (count($comments)) {
        db_query("UPDATE {comments} SET status = %d WHERE uid = %d", COMMENT_NOT_PUBLISHED, $account->uid);
        cache_clear_all();
      }
      drupal_set_message(t('Nodes and comments have been unpublished.'));
    }

    // Delete user
    if (!empty($form_state['values']['delete_user'])) {
      // Redirect to user delete form
      $form_state['redirect'] = 'user/' . $account->uid . '/delete';
    }
  }
}
