<?php
// $Id: views_handler_filter_block_format.inc,v 1.1.2.1 2010/05/22 13:50:26 kratib Exp $

/**
 * Filter based on block body format.
 */
class views_handler_filter_block_format extends views_handler_filter_in_operator {
  function get_value_options() {
    foreach (filter_formats() as $fid => $format) {
      $this->value_options[$fid] = $format->name;
    }
  }
}

