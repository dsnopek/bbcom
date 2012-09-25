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
      'weight' => '-2',
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Image');

  return $fields;
}
