<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function lingwoorg_content_context_default_contexts() {
  module_load_include('inc', 'lingwoorg_content', 'lingwoorg_content.defaults');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_content_context_default_contexts', $args);
}

/**
 * Implementation of hook_menu_default_items().
 */
function lingwoorg_content_menu_default_items() {
  module_load_include('inc', 'lingwoorg_content', 'lingwoorg_content.defaults');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_content_menu_default_items', $args);
}

/**
 * Implementation of hook_node_info().
 */
function lingwoorg_content_node_info() {
  module_load_include('inc', 'lingwoorg_content', 'lingwoorg_content.features.node');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_content_node_info', $args);
}

/**
 * Implementation of hook_user_default_permissions().
 */
function lingwoorg_content_user_default_permissions() {
  module_load_include('inc', 'lingwoorg_content', 'lingwoorg_content.defaults');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_content_user_default_permissions', $args);
}

/**
 * Implementation of hook_views_default_views().
 */
function lingwoorg_content_views_default_views() {
  module_load_include('inc', 'lingwoorg_content', 'lingwoorg_content.features.views');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_content_views_default_views', $args);
}
