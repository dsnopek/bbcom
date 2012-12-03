<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function bbcom_wiki_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'bbcom-section-wiki';
  $context->description = 'Working in the wiki.';
  $context->tag = '';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'wiki' => 'wiki',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
    'path' => array(
      'values' => array(
        'wiki' => 'wiki',
        'wiki/*' => 'wiki/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'bbcom_wiki-0' => array(
          'module' => 'bbcom_wiki',
          'delta' => 0,
          'region' => 'right',
          'weight' => 0,
        ),
      ),
    ),
    'menu' => 'wiki',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Working in the wiki.');
  $export['bbcom-section-wiki'] = $context;

  return $export;
}
