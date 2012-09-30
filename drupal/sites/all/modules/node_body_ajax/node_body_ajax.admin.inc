<?php

function node_body_ajax_admin_settings(&$form_state) {
  $form = array();

  $form['node_body_ajax_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types'),
    '#options' => array_map('filter_xss', node_get_types('names')),
    '#default_value' => variable_get('node_body_ajax_node_types', array()),
    '#description' => t('Select all the content types whose body you would like served via AJAX. This makes it so that users can\'t copy the body or see it via view source.'),
  );

  $form['node_body_ajax_disable_selection'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable selection'),
    '#default_value' => variable_get('node_body_ajax_disable_selection', FALSE),
    '#description' => t('Attempts to disable selection in JavaScript on a page where a node body is loaded via AJAX.'),
  );

  return system_settings_form($form);
}

