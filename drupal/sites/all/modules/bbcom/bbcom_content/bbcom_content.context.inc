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
        0 => 'content',
      ),
      'options' => array(),
    ),
    'path' => array(
      'values' => array(
        'content' => 'content',
        'node/*/annotate' => 'node/*/annotate',
        'content/search' => 'content/search',
      ),
      'options' => array(),
    ),
  );
  $context->reactions = array(
    'menu' => 'content',
    'block' => array(
      'blocks' => array(
        'views--exp-search_content-page' => array(
          'module' => 'views',
          'delta' => '-exp-search_content-page',
          'weight' => 20,
          'region' => 'header',
          'status' => '0',
          'label' => 'Exposed form: search_content-page',
          'type' => 'context_ui',
        ),
        'bbcom_content-0' => array(
          'module' => 'bbcom_content',
          'delta' => '0',
          'weight' => 20,
          'region' => 'right',
          'status' => '0',
          'label' => 'BiblioBird.com Content: Add Content Block',
          'type' => 'context_ui',
        ),
        'lingwo_korpus-0' => array(
          'module' => 'lingwo_korpus',
          'delta' => '0',
          'weight' => 21,
          'region' => 'right',
          'status' => '0',
          'label' => 'Lingwo Korpus: Annotation Reader block',
          'type' => 'context_ui',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Working in the content area.');

  $export['bbcom-section-content'] = $context;
  return $export;
}
