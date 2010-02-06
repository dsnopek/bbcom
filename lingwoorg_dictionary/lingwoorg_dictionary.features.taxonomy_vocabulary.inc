<?php

/**
 * Helper to implementation of hook_taxonomy_default_vocabularies().
 */
function _lingwoorg_dictionary_taxonomy_default_vocabularies() {
  $items = array(
  array(
      'name' => 'Part of Speech',
      'description' => '',
      'help' => '',
      'relations' => '1',
      'hierarchy' => '0',
      'multiple' => '0',
      'required' => '1',
      'tags' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
      'language' => '',
      'nodes' => array(
        'entry' => 'entry',
      ),
      'machine' => 'pos',
    ),
  );
  return $items;
}
