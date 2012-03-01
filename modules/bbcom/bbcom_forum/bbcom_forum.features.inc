<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function bbcom_forum_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
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
