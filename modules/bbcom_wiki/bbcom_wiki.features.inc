<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function bbcom_wiki_ctools_plugin_api() {
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
function bbcom_wiki_node_info() {
  $items = array(
    'rwiki' => array(
      'name' => t('Restricted Wiki Page'),
      'module' => 'features',
      'description' => t('A protected <em>Wiki page</em>.  We use this for important pages that only trusted members should edit.  For most pages <em>Wiki page</em> instead.  Only restrict pages that control site policy.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'wiki' => array(
      'name' => t('Wiki Page'),
      'module' => 'features',
      'description' => t('A new page in the wiki!'),
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
