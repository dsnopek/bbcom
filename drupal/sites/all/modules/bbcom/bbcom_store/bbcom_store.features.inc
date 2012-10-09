<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function bbcom_store_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_uc_product_default_classes().
 */
function bbcom_store_uc_product_default_classes() {
  $items = array(
    'package' => array(
      'name' => t('Package'),
      'module' => 'uc_product',
      'description' => t('A paid package of texts.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'has_body' => '1',
      'body_label' => t('Description'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function bbcom_store_views_api() {
  return array(
    'api' => '2',
  );
}
