<?php

/**
 * Implementation of hook_node_info().
 */
function lingwoorg_content_node_info() {
  module_load_include('inc', 'lingwoorg_content', 'lingwoorg_content.features.node');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_content_node_info', $args);
}
