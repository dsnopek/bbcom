<?php

/**
 * Implementation of hook_content_default_fields().
 */
function lingwoorg_marketing_content_default_fields() {
  module_load_include('inc', 'lingwoorg_marketing', 'lingwoorg_marketing.defaults');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_marketing_content_default_fields', $args);
}

/**
 * Implementation of hook_context_default_contexts().
 */
function lingwoorg_marketing_context_default_contexts() {
  module_load_include('inc', 'lingwoorg_marketing', 'lingwoorg_marketing.defaults');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_marketing_context_default_contexts', $args);
}

/**
 * Implementation of hook_node_info().
 */
function lingwoorg_marketing_node_info() {
  module_load_include('inc', 'lingwoorg_marketing', 'lingwoorg_marketing.features.node');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_marketing_node_info', $args);
}

/**
 * Implementation of hook_views_default_views().
 */
function lingwoorg_marketing_views_default_views() {
  module_load_include('inc', 'lingwoorg_marketing', 'lingwoorg_marketing.features.views');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_marketing_views_default_views', $args);
}
