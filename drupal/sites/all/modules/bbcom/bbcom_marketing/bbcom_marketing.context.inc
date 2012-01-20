<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function bbcom_marketing_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'bbcom-marketing-teamspeak';
  $context->description = 'Blocks shown accross the entire site for marketing purposes';
  $context->tag = 'bbcom';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'page' => 'page',
        'story' => 'story',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        '~<front>' => '~<front>',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'bbcom_marketing-teamspeak' => array(
          'module' => 'bbcom_marketing',
          'delta' => 'teamspeak',
          'region' => 'right',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Blocks shown accross the entire site for marketing purposes');
  t('bbcom');

  $export['bbcom-marketing-teamspeak'] = $context;
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
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-all_news-block_1' => array(
          'module' => 'views',
          'delta' => 'all_news-block_1',
          'region' => 'content_bottom',
          'weight' => 0,
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
