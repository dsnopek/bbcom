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
 * Implementation of hook_views_default_views().
 */
function lingwoorg_flashcards_views_default_views() {
  module_load_include('inc', 'lingwoorg_flashcards', 'lingwoorg_flashcards.features.views');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_flashcards_views_default_views', $args);
}
