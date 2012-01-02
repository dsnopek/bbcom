<?php
// $Id$

/**
 * @file
 * Admin pages for metrics
 */

function bbcom_metrics_admin_settings() {
  $form['bbcom_metrics_mixpanel_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Mixpanel token'),
    '#default_value' => variable_get('bbcom_metrics_mixpanel_token', ''),
  );

  return system_settings_form($form);
}

