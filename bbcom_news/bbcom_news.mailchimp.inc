<?php
// $Id$

/**
 * @file
 * MailChimp integration code
 */

// DRS: Borrowed graciously from the 'mailchimp' module
function _bbcom_news_mailchimp_object() {
  module_load_include('php', 'bbcom_news', 'MCAPI.class');

  $q = new MCAPI(variable_get('bbcom_news_mailchimp_api_key', ''));
  // set the timeout to something reasonsable to avoid taking down the Drupal site
  $q->setTimeout(60);
  
  if ($q->errorCode) {
    watchdog('mailchimp', $q->errorMessage, NULL, WATCHDOG_ERROR);
    return NULL;
  }
  return $q;
}

function _bbcom_news_mailchimp_list_id($q) {
  $list_name = variable_get('bbcom_news_mailchimp_list_name', 'BiblioBird.com Newsletter');
  $lists = $q->lists(array('list_name' => $list_name));
  if ($q->errorCode) {
    watchdog('mailchimp', 'Unable to get lists: (!code) !message',
      array('!code' => $q->errorCode, '!message' => $q->errorMessage),
      WATCHDOG_ERROR);
    return NULL;
  }
  elseif ($lists['total'] != 1) {
    watchdog('mailchimp', 'Unable to find list: !list_name', array('!list_name' => $list_name));
    return NULL;
  }
  return $lists['data'][0]['id'];
}

function _bbcom_news_mailchimp_update($account, $subscribe) {
  if (($q = _bbcom_news_mailchimp_object()) && ($list_id = _bbcom_news_mailchimp_list_id($q))) {
    if ($subscribe) {
      $lang_list = language_list();
      $name_parts = explode(' ', $account->name);
      $merge_vars = array(
        'FNAME' => array_shift($name_parts),
        'LNAME' => implode(' ', $name_parts),
        'GROUPINGS' => array(
          array(
            'name' => variable_get('bbcom_news_mailchimp_language_group', 'Language'),
            'groups' => $lang_list[$account->language]->native,
          ),
        ),
      );
      watchdog('mailchimp', 'merge_vars: '.print_r($merge_vars, TRUE), array(), WATCHDOG_DEBUG);
    }

    $sub_info = $q->listMemberInfo($list_id, array($account->mail));
    if ($q->errorCode) {
      watchdog('mailchimp', 'Unable to get listMemberInfo for user !username: (!code) !message',
        array('!username' => $account->name, '!code' => $q->errorCode, '!message' => $q->errorMessage),
        WATCHDOG_ERROR);
      return;
    }

    if (@$sub_info['data'][0]['status'] != 'subscribed') {
      if ($subscribe) {
        watchdog('mailchimp', 'Subscribing !user', array('!user' => $account->name), WATCHDOG_INFO);
        $q->listSubscribe($list_id, $account->mail, $merge_vars, 'html', FALSE, TRUE);
      }
    }
    else {
      if ($subscribe) {
        watchdog('mailchimp', 'Updating member info for !user', array('!user' => $account->name), WATCHDOG_INFO);
        $email_type = $sub_info['data'][0]['email_type'];
        $q->listUpdateMember($list_id, $account->mail, $merge_vars, $email_type);
      }
      else {
        watchdog('mailchimp', 'Unsubscribing !user', array('!user' => $account->name), WATCHDOG_INFO);
        $q->listUnsubscribe($list_id, $account->mail);
      }
    }

    if ($q->errorCode) {
      watchdog('mailchimp', 'Error for user !username: (!code) !message',
        array('!username' => $account->name, '!code' => $q->errorCode, '!message' => $q->errorMessage),
        WATCHDOG_ERROR);
    }
  }
}

