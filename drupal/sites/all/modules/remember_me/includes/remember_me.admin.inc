<?php
// $Id: remember_me.admin.inc,v 1.1.2.1 2009/05/25 15:09:03 nickl Exp $

/**
 * @file
 * Menu callback include file for admin/settings pages.
 */

/**
 * Implementation of menu callback.
 */
function remember_me_admin_settings() {
  global $user;
  $options = drupal_map_assoc(
    array(3600, 10800, 21600, 43200, 86400, 172800, 259200, 604800, 1209600, 2419200),
    'format_interval'
  );
  $dependency = array();
  if (function_exists('ctools_include')) {
    ctools_include('modal');
    ctools_include('ajax');
    ctools_include('dependent');
    ctools_modal_add_js();
    drupal_add_js('misc/collapse.js');

    $link = ctools_modal_text_button(
      t('Configure Who\'s online settings'),
      'remember_me/ajax/users/online',
      t('Who\'s online settings.'),
      'remember-me-users-online'
    );
    $dependency = array(
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-remember-me-managed' => array(1)),
    );
  }
  else {
    $link = l(
      t('Configure Who\'s online settings'),
      'admin/settings/remember_me/users/online',
      array(
        'attributes' => array(
          'title' => t('Who\'s online settings.'),
          'class' => 'remember-me-users-online',
        ),
        'query' => array(
          'destination' => 'admin/settings/remember_me',
        ),
      )
    );
  }

  $vars = array(
    'remember' => array(
      '#type' => 'item',
      '#title' => t('Remember me'),
      '#value' => $user->remember_me ? t('Yes') : t('No'),
      '#description' => t("Current user chose at log in."),
    ),
    'session' => array(
      '#type' => 'item',
      '#title' => t('Session lifetime'),
      '#value' => format_interval(ini_get('session.cookie_lifetime')),
      '#description' => t("Currently configured session cookie lifetime."),
    ),
    'activity' => array(
      '#type' => 'item',
      '#title' => t('User activity'),
      '#value' => '<span id="remember-me-timeout">'.
        format_interval(variable_get('user_block_seconds_online', 900))
        .'</span>',
      '#description' => t("Period a user is considered to be online for after they have last viewed a page."),
    ),
  );
  $form['legend'] = array(
    '#type'        => 'markup',
    '#value'       => theme('remember_me_settings_display', $vars),
  );

  $form['remember_me_phantom_session'] = array(
    '#type' => 'checkbox',
    '#title' => t('Manage phantom sessions'),
    '#default_value' => variable_get('remember_me_phantom_session', 0),
    '#description' => t("Prevent phantom sessions from causing duplicate entries on the Who's online list."),
  );
  $form['user_activity'] = array(
    '#type' => 'item',
    '#value' => $link,
    '#description' => t("Decrease the likelihood of trapping phantom sessions by shortening the Who's online user activity period."),
  );
  $form['remember_me_managed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Manage session lifetime'),
    '#default_value' => variable_get('remember_me_managed', 0),
    '#description' => t('Choose to manually overwrite the configuration value from settings.php.'),
  );
  $form['remember_me_lifetime'] = array(
    '#type' => 'select',
    '#title' => t('Lifetime'),
    '#default_value' => variable_get('remember_me_lifetime', 604800),
    '#options' => $options,
    '#description' => t('Duration a user will be remembered for. This setting is ignored if Manage session lifetime (above) is disabled.'),
  ) + $dependency;
  $form['remember_me_checkbox'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remember me field'),
    '#default_value' => variable_get('remember_me_checkbox', 1),
    '#description' => t('Default state of the "Remember me" field on the login forms.'),
  );

  return system_settings_form($form);
}


function theme_remember_me_settings_display($vars) {
  $rows = array();
  foreach($vars as $var) {
    $desc = $var['#description'];
    unset($var['#description']);
    $output = '<div class="container-inline">'. theme('item', $var) .'</div>';
    $output .= '<div class="description">'. $desc .'</div>';
    $rows[] = $output;
    if(is_array($rows[0])) {
      $rows[1] = array($rows[1]);
    }
    else if(sizeof($rows) == 2) {
      $rows = array(array('data' => $rows, 'class' => 'even'));
    }
  }

  return theme('table', array(), $rows);
}