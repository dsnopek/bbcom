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
 * Implementation of hook_node_info().
 */
function lingwoorg_marketing_node_info() {
  module_load_include('inc', 'lingwoorg_marketing', 'lingwoorg_marketing.features.node');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_marketing_node_info', $args);
}
