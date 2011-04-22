<?php
// $Id: remember_me.users.inc,v 1.1.2.1 2009/05/25 15:09:03 nickl Exp $

/**
 * @file
 * Menu callback include file for url branch remember_me/ajax/users.
 */

/**
 * Implementation of menu callback.
 * Prepare $form_state and wrap form in modal.
 */
function remember_me_ajax_users_online() {
  if (function_exists('ctools_include')) {
    ctools_include('ajax');
    ctools_include('modal');

    $form_state = array(
      'title' => t("'Who's online' settings"),
      'ajax' => TRUE,
    );
    $output = ctools_modal_form_wrapper(
      'remember_me_ajax_users_online_form',
      $form_state
    );
    if (empty($output)) {

      $output   = array();
      $output[] = ctools_ajax_command_remove( 'div.messages');
      $output[] = ctools_ajax_command_before(
        'form#remember-me-admin-settings',
        theme('status_messages')
      );
      $output[] = ctools_ajax_command_html(
        'span#remember-me-timeout',
        format_interval(variable_get('user_block_seconds_online', 900))
      );

      $output[] = ctools_modal_command_dismiss();
    }

    ctools_ajax_render($output);
  }
  else {
    return t('!chaos is required for this ajax implementation, install it today.',
      array(
        '!chaos' => l(t('Chaos tool suite'), 'http://drupal.org/project/ctools'),
      )
    );
  }
}

/**
 * Who's online form callback.
 * Extract form from user_block method and add buttons.
 */
function remember_me_ajax_users_online_form(&$form_state) {
  $form['conf'] = array();
  $form['conf'] += user_block('configure', 3);
  $form['disp'] = array(
    '#type'        => 'fieldset',
    '#title'       => t("'Who's online' block preview"),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );
  $view = user_block('view', 3);
  $form['disp']['items'] = array(
    '#type'  => 'markup',
    '#value' => '<div>'. $view['content'] .'</div>'
  );
  return system_settings_form($form);
}