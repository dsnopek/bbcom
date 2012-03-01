<?php

/**
 * Implementation of hook_content_default_fields().
 */
function bbcom_content_content_default_fields() {
  $fields = array();

  // Exported field: field_audio
  $fields['content-field_audio'] = array(
    'field_name' => 'field_audio',
    'type_name' => 'content',
    'display_settings' => array(
      'weight' => '1',
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
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 1,
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
      'max_filesize_per_file' => '10M',
      'max_filesize_per_node' => '',
      'insert' => 0,
      'insert_styles' => array(
        'auto' => 0,
        'link' => 0,
        'image' => 0,
        'imagecache_large' => 0,
        'imagecache_medium' => 0,
        'imagecache_small' => 0,
        'wiki' => 0,
      ),
      'insert_default' => 'auto',
      'insert_class' => '',
      'insert_width' => '',
      'label' => 'Audio (natural speed)',
      'weight' => '1',
      'description' => '',
      'type' => 'filefield_widget',
      'module' => 'filefield',
    ),
  );

  // Exported field: field_audio_slower
  $fields['content-field_audio_slower'] = array(
    'field_name' => 'field_audio_slower',
    'type_name' => 'content',
    'display_settings' => array(
      'weight' => '2',
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
      'max_filesize_per_file' => '10M',
      'max_filesize_per_node' => '',
      'insert' => 0,
      'insert_styles' => array(
        'auto' => 0,
        'link' => 0,
        'image' => 0,
        'imagecache_large' => 0,
        'lightbox2--large--large' => 0,
        'lightbox2--large--medium' => 0,
        'lightbox2--large--original' => 0,
        'lightbox2--large--small' => 0,
        'lightbox2--large--tiny' => 0,
        'lightbox2--link--large' => 0,
        'lightbox2--link--medium' => 0,
        'lightbox2--link--small' => 0,
        'lightbox2--link--tiny' => 0,
        'lightbox2--medium--large' => 0,
        'lightbox2--medium--medium' => 0,
        'lightbox2--medium--original' => 0,
        'lightbox2--medium--small' => 0,
        'lightbox2--medium--tiny' => 0,
        'lightbox2--original--large' => 0,
        'lightbox2--original--medium' => 0,
        'lightbox2--original--small' => 0,
        'lightbox2--original--tiny' => 0,
        'lightbox2--small--large' => 0,
        'lightbox2--small--medium' => 0,
        'lightbox2--small--original' => 0,
        'lightbox2--small--small' => 0,
        'lightbox2--small--tiny' => 0,
        'lightbox2--tiny--large' => 0,
        'lightbox2--tiny--medium' => 0,
        'lightbox2--tiny--original' => 0,
        'lightbox2--tiny--small' => 0,
        'lightbox2--tiny--tiny' => 0,
        'imagecache_medium' => 0,
        'imagecache_small' => 0,
        'imagecache_tiny' => 0,
        'wiki' => 0,
      ),
      'insert_default' => 'auto',
      'insert_class' => '',
      'insert_width' => '',
      'label' => 'Audio (slower)',
      'weight' => '2',
      'description' => '',
      'type' => 'filefield_widget',
      'module' => 'filefield',
    ),
  );

  // Exported field: field_content_image
  $fields['content-field_content_image'] = array(
    'field_name' => 'field_content_image',
    'type_name' => 'content',
    'display_settings' => array(
      'weight' => '-1',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'image_plain',
        'exclude' => 1,
      ),
      'full' => array(
        'format' => 'image_plain',
        'exclude' => 1,
      ),
      '2' => array(
        'format' => 'image_plain',
        'exclude' => 1,
      ),
      '3' => array(
        'format' => 'image_plain',
        'exclude' => 1,
      ),
      '4' => array(
        'format' => 'image_plain',
        'exclude' => 1,
      ),
      'token' => array(
        'format' => 'image_plain',
        'exclude' => 0,
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
      'insert' => 1,
      'insert_styles' => array(
        'imagecache_large' => 'imagecache_large',
        'imagecache_medium' => 'imagecache_medium',
        'imagecache_small' => 'imagecache_small',
        'auto' => 0,
        'link' => 0,
        'image' => 0,
        'wiki' => 0,
      ),
      'insert_default' => 'imagecache_small',
      'insert_class' => '',
      'insert_width' => '',
      'label' => 'Images',
      'weight' => '-1',
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Exported field: field_remote_url
  $fields['content-field_remote_url'] = array(
    'field_name' => 'field_remote_url',
    'type_name' => 'content',
    'display_settings' => array(
      'weight' => '3',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 1,
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
      'weight' => '3',
      'description' => '',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Audio (natural speed)');
  t('Audio (slower)');
  t('Images');
  t('Remote URL');

  return $fields;
}
