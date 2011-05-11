<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function bbcom_forum_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'bbcom-section-forum';
  $context->description = 'The forum context.';
  $context->tag = '';
  $context->conditions = array(
    'menu' => array(
      'values' => array(
        'forum' => 'forum',
      ),
    ),
    'node' => array(
      'values' => array(
        'forum' => 'forum',
        'forum_reply' => 'forum_reply',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
    'path' => array(
      'values' => array(
        'forum' => 'forum',
        'forum/*' => 'forum/*',
        'node/add/forum-reply/*' => 'node/add/forum-reply/*',
      ),
    ),
  );
  $context->reactions = array(
    'menu' => 'forum',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('The forum context.');

  $export['bbcom-section-forum'] = $context;
  return $export;
}
