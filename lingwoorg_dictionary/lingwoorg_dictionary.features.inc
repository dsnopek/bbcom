<?php

/**
 * Implementation of hook_content_default_fields().
 */
function lingwoorg_dictionary_content_default_fields() {
  module_load_include('inc', 'lingwoorg_dictionary', 'lingwoorg_dictionary.defaults');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_dictionary_content_default_fields', $args);
}

/**
 * Implementation of hook_context_default_contexts().
 */
function lingwoorg_dictionary_context_default_contexts() {
  module_load_include('inc', 'lingwoorg_dictionary', 'lingwoorg_dictionary.defaults');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_dictionary_context_default_contexts', $args);
}

/**
 * Implementation of hook_node_info().
 */
function lingwoorg_dictionary_node_info() {
  module_load_include('inc', 'lingwoorg_dictionary', 'lingwoorg_dictionary.features.node');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_dictionary_node_info', $args);
}
