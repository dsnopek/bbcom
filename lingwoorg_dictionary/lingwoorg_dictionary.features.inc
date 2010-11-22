<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function lingwoorg_dictionary_context_default_contexts() {
  $items = array();

  $items['lingwoorg-section-dictionary'] = array(
    'namespace' => 'lingwoorg',
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
        'weight' => -17,
        'region' => 'header',
        'status' => '0',
        'label' => 'Lingwo Dictionary: Search block',
        'type' => 'context_ui',
      ),
      'lingwoorg_dictionary_0' => array(
        'module' => 'lingwoorg_dictionary',
        'delta' => '0',
        'weight' => 16,
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
 * Implementation of hook_node_info().
 */
function lingwoorg_dictionary_node_info() {
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
function lingwoorg_dictionary_views_api() {
  return array(
    'api' => '2',
  );
}
