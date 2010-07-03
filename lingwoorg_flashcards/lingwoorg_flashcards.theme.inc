<?php

function theme_lingwoorg_flashcards_flashcard_front($node) {
  // TODO: we shouldn't do this!!
  $node = node_build_content($node, FALSE, TRUE);

  $allowed_tags = array('em','strong','ul','ol','li');

  $front .= implode(' => ', lingwo_dictionary_entry_get_languages($node, TRUE)) ."\n";
  $front .= $node->title ." (". lingwo_dictionary_entry_get_pos($node) .")\n";
  // TODO: fer real, we need to not abuse node_build_content() and just call the function
  // to build this.
  $front .= filter_xss(drupal_render($node->content['entry_summary']), $allowed_tags);

  return $front;
}

function theme_lingwoorg_flashcards_flashcard_back($node) {
  // TODO: we shouldn't do this!!
  $node = node_build_content($node, FALSE, TRUE);

  $allowed_tags = array('em','strong','ul','ol','li');

  // TODO: we want to get the examples on the front of the card, and only the translations
  // on the back of the card
  // TODO: we need to stop cheating by re-using the original theme!  A new theme 
  // function or template would be cool though.
  return trim(filter_xss(drupal_render($node->content['field_senses']), $allowed_tags));
}

