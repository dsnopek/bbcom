<?php

function theme_bbcom_flashcards_flashcard_front($node) {
  list ($source_lang, $target_lang) = lingwo_dictionary_entry_get_languages($node, TRUE);

  $entry_summary = theme('lingwoorg_dictionary_entry_summary', FALSE, TRUE);
  $pos = lingwo_dictionary_entry_get_pos($node);

  $output  = $source_lang .' =&gt; '. $target_lang .'<br />';
  $output .= $node->title .' ('. $pos .')<br />';
  $output .= $entry_summary;

  return $output;
}

function theme_bbcom_flashcards_flashcard_back($node) {
  $senses = lingwo_senses_get_translations($node);
  # we have to remove the examples because they will give it away
  foreach ($senses as &$sense) {
    unset($sense['orig_sense']['data']['example']);
  }

  return theme('lingwo_senses_translation', $node, $senses);
}

