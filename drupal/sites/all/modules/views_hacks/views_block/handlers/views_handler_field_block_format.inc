<?php
// $Id: views_handler_field_block_format.inc,v 1.1.2.1 2010/05/22 13:50:26 kratib Exp $

class views_handler_field_block_format extends views_handler_field {
  function render($values) {
    if (!isset($values->{$this->field_alias})) return '';
    $formats = filter_formats();
    return $formats[$values->{$this->field_alias}]->name;
  }
}

