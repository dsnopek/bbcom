<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function lingwoorg_content_context_default_contexts() {
  $items = array();

  $items['lingwoorg-section-content'] = array(
    'namespace' => 'lingwoorg',
    'attribute' => 'section',
    'value' => 'content',
    'description' => 'Working in the content area.',
    'node' => array(
      '0' => 'content',
    ),
    'path' => array(
      '<front>' => '<front>',
      'node/*/annotate' => 'node/*/annotate',
      'content/search' => 'content/search',
    ),
    'menu' => 'content',
    'block' => array(
      'views_-exp-search_content-page' => array(
        'module' => 'views',
        'delta' => '-exp-search_content-page',
        'weight' => 18,
        'region' => 'header',
        'status' => '0',
        'label' => 'Exposed form: search_content-page',
        'type' => 'context_ui',
      ),
      'lingwoorg_content_0' => array(
        'module' => 'lingwoorg_content',
        'delta' => '0',
        'weight' => 18,
        'region' => 'right',
        'status' => '0',
        'label' => 'Lingwo.org Content: Add Content Block',
        'type' => 'context_ui',
      ),
      'lingwo_korpus_0' => array(
        'module' => 'lingwo_korpus',
        'delta' => '0',
        'weight' => 19,
        'region' => 'right',
        'status' => '0',
        'label' => 'Lingwo Korpus: Annotation Reader block',
        'type' => 'context_ui',
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_ctools_plugin_api().
 */
function lingwoorg_content_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function lingwoorg_content_node_info() {
  $items = array(
    'content' => array(
      'name' => t('Content'),
      'module' => 'features',
      'description' => t('A piece of content used to study.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Content'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function lingwoorg_content_views_api() {
  return array(
    'api' => '2',
  );
}
