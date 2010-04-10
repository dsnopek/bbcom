<?php

/**
 * Helper to implementation of hook_context_default_contexts().
 */
function _lingwoorg_content_context_default_contexts() {
  $items = array();

  $items[] = array(
    'namespace' => 'lingwoorg',
    'attribute' => 'section',
    'value' => 'content',
    'description' => 'Working in the content area.',
    'node' => array(
      '0' => 'content',
    ),
    'path' => array(
      '<front>' => '<front>',
    ),
    'menu' => 'content',
    'block' => array(
      'lingwoorg_content_0' => array(
        'module' => 'lingwoorg_content',
        'delta' => '0',
        'weight' => 15,
        'region' => 'right',
        'status' => '0',
        'label' => 'Lingwo.org Content: Add Content Block',
        'type' => 'context_ui',
      ),
      'lingwo_korpus_0' => array(
        'module' => 'lingwo_korpus',
        'delta' => '0',
        'weight' => 16,
        'region' => 'right',
        'status' => '0',
        'label' => 'Lingwo Korpus: Annotation Reader block',
        'type' => 'context_ui',
      ),
    ),
  );
  return $items;
}

/**
 * Helper to implementation of hook_menu_default_items().
 */
function _lingwoorg_content_menu_default_items() {
  $items = array();

  $items[] = array(
    'title' => 'Content',
    'path' => 'content',
    'weight' => '0',
  );
  // Translatables
  array(
    t('Content'),
  );


  return $items;
}

/**
 * Helper to implementation of hook_user_default_permissions().
 */
function _lingwoorg_content_user_default_permissions() {
  $permissions = array();

  // Exported permission: create content content
  $permissions[] = array(
    'name' => 'create content content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'editor',
    ),
  );

  // Exported permission: delete any content content
  $permissions[] = array(
    'name' => 'delete any content content',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: delete own content content
  $permissions[] = array(
    'name' => 'delete own content content',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: edit any content content
  $permissions[] = array(
    'name' => 'edit any content content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'editor',
    ),
  );

  // Exported permission: edit own content content
  $permissions[] = array(
    'name' => 'edit own content content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'editor',
    ),
  );

  return $permissions;
}
