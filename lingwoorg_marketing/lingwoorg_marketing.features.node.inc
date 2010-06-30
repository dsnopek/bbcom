<?php

/**
 * Helper to implementation of hook_node_info().
 */
function _lingwoorg_marketing_node_info() {
  $items = array(
    'story' => array(
      'name' => t('Story'),
      'module' => 'features',
      'description' => t('For information about us.  This is a "marketing story" as it were, to explain our product to customers, etc..'),
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
