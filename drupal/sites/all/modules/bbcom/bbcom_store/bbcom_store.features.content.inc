<?php

/**
 * Implementation of hook_content_default_fields().
 */
function bbcom_store_content_default_fields() {
  $fields = array();

  // Exported field: field_image_cache
  $fields['package-field_image_cache'] = array(
    'field_name' => 'field_image_cache',
    'type_name' => 'package',
    'display_settings' => array(
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'hidden',
      ),
      'full' => array(
        'format' => 'hidden',
      ),
      '4' => array(
        'format' => 'hidden',
      ),
    ),
    'widget_active' => '1',
    'type' => 'filefield',
    'required' => '0',
    'multiple' => '1',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'premium_fields' => '',
    'widget' => array(
      'file_extensions' => 'gif jpg png',
      'file_path' => NULL,
      'progress_indicator' => NULL,
      'max_filesize_per_file' => NULL,
      'max_filesize_per_node' => NULL,
      'max_resolution' => NULL,
      'min_resolution' => NULL,
      'alt' => NULL,
      'custom_alt' => 1,
      'title' => NULL,
      'custom_title' => 1,
      'title_type' => NULL,
      'default_image' => NULL,
      'use_default_image' => NULL,
      'insert' => NULL,
      'insert_styles' => NULL,
      'insert_default' => NULL,
      'insert_class' => NULL,
      'insert_width' => NULL,
      'label' => 'Image',
      'weight' => '-3',
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Exported field: field_package_dependencies
  $fields['package-field_package_dependencies'] = array(
    'field_name' => 'field_package_dependencies',
    'type_name' => 'package',
    'display_settings' => array(
      'label' => array(
        'format' => 'above',
        'exclude' => 0,
      ),
      '5' => array(
        'format' => 'default',
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
    'type' => 'nodereference',
    'required' => '0',
    'multiple' => '1',
    'module' => 'nodereference',
    'active' => '1',
    'referenceable_types' => array(
      'package' => 'package',
      'book' => 0,
      'content' => 0,
      'entry' => 0,
      'forum_reply' => 0,
      'forum' => 0,
      'language' => 0,
      'long_answer' => 0,
      'matching' => 0,
      'multichoice' => 0,
      'page' => 0,
      'product' => 0,
      'product_kit' => 0,
      'profile' => 0,
      'quiz' => 0,
      'quiz_directions' => 0,
      'rwiki' => 0,
      'scale' => 0,
      'short_answer' => 0,
      'story' => 0,
      'support_ticket' => 0,
      'truefalse' => 0,
      'wiki' => 0,
    ),
    'advanced_view' => '--',
    'advanced_view_args' => '',
    'premium_fields' => 0,
    'widget' => array(
      'autocomplete_match' => 'contains',
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'nid' => NULL,
          '_error_element' => 'default_value_widget][field_package_dependencies][0][nid][nid',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Dependencies',
      'weight' => '2',
      'description' => '',
      'type' => 'nodereference_autocomplete',
      'module' => 'nodereference',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Dependencies');
  t('Image');

  return $fields;
}
