<?php

/**
 * Helper to implementation of hook_node_info().
 */
function _lingwoorg_content_node_info() {
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
