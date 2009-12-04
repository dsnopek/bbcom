<?php

/**
 * Helper to implementation of hook_taxonomy_default_terms().
 */
function _lingwoorg_dictionary_taxonomy_default_terms() {
  $items = array(
  array(
      'vid' => '5',
      'name' => 'Adjective',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'adjective',
    ),
  array(
      'vid' => '5',
      'name' => 'Noun',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'noun',
    ),
  array(
      'vid' => '5',
      'name' => 'Verb',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'verb',
    ),
  );
  return $items;
}

/**
 * Helper to implementation of hook_enable_taxonomy_term().
 */
function _lingwoorg_dictionary_enable_taxonomy_term() {
  exportables_sync('taxonomy_term', 'lingwoorg_dictionary');
}

/**
 * Helper to implementation of hook_disable_taxonomy_term().
 */
function _lingwoorg_dictionary_disable_taxonomy_term() {
  exportables_unsync('taxonomy_term', 'lingwoorg_dictionary');
}
