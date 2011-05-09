<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function bbcom_dictionary_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'bbcom-section-dictionary';
  $context->description = 'Working in the dictionary';
  $context->tag = '';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        0 => 'entry',
      ),
      'options' => array(),
    ),
    'path' => array(
      'values' => array(
        'dictionary' => 'dictionary',
        'dictionary/*' => 'dictionary/*',
      ),
      'options' => array(),
    ),
  );
  $context->reactions = array(
    'menu' => 'dictionary',
    'block' => array(
      'blocks' => array(
        'lingwo_entry-0' => array(
          'module' => 'lingwo_entry',
          'delta' => '0',
          'weight' => 20,
          'region' => 'header',
          'status' => '0',
          'label' => 'Lingwo Dictionary: Search block',
          'type' => 'context_ui',
        ),
        'bbcom_dictionary-0' => array(
          'module' => 'bbcom_dictionary',
          'delta' => '0',
          'weight' => -21,
          'region' => 'right',
          'status' => '0',
          'label' => 'BiblioBird.com Dictionary: Add Content Block',
          'type' => 'context_ui',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Working in the dictionary');

  $export['bbcom-section-dictionary'] = $context;
  return $export;
}