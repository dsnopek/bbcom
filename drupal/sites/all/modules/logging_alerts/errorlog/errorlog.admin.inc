<?php
// $Id: errorlog.admin.inc,v 1.1.2.1 2009/07/23 16:29:00 davereid Exp $

/**
 * @file
 * Administrative page callbacks for the errorlog module.
 */

/**
 * Administration settings form.
 *
 * @see system_settings_form()
 */
function errorlog_admin_settings() {
  $form['errorlog'] = array(
    '#type' => 'fieldset',
    '#title' => t('Error logging for each severity level.'),
    '#description' => t('Check each severity level you want to get logged to the error log.'),
  );

  foreach (watchdog_severity_levels() as $severity => $description) {
    $description = drupal_ucfirst($description);
    $key = 'errorlog_' . $severity;
    $form['errorlog'][$key] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Severity: @description', array('@description' => $description)),
      '#default_value' => variable_get($key, FALSE),
    );
  }

  return system_settings_form($form);
}
