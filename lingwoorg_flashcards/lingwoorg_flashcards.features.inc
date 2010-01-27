<?php

/**
 * Implementation of hook_content_default_fields().
 */
function lingwoorg_flashcards_content_default_fields() {
  module_load_include('inc', 'lingwoorg_flashcards', 'lingwoorg_flashcards.defaults');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_flashcards_content_default_fields', $args);
}

/**
 * Implementation of hook_node_info().
 */
function lingwoorg_flashcards_node_info() {
  module_load_include('inc', 'lingwoorg_flashcards', 'lingwoorg_flashcards.features.node');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_flashcards_node_info', $args);
}

/**
 * Helper to implementation of hook_enable().
 */
function _lingwoorg_flashcards_enable() {

}

/**
 * Helper to implementation of hook_disable().
 */
function _lingwoorg_flashcards_disable() {

}
