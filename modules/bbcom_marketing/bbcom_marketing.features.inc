<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function bbcom_marketing_context_default_contexts() {
  $items = array();

  $items['lingwoorg-section-marketing'] = array(
    'namespace' => 'lingwoorg',
    'attribute' => 'section',
    'value' => 'marketing',
    'description' => 'Viewing our marketing materials.',
    'path' => array(
      'about' => 'about',
    ),
    'block' => array(
      'views_latest_news-block_1' => array(
        'module' => 'views',
        'delta' => 'latest_news-block_1',
        'weight' => 18,
        'region' => 'content_top',
        'status' => '0',
        'label' => 'Story Slideshow',
        'type' => 'context_ui',
      ),
      'views_all_news-block_1' => array(
        'module' => 'views',
        'delta' => 'all_news-block_1',
        'weight' => 18,
        'region' => 'content_bottom',
        'status' => '0',
        'label' => 'Aktualności',
        'type' => 'context_ui',
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_ctools_plugin_api().
 */
function bbcom_marketing_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function bbcom_marketing_node_info() {
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
function bbcom_marketing_views_api() {
  return array(
    'api' => '2',
  );
}
