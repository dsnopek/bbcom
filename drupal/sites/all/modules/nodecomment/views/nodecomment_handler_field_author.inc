<?php
// $Id: nodecomment_handler_field_author.inc,v 1.1.2.1 2009/06/15 18:40:37 merlinofchaos Exp $
/**
 * Field handler to allow linking to a user account or homepage
 */
class nodecomment_handler_field_author extends views_handler_field_user_name {
  /**
   * Override init function to add uid and homepage fields.
   */
  function init(&$view, &$data) {
    parent::init($view, $data);
    $this->additional_fields['uid'] = array('table' => 'users', 'field' => 'uid');
    $this->additional_fields['user_name'] = array('table' => 'users', 'field' => 'name');
    $this->additional_fields['name'] = array('table' => 'node_comments', 'field' => 'name');
    $this->additional_fields['homepage'] = array('table' => 'node_comments', 'field' => 'homepage');
  }

  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }

  function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['user_name'];
  }

  function render_link($data, $values) {
    // The parent will handle anything that isn't a node comment just fine.
    if (!empty($values->{$this->aliases['uid']}) || empty($values->{$this->aliases['name']})) {
      return parent::render_link($data, $values);
    }

    if (!empty($this->options['link_to_user'])) {
      $account->uid = 0;
      $account->name = $values->{$this->aliases['name']};
      $account->homepage = $values->{$this->aliases['homepage']};

      return theme('username', $account);
    }
    else {
      return $data;
    }
  }
}
