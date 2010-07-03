<?php

/**
 * Implementation of hook_rules_defaults().
 */
function lingwoorg_wial_rules_defaults() {
  module_load_include('inc', 'lingwoorg_wial', 'lingwoorg_wial.defaults');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_wial_rules_defaults', $args);
}

/**
 * Implementation of hook_user_default_permissions().
 */
function lingwoorg_wial_user_default_permissions() {
  module_load_include('inc', 'lingwoorg_wial', 'lingwoorg_wial.defaults');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_wial_user_default_permissions', $args);
}

/**
 * Implementation of hook_views_default_views().
 */
function lingwoorg_wial_views_default_views() {
  module_load_include('inc', 'lingwoorg_wial', 'lingwoorg_wial.features.views');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_wial_views_default_views', $args);
}
