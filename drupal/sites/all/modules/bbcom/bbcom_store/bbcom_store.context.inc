<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function bbcom_store_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'bbcom-section-store';
  $context->description = 'Stuff pertaining to the for-sale products on Bibliobird.';
  $context->tag = '';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'package' => 'package',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array();
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Stuff pertaining to the for-sale products on Bibliobird.');
  $export['bbcom-section-store'] = $context;

  return $export;
}
