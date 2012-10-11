<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function bbcom_forum_user_default_permissions() {
  $permissions = array();

  // Exported permission: create forum topics
  $permissions['create forum topics'] = array(
    'name' => 'create forum topics',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: create forum_reply content
  $permissions['create forum_reply content'] = array(
    'name' => 'create forum_reply content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit own forum topics
  $permissions['edit own forum topics'] = array(
    'name' => 'edit own forum topics',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit own forum_reply content
  $permissions['edit own forum_reply content'] = array(
    'name' => 'edit own forum_reply content',
    'roles' => array(
      '0' => 'Forum Moderator',
      '1' => 'admin',
      '2' => 'authenticated user',
    ),
  );

  return $permissions;
}
