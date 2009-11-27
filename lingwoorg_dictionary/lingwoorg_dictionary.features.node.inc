<?php

/**
 * Helper to implementation of hook_node_info().
 */
function _lingwoorg_dictionary_node_info() {
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
