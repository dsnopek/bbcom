<?php
// $Id$

/**
 * @file
 * Admin pages for Anki integration.
 */

function lingwoorg_flashcards_admin_settings() {
  $form['lingwo_flashcards_anki_server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Server url'),
    '#default_value' => lingwo_flashcards_anki_server_url(),
    '#field_prefix' => 'http://',
    '#description' => 'The URL (including <em>hostname</em> and <em>port</em>) that the AnkiServer is listening on.  For example: <em>localhost:5000</em>',
  );

  return system_settings_form($form);
}

