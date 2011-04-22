<?php
// $Id: views_handler_field_block_info.inc,v 1.1.2.1 2010/05/22 13:50:26 kratib Exp $

class views_handler_field_block_info extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['module'] = array('table' => 'blocks', 'field' => 'module');
    $this->additional_fields['delta'] = array('table' => 'blocks', 'field' => 'delta');
  }

  function render($values) {
    if ($values->{$this->aliases['module']} != 'block') {
      $block = module_invoke($values->{$this->aliases['module']}, 'block', 'list');
      return isset($block[$values->{$this->aliases['delta']}]['info']) ? $block[$values->{$this->aliases['delta']}]['info'] : '';
    }
    return parent::render($values);
  }
}

