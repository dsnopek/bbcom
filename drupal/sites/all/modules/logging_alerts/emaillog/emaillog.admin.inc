<?php
// $Id: emaillog.admin.inc,v 1.1.2.1 2009/07/23 16:28:59 davereid Exp $

/**
 * @file
 * Administrative page callbacks for the emaillog module.
 */

/**
 * Administration settings form.
 *
 * @see system_settings_form()
 */
function emaillog_admin_settings() {
  $form['emaillog'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email addresses for each severity level.'),
    '#description' => t('Enter an email address for each severity level. For example, you may want to get emergency and critical levels to your pager or mobile phone, while notice level messages can go to a regular email. If you leave the email address blank for a severity level, no email messages will be sent for that severity level.'),
  );
  foreach (watchdog_severity_levels() as $severity => $description) {
    $description = drupal_ucfirst($description);
    $key = 'emaillog_' . $severity;
    $form['emaillog'][$key] = array(
      '#type' => 'textfield',
      '#title' => t('Email address for severity @description', array('@description' => $description)),
      '#default_value' => variable_get($key, ''),
      '#description' => t('The email address to send log entries of severity @description to.', array('@description' => $description)),
    );
  }

  return system_settings_form($form);
}
