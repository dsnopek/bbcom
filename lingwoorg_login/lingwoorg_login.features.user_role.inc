<?php

/**
 * Implementation of hook_user_default_roles().
 */
function lingwoorg_login_user_default_roles() {
  $roles = array();

  // Exported role: unverified user
  $roles['unverified user'] = array(
    'name' => 'unverified user',
  );

  return $roles;
}
