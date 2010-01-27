<?php

/**
 * Helper to implementation of hook_node_info().
 */
function _lingwoorg_flashcards_node_info() {
  $items = array(
    'deck' => array(
      'name' => t('Flashcard Deck'),
      'module' => 'features',
      'description' => t('A deck of flashcards.'),
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
