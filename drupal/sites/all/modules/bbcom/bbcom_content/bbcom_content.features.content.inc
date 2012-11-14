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
      'weight' => '4',
      'parent' => '',
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
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
      '5' => array(
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
    'premium_fields' => 1,
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
      'label' => 'Audio (natural speed)',
      'weight' => '4',
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
      'weight' => '5',
      'parent' => '',
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
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
      '5' => array(
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
    'type' => 'filefield',
    'required' => '0',
    'multiple' => '0',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'premium_fields' => 1,
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
      'weight' => '5',
      'description' => '',
      'type' => 'filefield_widget',
      'module' => 'filefield',
    ),
  );

  // Exported field: field_content_category
  $fields['content-field_content_category'] = array(
    'field_name' => 'field_content_category',
    'type_name' => 'content',
    'display_settings' => array(
      'weight' => 0,
      'parent' => '',
      '2' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '5' => array(
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
    'required' => '1',
    'multiple' => '1',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => 'Business and Economics
Careers
Community and Society
Culture and Religion
Daily Life
Ecology
Education
Entertainment
Food
Health and Beauty
History
Language
Literature
News and Politics
Music
Other
Poland
Psychology and the Mind
Science and Technology
Shopping
Sports
Travel
USA',
    'allowed_values_php' => '',
    'premium_fields' => 0,
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Category',
      'weight' => 0,
      'description' => '',
      'type' => 'optionwidgets_select',
      'module' => 'optionwidgets',
    ),
  );

  // Exported field: field_content_image
  $fields['content-field_content_image'] = array(
    'field_name' => 'field_content_image',
    'type_name' => 'content',
    'display_settings' => array(
      'weight' => '2',
      'parent' => '',
      '2' => array(
        'format' => 'image_plain',
        'exclude' => 1,
      ),
      '3' => array(
        'format' => 'image_plain',
        'exclude' => 1,
      ),
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
      '5' => array(
        'format' => 'default',
        'exclude' => 0,
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
    'premium_fields' => 0,
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
        'imagecache_cart' => 0,
        'lightbox2--cart--cart' => 0,
        'lightbox2--cart--large' => 0,
        'lightbox2--cart--medium' => 0,
        'lightbox2--cart--original' => 0,
        'lightbox2--cart--product' => 0,
        'lightbox2--cart--product_full' => 0,
        'lightbox2--cart--product_list' => 0,
        'lightbox2--cart--small' => 0,
        'lightbox2--cart--tiny' => 0,
        'lightbox2--cart--uc_thumbnail' => 0,
        'lightbox2--large--cart' => 0,
        'lightbox2--large--large' => 0,
        'lightbox2--large--medium' => 0,
        'lightbox2--large--original' => 0,
        'lightbox2--large--product' => 0,
        'lightbox2--large--product_full' => 0,
        'lightbox2--large--product_list' => 0,
        'lightbox2--large--small' => 0,
        'lightbox2--large--tiny' => 0,
        'lightbox2--large--uc_thumbnail' => 0,
        'lightbox2--link--cart' => 0,
        'lightbox2--link--large' => 0,
        'lightbox2--link--medium' => 0,
        'lightbox2--link--product' => 0,
        'lightbox2--link--product_full' => 0,
        'lightbox2--link--product_list' => 0,
        'lightbox2--link--small' => 0,
        'lightbox2--link--tiny' => 0,
        'lightbox2--link--uc_thumbnail' => 0,
        'lightbox2--medium--cart' => 0,
        'lightbox2--medium--large' => 0,
        'lightbox2--medium--medium' => 0,
        'lightbox2--medium--original' => 0,
        'lightbox2--medium--product' => 0,
        'lightbox2--medium--product_full' => 0,
        'lightbox2--medium--product_list' => 0,
        'lightbox2--medium--small' => 0,
        'lightbox2--medium--tiny' => 0,
        'lightbox2--medium--uc_thumbnail' => 0,
        'lightbox2--original--cart' => 0,
        'lightbox2--original--large' => 0,
        'lightbox2--original--medium' => 0,
        'lightbox2--original--product' => 0,
        'lightbox2--original--product_full' => 0,
        'lightbox2--original--product_list' => 0,
        'lightbox2--original--small' => 0,
        'lightbox2--original--tiny' => 0,
        'lightbox2--original--uc_thumbnail' => 0,
        'lightbox2--product--cart' => 0,
        'lightbox2--product--large' => 0,
        'lightbox2--product--medium' => 0,
        'lightbox2--product--original' => 0,
        'lightbox2--product--product' => 0,
        'lightbox2--product--product_full' => 0,
        'lightbox2--product--product_list' => 0,
        'lightbox2--product--small' => 0,
        'lightbox2--product--tiny' => 0,
        'lightbox2--product--uc_thumbnail' => 0,
        'lightbox2--product_full--cart' => 0,
        'lightbox2--product_full--large' => 0,
        'lightbox2--product_full--medium' => 0,
        'lightbox2--product_full--original' => 0,
        'lightbox2--product_full--product' => 0,
        'lightbox2--product_full--product_full' => 0,
        'lightbox2--product_full--product_list' => 0,
        'lightbox2--product_full--small' => 0,
        'lightbox2--product_full--tiny' => 0,
        'lightbox2--product_full--uc_thumbnail' => 0,
        'lightbox2--product_list--cart' => 0,
        'lightbox2--product_list--large' => 0,
        'lightbox2--product_list--medium' => 0,
        'lightbox2--product_list--original' => 0,
        'lightbox2--product_list--product' => 0,
        'lightbox2--product_list--product_full' => 0,
        'lightbox2--product_list--product_list' => 0,
        'lightbox2--product_list--small' => 0,
        'lightbox2--product_list--tiny' => 0,
        'lightbox2--product_list--uc_thumbnail' => 0,
        'lightbox2--small--cart' => 0,
        'lightbox2--small--large' => 0,
        'lightbox2--small--medium' => 0,
        'lightbox2--small--original' => 0,
        'lightbox2--small--product' => 0,
        'lightbox2--small--product_full' => 0,
        'lightbox2--small--product_list' => 0,
        'lightbox2--small--small' => 0,
        'lightbox2--small--tiny' => 0,
        'lightbox2--small--uc_thumbnail' => 0,
        'lightbox2--tiny--cart' => 0,
        'lightbox2--tiny--large' => 0,
        'lightbox2--tiny--medium' => 0,
        'lightbox2--tiny--original' => 0,
        'lightbox2--tiny--product' => 0,
        'lightbox2--tiny--product_full' => 0,
        'lightbox2--tiny--product_list' => 0,
        'lightbox2--tiny--small' => 0,
        'lightbox2--tiny--tiny' => 0,
        'lightbox2--tiny--uc_thumbnail' => 0,
        'lightbox2--uc_thumbnail--cart' => 0,
        'lightbox2--uc_thumbnail--large' => 0,
        'lightbox2--uc_thumbnail--medium' => 0,
        'lightbox2--uc_thumbnail--original' => 0,
        'lightbox2--uc_thumbnail--product' => 0,
        'lightbox2--uc_thumbnail--product_full' => 0,
        'lightbox2--uc_thumbnail--product_list' => 0,
        'lightbox2--uc_thumbnail--small' => 0,
        'lightbox2--uc_thumbnail--tiny' => 0,
        'lightbox2--uc_thumbnail--uc_thumbnail' => 0,
        'imagecache_product' => 0,
        'imagecache_product_full' => 0,
        'imagecache_product_list' => 0,
        'imagecache_tiny' => 0,
        'imagecache_uc_thumbnail' => 0,
        'wiki' => 0,
      ),
      'insert_default' => 'imagecache_small',
      'insert_class' => '',
      'insert_width' => '',
      'filefield_sources' => array(
        'reference' => 0,
        'remote' => 0,
        'attach' => 0,
      ),
      'filefield_source_autocomplete' => '0',
      'filefield_source_imce_mode' => 0,
      'filefield_source_attach_path' => 'file_attach',
      'filefield_source_attach_absolute' => '0',
      'filefield_source_attach_mode' => 'move',
      'label' => 'Images',
      'weight' => '2',
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Exported field: field_content_level
  $fields['content-field_content_level'] = array(
    'field_name' => 'field_content_level',
    'type_name' => 'content',
    'display_settings' => array(
      'weight' => '-2',
      'parent' => '',
      '2' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '5' => array(
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
    'required' => '1',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => 'beginner|Beginner
intermediate|Intermediate
advanced|Advanced',
    'allowed_values_php' => '',
    'premium_fields' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Level',
      'weight' => '-2',
      'description' => 'What level of learners is this text for?',
      'type' => 'optionwidgets_select',
      'module' => 'optionwidgets',
    ),
  );

  // Exported field: field_content_type
  $fields['content-field_content_type'] = array(
    'field_name' => 'field_content_type',
    'type_name' => 'content',
    'display_settings' => array(
      'weight' => '-1',
      'parent' => '',
      '2' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '5' => array(
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
    'required' => '1',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => 'article|Article
conversation|Conversation / Dialog
interview|Interview
narrative|Narrative story
poetry|Poetry
song|Song',
    'allowed_values_php' => '',
    'premium_fields' => 0,
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Type',
      'weight' => '-1',
      'description' => '',
      'type' => 'optionwidgets_select',
      'module' => 'optionwidgets',
    ),
  );

  // Exported field: field_remote_url
  $fields['content-field_remote_url'] = array(
    'field_name' => 'field_remote_url',
    'type_name' => 'content',
    'display_settings' => array(
      'weight' => '7',
      'parent' => '',
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
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
      '5' => array(
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
    'premium_fields' => '',
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
      'weight' => '7',
      'description' => '',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Audio (natural speed)');
  t('Audio (slower)');
  t('Category');
  t('Images');
  t('Level');
  t('Remote URL');
  t('Type');

  return $fields;
}
