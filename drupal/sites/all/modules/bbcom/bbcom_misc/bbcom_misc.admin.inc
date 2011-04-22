<?php
// $Id$

/**
 * @file
 * Admin pages for misc
 */

function bbcom_misc_admin_settings() {
  $form['bbcom_misc_role_titles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role titles'),
	'#tree' => TRUE,
  );

  $values = variable_get('bbcom_misc_role_titles', array());
  foreach(user_roles() as $rid => $role) {
    $form['bbcom_misc_role_titles'][$rid] = array(
      '#type' => 'textfield',
      '#title' => $role,
      '#size' => 40,
      '#maxlength' => 255,
      '#default_value' => isset($values[$rid]) ? $values[$rid] : '',
    );
  }

  return system_settings_form($form);
}

