<?php

/**
 * CTools export UI extending class. Slightly customized for Context.
 */
class levels_export_ui extends ctools_export_ui {
  function list_form(&$form, &$form_state) {
    parent::list_form($form, $form_state);
    // This is perhaps a bit brutal, but I don't think that any sites 
    // will have enough premium levels to warrant the very fancy 
    // filtering interface CTools provide by default. If you really need 
    // this interface, please file a bug, and we will consider making an 
    // option for enabling it.
    // TODO: This is really an ugly way to hide these, but pending 
    // resolution of http://drupal.org/node/1133740, it seems to be the 
    // best way to go.
    $form['top row']['#access'] = FALSE;
    $form['bottom row']['#access'] = FALSE;
  }
}

