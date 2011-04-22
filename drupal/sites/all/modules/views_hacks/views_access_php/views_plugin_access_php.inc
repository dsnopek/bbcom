<?php
// $Id: views_plugin_access_php.inc,v 1.1.2.1 2010/05/22 13:50:26 kratib Exp $

/**
 * Access plugin that provides PHP-based access control.
 */
class views_plugin_access_php extends views_plugin_access {
  function access($account) {
    return views_access_php_eval($this->options['code'], $this->view);
  }

  function get_access_callback() {
    return array('views_access_php_eval', array($this->options['code'], $this->view));
  }

  function summary_title() {
    return t('PHP code');
  }

  function option_defaults(&$options) {
    $options['code'] = "<?php\r\nreturn TRUE;\r\n?>";
  }

  function options_form(&$form, &$form_state) {
    $form['code'] = array(
      '#type' => 'textarea',
      '#title' => t('PHP code'),
      '#default_value' => $this->options['code'],
      '#description' => t('Users will be able to access this display if PHP code returns TRUE. Enclose your code within <code>&lt;?php</code> and <code>?&gt;</code> tags.'),
      '#required' => TRUE,
    );
  }
}

