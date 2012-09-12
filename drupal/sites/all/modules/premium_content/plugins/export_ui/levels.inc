<?php
/**
 * @file
 * CTools export UI for Premium levels.
 */

/**
* Define this Export UI plugin.
*/
$plugin = array(
  'schema' => 'premium_levels',
  'access' => 'administer premium content levels',
  'menu' => array(
    'menu item' => 'premium_levels',
    'menu title' => 'Premium content levels',
    'menu description' => 'Administer Premium content levels.',
  ),

  'title singular' => t('level'),
  'title plural' => t('levels'),
  'title singular proper' => t('Premium content level'),
  'title plural proper' => t('Premium content levels'),

  'export' => array(
    'admin_title' => 'name',
  ),

  'form' => array(
    'settings' => 'premium_levels_ctools_export_ui_form',
  ),

  'handler' => array(
    'class' => 'levels_export_ui',
    'parent' => 'ctools_export_ui',
  ),
);

/**
 * Form for editing Premium content levels. 
 */
function premium_levels_ctools_export_ui_form(&$form, &$form_state) {
  $level = $form_state['item'];

  // We require a title, since it's used in select boxes in forms, etc.
  $form['info']['name']['#required'] = TRUE;

  $form['protection_type'] = array(
    '#type' => 'radios',
    '#title' => t('Protection type'),
    '#default_value' => $level->protection_type,
    '#options' => array(
      'all' => t('Premium items are permanently restricted'),
      'archive' => t('Protect archives only: Items switch to premium status after a specified period'),
      'latest' => t('Protect latest content only: Items switch to non-premium status after a specified period'),
    ),
  );

  $form['duration'] = array(
    '#type' => 'select',
    '#title' => t('Protection duration'),
    '#default_value' => $level->duration,
    '#options' => array(1 => 1, 2 => 2, 3 => 3, 4 => 4, 5 => 5, 6 => 6, 7 => 7, 8 => 8, 9 => 9,
      10 => 10, 11 => 11, 12 => 12, 13 => 13, 14 => 14, 15 => 15),
  );

  $form['duration_unit'] = array(
    '#type' => 'select',
    '#title' => t('Duration unit'),
    '#default_value' => $level->duration_unit,
    '#options' => array(
      'D' => t('Days'),
      'W' => t('Weeks'),
      'M' => t('Months'),
      'Y' => t('Years'),
    ),
  );

  $form['denied_message_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access denied message'),
    '#tree' => FALSE,
  );

  $form['denied_message_fieldset']['denied_message'] = array(
    '#type' => 'textarea',
    '#default_value' => $level->denied_message,
    '#cols' => 60,
    '#rows' => 6,
    '#description' => t('When a visitor doesn\'t have access to an item at this level they will see this message instead of its full text'),
  );

  $form['denied_message_fieldset']['denied_message_format'] = filter_form($level->denied_message_format, 1, array('denied_message_format'));

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save premium level'),
  );
}

