<?php
// $Id$

/**
 * @file
 * Admin pages for metrics
 */

function bbcom_metrics_admin_settings() {
  $form['mixpanel'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mixpanel'),
  );

  $form['mixpanel']['bbcom_metrics_mixpanel_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Mixpanel token'),
    '#default_value' => variable_get('bbcom_metrics_mixpanel_token', ''),
  );

  $form['mixpanel']['bbcom_metrics_mixpanel_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Mixpanel key'),
    '#default_value' => variable_get('bbcom_metrics_mixpanel_key', ''),
  );

  $form['mixpanel']['bbcom_metrics_mixpanel_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Mixpanel secret'),
    '#default_value' => variable_get('bbcom_metrics_mixpanel_secret', ''),
  );

  $form['bbcom_metrics_defer_backend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Defer backend analytics to cron'),
    '#default_value' => variable_get('bbcom_metrics_defer_backend', TRUE),
    '#description' => t('Keep enabled in production or you could experience some performance problems.'),
  );

  $form['bbcom_metrics_track_locally'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track locally'),
    '#default_value' => variable_get('bbcom_metrics_defer_track_locally', TRUE),
    '#description' => t('Track in our database as well as mix panel'),
  );

  return system_settings_form($form);
}

