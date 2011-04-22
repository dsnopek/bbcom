<?php
/**
 * @file
 * Drupal Notifications Framework - Default class file
 */

/**
 * Message destination class
 */
class Notifications_Custom_Subscription extends Notifications_Subscription {
  // Unique custom subscription id
  public $csid;
  // Array of field values, each is an array with type and value
  public $field_values;

  /**
   * Build for a subscription type
   * 
   * @param $template
   *   Template object or subscription type
   */
  public static function build($template, $default_class = 'Notifications_Custom_Subscription') {
    return parent::build($template, $default_class);
  }
  // Get name
  function get_name() {
    return check_plain(notifications_translate("custom:$this->type:name", $this->get_type('name')));
  }  
  // Get title
  function get_title() {
    return check_plain(notifications_translate("custom:$this->type:title", $this->get_type('title')));
  }  
  // Get description
  function get_description() {
    return check_plain(notifications_translate("custom:$this->type:title", $this->get_type('description')));
  }

  /**
   * Get default value for a possible checkbox
   */
  function get_default($user_registration = FALSE) {
    return $this->is_instance() || $user_registration && $this->get_type('default_value');
  }
  /**
   * Get fields as arrays
   */
  function get_fields_array() {
    $list = array();
    foreach ($this->get_fields() as $field) {
      $list[] = array('type' => $field->field, 'value' => $field->value);
    }
    return $list;
  }
  /**
   * Check visibility of this subscription type
   */
  function is_visible($user_registration = FALSE) {
    return user_access('administer users') || $user_registration ? $this->get_type('register') : $this->get_type('visibility');
  }
  /**
   * Format as a form element, which may be a visible checkbox or a hidden field
   */
  function form_element($user_registration = FALSE) {
    return $this->is_visible($user_registration) ? $this->form_checkbox($user_registration) : $this->form_value();
  }
  
  /**
   * Format as a form checkbox
   */
  function form_checkbox($user_registration = FALSE) {
    return array(
      '#type' => 'checkbox',
      '#title' => $this->get_name(),
      '#default_value' => $this->get_default($user_registration),
      '#description' => $this->get_description(),
    );    
  }
  /**
   * Format as a form value
   */
  function form_value() {
    return array(
      '#type' => 'value',
      '#value' => $this->type,
    );
  }
  
}