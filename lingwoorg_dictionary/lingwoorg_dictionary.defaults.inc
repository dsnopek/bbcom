<?php

/**
 * Helper to implementation of hook_content_default_fields().
 */
function _lingwoorg_dictionary_content_default_fields() {
  $fields = array();

  // Exported field: field_fields
  $fields[] = array(
    'field_name' => 'field_fields',
    'type_name' => 'entry',
    'display_settings' => array(
      'weight' => '5',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'lingwo_fields',
    'required' => '0',
    'multiple' => '1',
    'module' => 'lingwo_fields',
    'active' => '1',
    'widget' => array(
      'default_value' => array(
        'refresh' => 'Refresh',
      ),
      'default_value_php' => NULL,
      'label' => 'Fields',
      'weight' => '5',
      'description' => '',
      'type' => 'lingwo_fields_widget',
      'module' => 'lingwo_fields',
    ),
  );

  // Exported field: field_senses
  $fields[] = array(
    'field_name' => 'field_senses',
    'type_name' => 'entry',
    'display_settings' => array(
      'weight' => '4',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
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
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'lingwo_senses',
    'required' => '0',
    'multiple' => '1',
    'module' => 'lingwo_senses',
    'active' => '1',
    'widget' => array(
      'default_value' => array(
        'wrapper' => array(
          'add_sense' => 'Add Another Sense',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Senses',
      'weight' => '4',
      'description' => '',
      'type' => 'lingwo_senses_widget',
      'module' => 'lingwo_senses',
    ),
  );

  // Exported field: field_source_copyright
  $fields[] = array(
    'field_name' => 'field_source_copyright',
    'type_name' => 'entry',
    'display_settings' => array(
      'weight' => '8',
      'parent' => 'group_sources',
      'label' => array(
        'format' => 'above',
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
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
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
    'multiple' => '1',
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
          '_error_element' => 'default_value_widget][field_source_copyright][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Copyright',
      'weight' => '8',
      'description' => '',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Exported field: field_source_license
  $fields[] = array(
    'field_name' => 'field_source_license',
    'type_name' => 'entry',
    'display_settings' => array(
      'weight' => '9',
      'parent' => 'group_sources',
      'label' => array(
        'format' => 'above',
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
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
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
    'multiple' => '1',
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
          '_error_element' => 'default_value_widget][field_source_license][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'License',
      'weight' => '9',
      'description' => '',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Exported field: field_source_name
  $fields[] = array(
    'field_name' => 'field_source_name',
    'type_name' => 'entry',
    'display_settings' => array(
      'weight' => '6',
      'parent' => 'group_sources',
      'label' => array(
        'format' => 'above',
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
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
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
    'multiple' => '1',
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
          '_error_element' => 'default_value_widget][field_source_name][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Source Name',
      'weight' => '6',
      'description' => '',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Exported field: field_source_raw
  $fields[] = array(
    'field_name' => 'field_source_raw',
    'type_name' => 'entry',
    'display_settings' => array(
      'weight' => '10',
      'parent' => 'group_sources',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'text_pre',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
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
    'multiple' => '1',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => '5',
      'size' => 60,
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_source_raw][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Raw Data',
      'weight' => '10',
      'description' => '',
      'type' => 'text_textarea',
      'module' => 'text',
    ),
  );

  // Exported field: field_source_timestamp
  $fields[] = array(
    'field_name' => 'field_source_timestamp',
    'type_name' => 'entry',
    'display_settings' => array(
      'weight' => '11',
      'parent' => 'group_sources',
      'label' => array(
        'format' => 'above',
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
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
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
    'multiple' => '1',
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
          '_error_element' => 'default_value_widget][field_source_timestamp][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Timestamp',
      'weight' => '11',
      'description' => '',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Exported field: field_source_url
  $fields[] = array(
    'field_name' => 'field_source_url',
    'type_name' => 'entry',
    'display_settings' => array(
      'weight' => '7',
      'parent' => 'group_sources',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'text_link',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
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
    'multiple' => '1',
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
          '_error_element' => 'default_value_widget][field_source_url][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Source URL',
      'weight' => '7',
      'description' => '',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Translatables
  array(
    t('Copyright'),
    t('Fields'),
    t('License'),
    t('Raw Data'),
    t('Senses'),
    t('Source Name'),
    t('Source URL'),
    t('Timestamp'),
  );

  return $fields;
}

/**
 * Helper to implementation of hook_context_default_contexts().
 */
function _lingwoorg_dictionary_context_default_contexts() {
  $items = array();

  $items[] = array(
    'namespace' => 'lingwoorg',
    'attribute' => 'section',
    'value' => 'dictionary',
    'description' => 'Working in the dictionary',
    'node' => array(
      '0' => 'entry',
    ),
    'path' => array(
      'dictionary' => 'dictionary',
      'dictionary/*' => 'dictionary/*',
    ),
    'menu' => 'node/53402',
    'block' => array(
      'lingwo_dictionary_0' => array(
        'module' => 'lingwo_dictionary',
        'delta' => '0',
        'weight' => -17,
        'region' => 'header',
        'status' => '0',
        'label' => 'Lingwo Dictionary: Search block',
        'type' => 'context_ui',
      ),
      'lingwoorg_dictionary_0' => array(
        'module' => 'lingwoorg_dictionary',
        'delta' => '0',
        'weight' => 16,
        'region' => 'right',
        'status' => '0',
        'label' => 'Lingwo Dictionary: Add Content Block',
        'type' => 'context_ui',
      ),
    ),
  );
  return $items;
}

/**
 * Helper to implementation of hook_user_default_permissions().
 */
function _lingwoorg_dictionary_user_default_permissions() {
  $permissions = array();

  // Exported permission: create entry content
  $permissions[] = array(
    'name' => 'create entry content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
      '2' => 'editor',
      '3' => 'manager',
    ),
  );

  // Exported permission: delete any entry content
  $permissions[] = array(
    'name' => 'delete any entry content',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: delete own entry content
  $permissions[] = array(
    'name' => 'delete own entry content',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: edit any entry content
  $permissions[] = array(
    'name' => 'edit any entry content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
      '2' => 'editor',
      '3' => 'manager',
    ),
  );

  // Exported permission: edit own entry content
  $permissions[] = array(
    'name' => 'edit own entry content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
      '2' => 'editor',
      '3' => 'manager',
    ),
  );

  return $permissions;
}
