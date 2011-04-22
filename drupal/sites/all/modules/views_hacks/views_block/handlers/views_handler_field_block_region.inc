<?php
// $Id: views_handler_field_block_region.inc,v 1.1.2.1 2010/05/22 13:50:26 kratib Exp $

class views_handler_field_block_region extends views_handler_field {
  function render($values) {
    return empty($values->{$this->field_alias}) ? '('. t('none') .')' : $values->{$this->field_alias};
  }
}

