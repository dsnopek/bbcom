<?php
// $Id: views_handler_field_block_visibility.inc,v 1.1.2.1 2010/05/22 13:50:26 kratib Exp $

class views_handler_field_block_visibility extends views_handler_field {
  function render($values) {
    $strings = array(
      t('All except listed pages'),
      t('Only listed pages'),
      t('PHP code'),
    );
    return $strings[$values->{$this->field_alias}];
  }
}

