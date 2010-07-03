<?php

function template_preprocess_lingwoorg_flashcards_flashcard_front(&$vars) {
  $node = $vars['node'];

  list ($source_lang, $target_lang) = lingwo_dictionary_entry_get_languages($node, TRUE);
  $vars['source_lang'] = $source_lang;
  $vars['target_lang'] = $target_lang;

  $vars['entry_summary'] = theme('lingwoorg_dictionary_entry_summary', FALSE, TRUE);
  $vars['pos'] = lingwo_dictionary_entry_get_pos($node);
}

function theme_lingwoorg_flashcards_flashcard_back($node) {
  $senses = lingwo_senses_get_translations($node, 'field_senses', $node->field_senses);
  # we have to remove the examples because they will give it away
  foreach ($senses as &$sense) {
    unset($sense['orig_sense']['data']['example']);
  }

  return theme('lingwo_senses_translation', $node, $senses);
}

