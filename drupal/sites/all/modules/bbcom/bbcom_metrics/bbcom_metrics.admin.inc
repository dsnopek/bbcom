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

function bbcom_metrics_admin_funnels($funnel_id = NULL) {
  if (empty($funnel_id)) {
    $items = array();
    foreach (_bbcom_metrics_mp_export('funnels/list') as $funnel) {
      $items[] = l($funnel->name, 'admin/settings/bbcom/metrics/funnels/' . $funnel->funnel_id);
    }
    return theme('item_list', $items, t('Funnels'));
  }
  else {
    // find the 4 most recent cohort groups
    $current = time();
    $cohorts = array();
    for ($i = 0; $i < 4; $i++) {
      $time = $current - (86400 * 7 * $i);
      $cohorts[] = date('Y', $time) . 'W' . date('W', $time);
    }
    $cohorts = array_reverse($cohorts);

    // find a date range that covers them all
    $from_date = date('Y-m-d', strtotime($cohorts[0]));

    // request the funnel each group using that range
    $funnels = _bbcom_metrics_mp_export('funnels', array(
      'funnel_id' => $funnel_id,
      'interval'  => '30',
      'from_date' => $from_date,
      'on'        => 'properties["cohort_week"]',
    ));

    $output = '<div class="clear-block">';
    foreach ($cohorts as $cohort) {
      $steps = $funnels['data'][$from_date][$cohort];
      if (!empty($steps)) {
        $output .= theme('bbcom_metrics_funnel', $steps, t('%cohort group', array('%cohort' => $cohort)));
      }
    }
    $output .= '</div>';

    drupal_add_css(drupal_get_path('module', 'bbcom_metrics') . '/bbcom_metrics.css');

    return $output;
  }
}

