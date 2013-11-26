<?php

function theme_bbcom_flashcards_flashcard_front($node) {
  $entry = LingwoEntry::fromNode($node);
  list ($source_lang, $target_lang) = $entry->getLanguages(TRUE);

  $entry_summary = theme('bbcom_dictionary_entry_summary', $node, FALSE, TRUE);
  $pos = $entry->getPos(TRUE);

  if ($target_lang) {
    $output = $source_lang .' =&gt; '. $target_lang .'<br />';
  }
  else {
    $output = $target_lang;
  }
  $output .= $node->title .' ('. $pos .')<br />';
  $output .= $entry_summary;

  return $output;
}

function theme_bbcom_flashcards_flashcard_back($node) {
  // we have to remove the examples because they will give it away
  // TODO: we should probably make a completely custom generator here, because Anki
  // has different requirements than the web..
  $function = _lingwo_is_translation($node) ?
    'lingwo_senses_translation' : 'lingwo_senses_original';
  return theme($function, $node, array('example' => FALSE, 'contribute' => FALSE));
}

