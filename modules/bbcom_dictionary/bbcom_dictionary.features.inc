<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function bbcom_dictionary_context_default_contexts() {
  $items = array();

  $items['bbcom-section-dictionary'] = array(
    'namespace' => 'bbcom',
    'attribute' => 'section',
    'value' => 'dictionary',
    'description' => 'Working in the dictionary',
    'node' => array(
      '0' => 'entry',
    ),
    'path' => array(
      'dictionary' => 'dictionary',
      'dictionary/*' => 'dictionary/*',
    ),
    'menu' => 'dictionary',
    'block' => array(
      'lingwo_dictionary_0' => array(
        'module' => 'lingwo_dictionary',
        'delta' => '0',
        'weight' => 20,
        'region' => 'header',
        'status' => '0',
        'label' => 'Lingwo Dictionary: Search block',
        'type' => 'context_ui',
      ),
      'bbcom_dictionary_0' => array(
        'module' => 'bbcom_dictionary',
        'delta' => '0',
        'weight' => -21,
        'region' => 'right',
        'status' => '0',
        'label' => 'Lingwo Dictionary: Add Content Block',
        'type' => 'context_ui',
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_ctools_plugin_api().
 */
function bbcom_dictionary_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function bbcom_dictionary_node_info() {
  $items = array(
    'entry' => array(
      'name' => t('Entry'),
      'module' => 'features',
      'description' => t('A dictionary entry.'),
      'has_title' => '1',
      'title_label' => t('Headword'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function bbcom_dictionary_views_api() {
  return array(
    'api' => '2',
  );
}
