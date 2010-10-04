<?php

/**
 * Helper to implementation of hook_content_default_fields().
 */
function _lingwoorg_content_content_default_fields() {
  $fields = array();

  // Exported field: field_audio
  $fields[] = array(
    'field_name' => 'field_audio',
    'type_name' => 'content',
    'display_settings' => array(
      'weight' => '-3',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'swftools_no_file',
        'exclude' => 1,
      ),
      'full' => array(
        'format' => 'swftools',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'filefield',
    'required' => '0',
    'multiple' => '0',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'widget' => array(
      'file_extensions' => 'mp3',
      'file_path' => '',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '',
      'max_filesize_per_node' => '',
      'label' => 'Audio',
      'weight' => '-1',
      'description' => '',
      'type' => 'filefield_widget',
      'module' => 'filefield',
    ),
  );

  // Exported field: field_remote_url
  $fields[] = array(
    'field_name' => 'field_remote_url',
    'type_name' => 'content',
    'display_settings' => array(
      'label' => array(
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => 5,
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_remote_url][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Remote URL',
      'weight' => 0,
      'description' => '',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Translatables
  array(
    t('Audio'),
    t('Remote URL'),
  );

  return $fields;
}

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
