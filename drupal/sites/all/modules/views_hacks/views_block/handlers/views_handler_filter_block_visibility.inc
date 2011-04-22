<?php
// $Id: views_handler_filter_block_visibility.inc,v 1.1.2.1 2010/05/22 13:50:26 kratib Exp $

/**
 * Filter based on block page visibility.
 */
class views_handler_filter_block_visibility extends views_handler_filter_in_operator {
  function get_value_options() {
    $this->value_options = array(
      t('All except listed pages'), 
      t('Only listed pages'),
      t('PHP code'),
    );
  }
}

