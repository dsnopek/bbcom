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
      'weight' => '-1',
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
      'weight' => '-1',
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
      'weight' => 0,
      'parent' => '',
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
      'weight' => 0,
      'description' => '',
      'type' => 'lingwo_senses_senses_widget',
      'module' => 'lingwo_senses',
    ),
  );

  // Translatables
  array(
    t('Fields'),
    t('Senses'),
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
    'description' => 'Working on the dictionary',
    'node' => array(
      '0' => 'entry',
    ),
    'menu_trail' => array(
      'node' => 'node',
    ),
    'block' => array(),
  );
  return $items;
}
