<?php
/**
 * Plugin code for a simple block with semi-dynamic subject and content.
 */

$plugin = array(
  'info' => 'Test Block',
  'subject' => 'ctools_block_example_block_example1_subject',
  'content' => 'ctools_block_example_block_example1_content',
);

/**
 * Provide our semi-dynamic subject.
 */
function ctools_block_example_block_example1_subject() {
  return t('Test Block 1 subject');
}

/**
 * Provide our semi-dynamic content.
 */
function ctools_block_example_block_example1_content() {
  return t('Test Block 1 output');
}
