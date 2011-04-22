<?php
// $Id: views_handler_filter_block_module.inc,v 1.1.2.1 2010/05/22 13:50:26 kratib Exp $

/**
 * Filter based on block module.
 */
class views_handler_filter_block_module extends views_handler_filter_in_operator {
  function get_value_options() {
    $modules = db_query("SELECT DISTINCT module FROM {blocks}");
    while ($module = db_fetch_object($modules)) {
      $this->value_options[$module->module] = $module->module;
    }
    ksort($this->value_options);
  }
}
