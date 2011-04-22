<?php
// $Id: views_handler_field_block_custom.inc,v 1.1.2.1 2010/05/22 13:50:26 kratib Exp $

class views_handler_field_block_custom extends views_handler_field {
  function render($values) {
    $strings = array(
      t('Show always'),
      t('Show by default'),
      t('Hide by default')
    );
    return $strings[$values->{$this->field_alias}];
  }
}

