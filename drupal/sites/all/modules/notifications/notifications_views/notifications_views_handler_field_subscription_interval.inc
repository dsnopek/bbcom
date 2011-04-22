<?php

/**
 * Field handler to translate a node type into its readable form.
 */
class notifications_views_handler_field_subscription_interval extends views_handler_field {
  function render($values) {
    $types = notifications_send_intervals();
    $value = $types[$values->{$this->field_alias}];
    return check_plain($value);
  }
}

