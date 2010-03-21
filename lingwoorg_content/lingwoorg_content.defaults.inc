<?php

/**
 * Helper to implementation of hook_context_default_contexts().
 */
function _lingwoorg_content_context_default_contexts() {
  $items = array();

  $items[] = array(
    'namespace' => 'lingwoorg',
    'attribute' => 'section',
    'value' => 'content',
    'description' => 'Working in the content area.',
    'node' => array(
      '0' => 'content',
    ),
    'path' => array(
      '<front>' => '<front>',
    ),
    'menu' => '<front>',
    'block' => array(
      'lingwoorg_content_0' => array(
        'module' => 'lingwoorg_content',
        'delta' => '0',
        'weight' => 16,
        'region' => 'right',
        'status' => '0',
        'label' => 'Lingwo.org Content: Add Content Block',
        'type' => 'context_ui',
      ),
    ),
  );
  return $items;
}