<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function bbcom_profile_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function bbcom_profile_node_info() {
  $items = array(
    'profile' => array(
      'name' => t('Profile'),
      'module' => 'features',
      'description' => t('A user profile built as content.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'has_body' => '1',
      'body_label' => t('About me'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
