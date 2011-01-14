<?php

function theme_bbcom_flashcards_flashcard_front($node) {
  $entry = LingwoEntry::fromNode($node);
  list ($source_lang, $target_lang) = $entry->getLanguages(TRUE);

  $entry_summary = theme('lingwoorg_dictionary_entry_summary', FALSE, TRUE);
  $pos = $entry->getPos(TRUE);

  $output  = $source_lang .' =&gt; '. $target_lang .'<br />';
  $output .= $node->title .' ('. $pos .')<br />';
  $output .= $entry_summary;

  return $output;
}

function theme_bbcom_flashcards_flashcard_back($node) {
  $senses = lingwo_senses_get_translations($node);
  # we have to remove the examples because they will give it away
  foreach ($senses as &$sense) {
    unset($sense['data']['example_translation']);
    unset($sense['orig_sense']['data']['example']);
  }

  return theme('lingwo_senses_translation', $node, $senses);
}

