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
  $form['bbcom_metrics_defer_backend'] = array(
    '#type' => 'checkbox',
    '#title' => 'Defer backend analytics to cron',
    '#default_value' => variable_get('bbcom_metrics_defer_backend', TRUE),
    '#description' => t('Keep enabled in production or you could experience some performance problems.'),
  );

  return system_settings_form($form);
}

