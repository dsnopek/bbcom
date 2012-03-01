<?php
// $Id$

/**
 * @file
 * Admin pages for dictionary
 */

function bbcom_dictionary_admin_settings() {
  $form['bbcom_dictionary_editorial_guidelines'] = array(
    '#type' => 'textfield',
    '#title' => 'Editorial Guidelines',
    '#description' => 'The <em>nid</em> of the editorial guidelines',
    '#default_value' => variable_get('bbcom_dictionary_editorial_guidelines', ''),
  );

  return system_settings_form($form);
}

