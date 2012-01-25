<?php

/**
 * Implementation of hook_content_default_fields().
 */
function bbcom_profile_content_default_fields() {
  $fields = array();

  // Exported field: field_profile_image
  $fields['profile-field_profile_image'] = array(
    'field_name' => 'field_profile_image',
    'type_name' => 'profile',
    'display_settings' => array(
      'weight' => '-3',
      'parent' => '',
      'token' => array(
        'format' => 'medium_default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'small_default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'medium_default',
        'exclude' => 1,
      ),
      '3' => array(
        'format' => 'small_default',
        'exclude' => 0,
      ),
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'medium_default',
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
      'file_extensions' => 'png gif jpg jpeg',
      'file_path' => '',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '',
      'max_filesize_per_node' => '',
      'max_resolution' => '0',
      'min_resolution' => '0',
      'alt' => '',
      'custom_alt' => 0,
      'title' => '',
      'custom_title' => 0,
      'title_type' => 'textfield',
      'default_image' => NULL,
      'use_default_image' => 0,
      'insert' => NULL,
      'insert_styles' => NULL,
      'insert_default' => NULL,
      'insert_class' => NULL,
      'insert_width' => NULL,
      'label' => 'Profile Image',
      'weight' => '-3',
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Profile Image');

  return $fields;
}
