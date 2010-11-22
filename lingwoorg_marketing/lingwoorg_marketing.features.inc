<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function lingwoorg_marketing_context_default_contexts() {
  $items = array();

  $items['lingwoorg-section-marketing'] = array(
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
        'weight' => 15,
        'region' => 'content_top',
        'status' => '0',
        'label' => 'Story Slideshow',
        'type' => 'context_ui',
      ),
      'webformblock_1449' => array(
        'module' => 'webformblock',
        'delta' => '1449',
        'weight' => 15,
        'region' => 'content_right',
        'status' => '0',
        'label' => 'Webform: Contact Us',
        'type' => 'context_ui',
      ),
      'views_all_news-block_1' => array(
        'module' => 'views',
        'delta' => 'all_news-block_1',
        'weight' => 15,
        'region' => 'content_bottom',
        'status' => '0',
        'label' => 'All News',
        'type' => 'context_ui',
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_node_info().
 */
function lingwoorg_marketing_node_info() {
  $items = array(
    'story' => array(
      'name' => t('Story'),
      'module' => 'features',
      'description' => t('For information about us.  This is a "marketing story" as it were, to explain our product to customers, etc..'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function lingwoorg_marketing_views_api() {
  return array(
    'api' => '2',
  );
}
