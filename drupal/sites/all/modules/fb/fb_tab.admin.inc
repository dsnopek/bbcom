<?php
// $Id: fb_tab.admin.inc,v 1.6 2010/11/02 18:42:39 yogadex Exp $

/**
 * @file
 * Drupal administration of fb_tab.module.
 */

/**
 * Implementation of hook_fb_admin().
 */
function fb_tab_fb_admin($op, $data, &$return) {
  $fb = isset($data['fb']) ? $data['fb'] : NULL;
  $fb_app = isset($data['fb_app']) ? $data['fb_app'] : NULL;
  $config = _fb_tab_get_config($fb_app);
  if ($op == FB_ADMIN_OP_SET_PROPERTIES) {
    $return['tab_default_name'] = $config['tab_default_name'];
    $return['profile_tab_url'] = $config['profile_tab_url'];
    //$return['edit_url'] = $config['edit_url']; // Apparently not used by facebook.
  }
  elseif ($op == FB_ADMIN_OP_LIST_PROPERTIES) {
    $return[t('Profile Tab Name')] = 'tab_default_name';
    $return[t('Profile Tab URL')] = 'profile_tab_url';
    $return[t('Profile Tab Edit URL')] = 'edit_url';
    $return[t('Installable')] = 'installable';
  }
}

/**
 * See fb_tab_form_alter.
 */
function fb_tab_admin_form_alter(&$form, &$form_state, $form_id) {
  // Add our settings to the fb_app edit form.
  if (isset($form['fb_app_data']) && is_array($form['fb_app_data'])) {
    $fb_app = $form['#fb_app'];
    $config = _fb_tab_get_config($fb_app);

    $form['fb_app_data']['fb_tab'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => isset($fb_app->label),
      '#title' => t('Facebook profile tabs'),
      '#description' => t('Settings which apply to <a href=!url target=_blank>profile tabs</a>.',
                          array('!url' => 'http://developers.facebook.com/docs/guides/canvas/#tabs')),
    );

    // Override themes
    $themes = system_theme_data();
    ksort($themes);
    $theme_options[0] = t('System default');
    foreach ($themes as $theme) {
      if ($theme->status) { // Only enabled themes.
        $theme_options[$theme->name] = $theme->name;
      }
    }
    $form['fb_app_data']['fb_tab']['custom_theme'] = array(
      '#type' => 'select',
      '#title' => t('Theme for profile tabs'),
      '#description' => t('Choose a theme designed to return FBML specifically for the 520 pixel wide space allocated to tabs.  <br/>Note that if your tab path is a menu callback which returns FBML, this setting is ignored.'),
      '#options' => $theme_options,
      '#required' => TRUE,
      '#default_value' => $config['custom_theme'],
    );

    // Properties: http://developers.facebook.com/docs/appproperties
    $form['fb_app_data']['fb_tab']['tab_default_name'] = array(
      '#type' => 'textfield',
      '#title' => 'Tab name',
      '#default_value' => $config['tab_default_name'],
      '#description' => t('A very short title.'),
    );
    $form['fb_app_data']['fb_tab']['profile_tab_url'] = array(
      '#type' => 'textfield',
      '#title' => 'Path',
      '#default_value' => $config['profile_tab_url'],
      '#description' => t('Recommended value is %tab_path.  You may choose another if you have defined a custom menu item or view.', array('%tab_path' => FB_TAB_PATH_VIEW)),
    );
    /* XXX is this used by facebook?
    $form['fb_app_data']['fb_tab']['edit_url'] = array(
      '#type' => 'textfield',
      '#title' => 'Edit URL',
      '#default_value' => $config['edit_url'],
      '#description' => t('Recommended value is %edit_url.', array(
                            '%edit_url' => url(FB_TAB_PATH_FORM, array('absolute'=> TRUE)))),
    );
    $form['fb_app_data']['fb_tab']['edit_url']['#description'] .= '<br/>' . t('Note that this field is apparently unused by facebook.');
    */
  }
}
