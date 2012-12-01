<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function bbcom_content_user_default_permissions() {
  $permissions = array();

  // Exported permission: create content content
  $permissions['create content content'] = array(
    'name' => 'create content content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'beta',
      '2' => 'editor',
    ),
  );

  // Exported permission: delete any content content
  $permissions['delete any content content'] = array(
    'name' => 'delete any content content',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: delete own content content
  $permissions['delete own content content'] = array(
    'name' => 'delete own content content',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: edit any content content
  $permissions['edit any content content'] = array(
    'name' => 'edit any content content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'editor',
    ),
  );

  // Exported permission: edit own content content
  $permissions['edit own content content'] = array(
    'name' => 'edit own content content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'beta',
      '2' => 'editor',
    ),
  );

  return $permissions;
}
