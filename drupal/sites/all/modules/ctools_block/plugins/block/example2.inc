<?php
/**
 * Plugin code for a simple block with semi-dynamic content.
 */

$plugin = array(
  'info' => 'Test Block 2',
  'subject string' => 'Test Block 2 subject',
  'content' => 'ctools_block_example_block_example2_content',
);

/**
 * Provide our semi-dynamic content.
 */
function ctools_block_example_block_example2_content() {
  return t('Test Block 2 output');
}
