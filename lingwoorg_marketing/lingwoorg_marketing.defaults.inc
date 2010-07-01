<?php

/**
 * Helper to implementation of hook_content_default_fields().
 */
function _lingwoorg_marketing_content_default_fields() {
  $fields = array();

  // Exported field: field_emvideo
  $fields[] = array(
    'field_name' => 'field_emvideo',
    'type_name' => 'story',
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
    'type' => 'emvideo',
    'required' => '0',
    'multiple' => '0',
    'module' => 'emvideo',
    'active' => '1',
    'widget' => array(
      'video_width' => '425',
      'video_height' => '290',
      'video_autoplay' => 0,
      'preview_width' => '425',
      'preview_height' => '290',
      'preview_autoplay' => 0,
      'thumbnail_width' => '120',
      'thumbnail_height' => '90',
      'thumbnail_default_path' => '',
      'thumbnail_link_title' => 'See video',
      'providers' => array(
        'archive' => 0,
        'bliptv' => 0,
        'dailymotion' => 0,
        'google' => 0,
        'guba' => 0,
        'imeem' => 0,
        'lastfm' => 0,
        'livevideo' => 0,
        'metacafe' => 0,
        'myspace' => 0,
        'revver' => 0,
        'sevenload' => 0,
        'spike' => 0,
        'tudou' => 0,
        'twistage' => 0,
        'ustream' => 0,
        'ustreamlive' => 0,
        'vimeo' => 0,
        'voicethread' => 0,
        'yahoomusic' => 0,
        'youtube' => 0,
        'zzz_custom_url' => 0,
      ),
      'default_value' => array(
        '0' => array(
          'embed' => '',
          'value' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Video',
      'weight' => '21',
      'description' => '',
      'type' => 'emvideo_textfields',
      'module' => 'emvideo',
    ),
  );

  // Translatables
  array(
    t('Video'),
  );

  return $fields;
}

/**
 * Helper to implementation of hook_context_default_contexts().
 */
function _lingwoorg_marketing_context_default_contexts() {
  $items = array();

  $items[] = array(
    'namespace' => 'lingwoorg',
    'attribute' => 'section',
    'value' => 'marketing',
    'description' => 'Viewing our marketing materials.',
    'path' => array(
      'about' => 'about',
    ),
    'menu_trail' => array(
      'about' => 'about',
    ),
    'block' => array(
      'views_latest_news-block_1' => array(
        'module' => 'views',
        'delta' => 'latest_news-block_1',
        'weight' => 16,
        'region' => 'content_top',
        'status' => '0',
        'label' => 'Story Slideshow',
        'type' => 'context_ui',
      ),
      'webformblock_1431' => array(
        'module' => 'webformblock',
        'delta' => '1431',
        'weight' => 16,
        'region' => 'content_right',
        'status' => '0',
        'label' => 'Webform: Contact Us',
        'type' => 'context_ui',
      ),
      'views_all_news-block_1' => array(
        'module' => 'views',
        'delta' => 'all_news-block_1',
        'weight' => 16,
        'region' => 'content_bottom',
        'status' => '0',
        'label' => 'All News',
        'type' => 'context_ui',
      ),
    ),
  );
  return $items;
}
