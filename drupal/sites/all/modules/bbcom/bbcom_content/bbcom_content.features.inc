<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function bbcom_content_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_flag_default_flags().
 */
function bbcom_content_flag_default_flags() {
  $flags = array();
  // Exported flag: "Promoted content".
  $flags['promoted'] = array(
    'content_type' => 'node',
    'title' => 'Promoted content',
    'global' => '0',
    'types' => array(
      '0' => 'content',
    ),
    'flag_short' => 'Promote this content',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Demote this content',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        '0' => '3',
      ),
      'unflag' => array(
        '0' => '3',
      ),
    ),
    'weight' => 0,
    'show_on_page' => 0,
    'show_on_teaser' => 0,
    'show_on_form' => 1,
    'access_author' => '',
    'i18n' => '0',
    'flag_confirmation' => '',
    'unflag_confirmation' => '',
    'api_version' => 2,
    'module' => 'bbcom_content',
    'locked' => array(
      '0' => 'name',
    ),
  );
  return $flags;

}

/**
 * Implementation of hook_node_info().
 */
function bbcom_content_node_info() {
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
function bbcom_content_views_api() {
  return array(
    'api' => '2',
  );
}
