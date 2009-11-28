<?php

/**
 * Helper to implementation of hook_taxonomy_default_terms().
 */
function _lingwoorg_dictionary_taxonomy_default_terms() {
  $items = array(
  array(
      'vid' => '3',
      'name' => 'Adjective',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'adjective',
    ),
  array(
      'vid' => '3',
      'name' => 'Noun',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'noun',
    ),
  array(
      'vid' => '3',
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
 * Helper to implementation of hook_enable().
 */
function _lingwoorg_dictionary_enable() {
  exportables_sync('taxonomy_term', 'lingwoorg_dictionary');
}

/**
 * Helper to implementation of hook_disable().
 */
function _lingwoorg_dictionary_disable() {
  exportables_unsync('taxonomy_term', 'lingwoorg_dictionary');
}
