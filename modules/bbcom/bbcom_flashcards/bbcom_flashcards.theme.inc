<?php

function theme_bbcom_flashcards_flashcard_front($node) {
  $entry = LingwoEntry::fromNode($node);
  list ($source_lang, $target_lang) = $entry->getLanguages(TRUE);

  $entry_summary = theme('bbcom_dictionary_entry_summary', FALSE, TRUE);
  $pos = $entry->getPos(TRUE);

  $output  = $source_lang .' =&gt; '. $target_lang .'<br />';
  $output .= $node->title .' ('. $pos .')<br />';
  $output .= $entry_summary;

  return $output;
}

function theme_bbcom_flashcards_flashcard_back($node) {
  // we have to remove the examples because they will give it away
  // TODO: we should probably make a completely custom generator here, because Anki
  // has different requirements than the web..
  return theme('lingwo_senses_translation', $node, array('example' => FALSE, 'contribute' => FALSE));
}

