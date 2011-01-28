<?php

/**
 * Implementation of hook_strongarm().
 */
function bbcom_dictionary_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_entry';
  $strongarm->value = array(
    'language' => 0,
    'pos' => 1,
    'title' => 2,
    'menu' => 3,
    'lingwo_senses' => 4,
    'lingwo_fields' => 5,
    'flag' => 6,
    'revision_information' => 7,
    'comment_settings' => 9,
    'translation' => 10,
  );

  $export['content_extra_weights_entry'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_entry';
  $strongarm->value = 1;

  $export['i18n_node_entry'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_entry';
  $strongarm->value = 2;

  $export['language_content_type_entry'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'lingwo_data_field_source_copyright';
  $strongarm->value = 'field_source_copyright';

  $export['lingwo_data_field_source_copyright'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'lingwo_data_field_source_license';
  $strongarm->value = 'field_source_license';

  $export['lingwo_data_field_source_license'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'lingwo_data_field_source_name';
  $strongarm->value = 'field_source_name';

  $export['lingwo_data_field_source_name'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'lingwo_data_field_source_raw';
  $strongarm->value = 'field_source_raw';

  $export['lingwo_data_field_source_raw'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'lingwo_data_field_source_timestamp';
  $strongarm->value = 'field_source_timestamp';

  $export['lingwo_data_field_source_timestamp'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'lingwo_data_field_source_url';
  $strongarm->value = 'field_source_url';

  $export['lingwo_data_field_source_url'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'lingwo_dictionary_entry_content_type';
  $strongarm->value = 'entry';

  $export['lingwo_dictionary_entry_content_type'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'lingwo_dictionary_lookup_path';
  $strongarm->value = 'dictionary/lookup';

  $export['lingwo_dictionary_lookup_path'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'lingwo_dictionary_pos_allowed_values';
  $strongarm->value = 'abbreviation|Abbreviation
adjective|Adjective
adverb|Adverb
article|Article
auxilary verb|Auxilary Verb
conjunction|Conjunction
determiner|Determiner
exclamation|Exclamation
noun|Noun
onomatopoeia|Onomatopoeia
particle|Particle
phrase|Phrase
prefix|Prefix
preposition|Preposition
pronoun|Pronoun
suffix|Suffix
symbol|Symbol
title|Title
verb|Verb';

  $export['lingwo_dictionary_pos_allowed_values'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_entry';
  $strongarm->value = array(
    0 => 'status',
    1 => 'revision',
  );

  $export['node_options_entry'] = $strongarm;
  return $export;
}
