<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function bbcom_dictionary_user_default_permissions() {
  $permissions = array();

  // Exported permission: create Lingwo entry content
  $permissions['create Lingwo entry content'] = array(
    'name' => 'create Lingwo entry content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'editor',
    ),
  );

  // Exported permission: delete any Lingwo entry source content
  $permissions['delete any Lingwo entry source content'] = array(
    'name' => 'delete any Lingwo entry source content',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: delete any Lingwo entry translation content
  $permissions['delete any Lingwo entry translation content'] = array(
    'name' => 'delete any Lingwo entry translation content',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: delete own Lingwo entry source content
  $permissions['delete own Lingwo entry source content'] = array(
    'name' => 'delete own Lingwo entry source content',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: delete own Lingwo entry translation content
  $permissions['delete own Lingwo entry translation content'] = array(
    'name' => 'delete own Lingwo entry translation content',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: edit any Lingwo entry source content
  $permissions['edit any Lingwo entry source content'] = array(
    'name' => 'edit any Lingwo entry source content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'beta',
      '2' => 'editor',
      '3' => 'editor2',
    ),
  );

  // Exported permission: edit any Lingwo entry translation content
  $permissions['edit any Lingwo entry translation content'] = array(
    'name' => 'edit any Lingwo entry translation content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
      '2' => 'beta',
      '3' => 'editor',
      '4' => 'editor1',
      '5' => 'editor2',
      '6' => 'translator',
    ),
  );

  // Exported permission: edit own Lingwo entry source content
  $permissions['edit own Lingwo entry source content'] = array(
    'name' => 'edit own Lingwo entry source content',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: edit own Lingwo entry translation content
  $permissions['edit own Lingwo entry translation content'] = array(
    'name' => 'edit own Lingwo entry translation content',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  return $permissions;
}
