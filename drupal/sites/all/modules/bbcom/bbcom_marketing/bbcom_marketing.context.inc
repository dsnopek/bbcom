<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function bbcom_marketing_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'bbcom-section-marketing';
  $context->description = 'Viewing our marketing materials.';
  $context->tag = '';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'about' => 'about',
      ),
      'options' => array(),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-all_news-block_1' => array(
          'module' => 'views',
          'delta' => 'all_news-block_1',
          'weight' => 18,
          'region' => 'content_bottom',
          'status' => '0',
          'label' => 'All News',
          'type' => 'context_ui',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Viewing our marketing materials.');

  $export['bbcom-section-marketing'] = $context;
  return $export;
}
