<?php

/**
 * Field handler that provides a link to delete the message.
 */
class notifications_views_handler_field_delete_link extends views_handler_field {
  function query() {
    return;
  }
  
  function render($values) {
    return l('delete', 'message/' . $values->mqid . '/delete', array('query' => array('destination' => $this->view->display_handler->options['path'])));
  }
}

