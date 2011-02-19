<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function bbcom_forum_context_default_contexts() {
  $items = array();

  $items['bbcom-section-forum'] = array(
    'namespace' => 'bbcom',
    'attribute' => 'section',
    'value' => 'forum',
    'description' => 'The forum context.',
    'node' => array(
      '0' => 'forum_reply',
      '1' => 'forum',
    ),
    'path' => array(
      'forum' => 'forum',
      'forum/*' => 'forum/*',
    ),
    'menu_trail' => array(
      'forum' => 'forum',
    ),
    'menu' => 'forum',
  );
  return $items;
}

/**
 * Implementation of hook_ctools_plugin_api().
 */
function bbcom_forum_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function bbcom_forum_node_info() {
  $items = array(
    'forum_reply' => array(
      'name' => t('Forum reply'),
      'module' => 'features',
      'description' => t('A reply to a form topic.'),
      'has_title' => '1',
      'title_label' => t('Subject'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
