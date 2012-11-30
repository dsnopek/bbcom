<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function bbcom_content_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'bbcom-section-content';
  $context->description = 'Working in the content area.';
  $context->tag = '';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'content' => 'content',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
    'path' => array(
      'values' => array(
        'content' => 'content',
        'node/*/annotate' => 'node/*/annotate',
        'content/search' => 'content/search',
      ),
    ),
    'views' => array(
      'values' => array(
        'recent_content' => 'recent_content',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views--exp-search_content-page' => array(
          'module' => 'views',
          'delta' => '-exp-search_content-page',
          'region' => 'header',
          'weight' => 0,
        ),
        'bbcom_content-0' => array(
          'module' => 'bbcom_content',
          'delta' => 0,
          'region' => 'right',
          'weight' => 0,
        ),
        'lingwo_korpus-0' => array(
          'module' => 'lingwo_korpus',
          'delta' => 0,
          'region' => 'right',
          'weight' => 1,
        ),
        'book-0' => array(
          'module' => 'book',
          'delta' => 0,
          'region' => 'right',
          'weight' => 2,
        ),
        'views-entries_need_work-block_1' => array(
          'module' => 'views',
          'delta' => 'entries_need_work-block_1',
          'region' => 'right',
          'weight' => 3,
        ),
      ),
    ),
    'menu' => 'content',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Working in the content area.');

  $export['bbcom-section-content'] = $context;
  return $export;
}
