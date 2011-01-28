<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function bbcom_dictionary_user_default_permissions() {
  $permissions = array();

  // Exported permission: create entry content
  $permissions['create entry content'] = array(
    'name' => 'create entry content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'editor',
    ),
  );

  // Exported permission: delete any entry content
  $permissions['delete any entry content'] = array(
    'name' => 'delete any entry content',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: delete own entry content
  $permissions['delete own entry content'] = array(
    'name' => 'delete own entry content',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: edit any entry content
  $permissions['edit any entry content'] = array(
    'name' => 'edit any entry content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'editor',
      '2' => 'translator',
    ),
  );

  // Exported permission: edit own entry content
  $permissions['edit own entry content'] = array(
    'name' => 'edit own entry content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'editor',
      '2' => 'translator',
    ),
  );

  return $permissions;
}
