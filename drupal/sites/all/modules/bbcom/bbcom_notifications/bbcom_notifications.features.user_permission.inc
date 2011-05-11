<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function bbcom_notifications_user_default_permissions() {
  $permissions = array();

  // Exported permission: maintain own subscriptions
  $permissions['maintain own subscriptions'] = array(
    'name' => 'maintain own subscriptions',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
      '2' => 'unverified user',
    ),
  );

  // Exported permission: manage own subscriptions
  $permissions['manage own subscriptions'] = array(
    'name' => 'manage own subscriptions',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
      '2' => 'unverified user',
    ),
  );

  // Exported permission: subscribe to author
  $permissions['subscribe to author'] = array(
    'name' => 'subscribe to author',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: subscribe to content
  $permissions['subscribe to content'] = array(
    'name' => 'subscribe to content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
      '2' => 'unverified user',
    ),
  );

  // Exported permission: subscribe to content type
  $permissions['subscribe to content type'] = array(
    'name' => 'subscribe to content type',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: subscribe to content type and author
  $permissions['subscribe to content type and author'] = array(
    'name' => 'subscribe to content type and author',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: subscribe to taxonomy terms
  $permissions['subscribe to taxonomy terms'] = array(
    'name' => 'subscribe to taxonomy terms',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
      '2' => 'unverified user',
    ),
  );

  return $permissions;
}
