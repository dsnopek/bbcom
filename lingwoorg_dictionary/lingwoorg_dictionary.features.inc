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

/**
 * Implementation of hook_taxonomy_default_terms().
 */
function lingwoorg_dictionary_taxonomy_default_terms() {
  module_load_include('inc', 'lingwoorg_dictionary', 'lingwoorg_dictionary.features.taxonomy_term');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_dictionary_taxonomy_default_terms', $args);
}

/**
 * Implementation of hook_enable_taxonomy_term().
 */
function lingwoorg_dictionary_enable_taxonomy_term() {
  module_load_include('inc', 'lingwoorg_dictionary', 'lingwoorg_dictionary.features.taxonomy_term');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_dictionary_enable_taxonomy_term', $args);
}

/**
 * Implementation of hook_disable_taxonomy_term().
 */
function lingwoorg_dictionary_disable_taxonomy_term() {
  module_load_include('inc', 'lingwoorg_dictionary', 'lingwoorg_dictionary.features.taxonomy_term');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_dictionary_disable_taxonomy_term', $args);
}

/**
 * Implementation of hook_taxonomy_default_vocabularies().
 */
function lingwoorg_dictionary_taxonomy_default_vocabularies() {
  module_load_include('inc', 'lingwoorg_dictionary', 'lingwoorg_dictionary.features.taxonomy_vocabulary');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_dictionary_taxonomy_default_vocabularies', $args);
}

/**
 * Implementation of hook_enable_taxonomy_vocabulary().
 */
function lingwoorg_dictionary_enable_taxonomy_vocabulary() {
  module_load_include('inc', 'lingwoorg_dictionary', 'lingwoorg_dictionary.features.taxonomy_vocabulary');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_dictionary_enable_taxonomy_vocabulary', $args);
}

/**
 * Implementation of hook_disable_taxonomy_vocabulary().
 */
function lingwoorg_dictionary_disable_taxonomy_vocabulary() {
  module_load_include('inc', 'lingwoorg_dictionary', 'lingwoorg_dictionary.features.taxonomy_vocabulary');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_dictionary_disable_taxonomy_vocabulary', $args);
}

/**
 * Implementation of hook_user_default_permissions().
 */
function lingwoorg_dictionary_user_default_permissions() {
  module_load_include('inc', 'lingwoorg_dictionary', 'lingwoorg_dictionary.defaults');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_dictionary_user_default_permissions', $args);
}

/**
 * Helper to implementation of hook_enable().
 */
function _lingwoorg_dictionary_enable() {
  lingwoorg_dictionary_enable_taxonomy_vocabulary();
  lingwoorg_dictionary_enable_taxonomy_term();
}

/**
 * Helper to implementation of hook_disable().
 */
function _lingwoorg_dictionary_disable() {
  lingwoorg_dictionary_disable_taxonomy_term();
  lingwoorg_dictionary_disable_taxonomy_vocabulary();
}
