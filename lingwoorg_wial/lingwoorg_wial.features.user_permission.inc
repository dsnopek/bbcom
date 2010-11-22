<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function lingwoorg_wial_user_default_permissions() {
  $permissions = array();

  // Exported permission: access other wials
  $permissions['access other wials'] = array(
    'name' => 'access other wials',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: access wial
  $permissions['access wial'] = array(
    'name' => 'access wial',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
      '2' => 'unverified user',
    ),
  );

  return $permissions;
}
