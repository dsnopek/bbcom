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
        'entry' => 'entry',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'dictionary' => 'dictionary',
        'dictionary/*' => 'dictionary/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'lingwo_entry-0' => array(
          'module' => 'lingwo_entry',
          'delta' => 0,
          'region' => 'header',
          'weight' => 0,
        ),
        'bbcom_dictionary-0' => array(
          'module' => 'bbcom_dictionary',
          'delta' => 0,
          'region' => 'right',
          'weight' => 0,
        ),
        'views-entries_need_work-block_1' => array(
          'module' => 'views',
          'delta' => 'entries_need_work-block_1',
          'region' => 'right',
          'weight' => 1,
        ),
        'views-entries_recent_comments-block_1' => array(
          'module' => 'views',
          'delta' => 'entries_recent_comments-block_1',
          'region' => 'right',
          'weight' => 2,
        ),
      ),
    ),
    'menu' => 'dictionary',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Working in the dictionary');

  $export['bbcom-section-dictionary'] = $context;
  return $export;
}
