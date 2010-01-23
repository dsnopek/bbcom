<?php

/**
 * Helper to implementation of hook_taxonomy_default_terms().
 */
function _lingwoorg_dictionary_taxonomy_default_terms() {
  $items = array(
  array(
      'vid' => '15',
      'name' => 'Abbreviation',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'abbreviation',
    ),
  array(
      'vid' => '15',
      'name' => 'Adjective',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'adjective',
    ),
  array(
      'vid' => '15',
      'name' => 'Adverb',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'adverb',
    ),
  array(
      'vid' => '15',
      'name' => 'Conjunction',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'conjunction',
    ),
  array(
      'vid' => '15',
      'name' => 'Exclamation',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'exclamation',
    ),
  array(
      'vid' => '15',
      'name' => 'Noun',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'noun',
    ),
  array(
      'vid' => '15',
      'name' => 'Onomatopoeia',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'onomatopoeia',
    ),
  array(
      'vid' => '15',
      'name' => 'Particle',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'particle',
    ),
  array(
      'vid' => '15',
      'name' => 'Phrase',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'phrase',
    ),
  array(
      'vid' => '15',
      'name' => 'Prefix',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'prefix',
    ),
  array(
      'vid' => '15',
      'name' => 'Preposition',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'preposition',
    ),
  array(
      'vid' => '15',
      'name' => 'Pronoun',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'pronoun',
    ),
  array(
      'vid' => '15',
      'name' => 'Suffix',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'suffix',
    ),
  array(
      'vid' => '15',
      'name' => 'Symbol',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'symbol',
    ),
  array(
      'vid' => '15',
      'name' => 'Title',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'title',
    ),
  array(
      'vid' => '15',
      'name' => 'Verb',
      'description' => '',
      'weight' => '0',
      'language' => '',
      'trid' => '0',
      'vocabulary' => 'pos',
      'machine' => 'verb',
    ),
  );
  return $items;
}

/**
 * Helper to implementation of hook_enable_taxonomy_term().
 */
function _lingwoorg_dictionary_enable_taxonomy_term() {
  exportables_sync('taxonomy_term', 'lingwoorg_dictionary');
}

/**
 * Helper to implementation of hook_disable_taxonomy_term().
 */
function _lingwoorg_dictionary_disable_taxonomy_term() {
  exportables_unsync('taxonomy_term', 'lingwoorg_dictionary');
}