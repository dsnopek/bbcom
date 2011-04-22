<?php
/**
 * @file
 *   User pages for User Interface for subscriptions modules
 */
 
/**
 * User add subscription
 */
function notifications_ui_page_user_add($account, $type = NULL) {
  drupal_set_title(t('Add subscription'));
  if ($type && notifications_ui_access_user_add($account, $type)) {    
    $subscription = notifications_build_subscription($type);
    $subscription->set_account($account);
    return drupal_get_form('notifications_subscription_add_form', $subscription, "user/$account->uid/notifications");
  }
  else {    
    foreach (notifications_ui_subscription_types() as $key => $type) {
      if (notifications_ui_access_user_add($account, $key)) {
        $options[] = array(
          'title' => $type['title'],
          'href'  => "user/$account->uid/notifications/add/$key",
          'description' => !empty($type['description']) ? $type['description'] : '',
        );
      }
    }
    return theme('notifications_ui_add_list', $options);
  }  
}
