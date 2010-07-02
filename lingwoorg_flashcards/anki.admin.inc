<?php
// $Id$

/**
 * @file
 * Admin pages for Anki integration.
 */

function anki_admin_settings() {
  $form['anki_server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Server url'),
    '#default_value' => anki_server_url(),
    '#field_prefix' => 'http://',
    '#description' => 'The URL (including <em>hostname</em> and <em>port</em>) that the AnkiServer is listening on.  For example: <em>localhost:5000</em>',
  );

  foreach (node_get_types() as $type) {
    $type_options[$type->type] = $type->name;
  }

  $form['anki_deck_content_type'] = array(
    '#type' => 'select',
    '#title' => t('Deck Content Type'),
    '#default_value' => anki_deck_content_type(),
    '#options' => $type_options,
    '#description' => t('The content type to use as an Anki deck.'),
  );

  // get information from CCK about our type
  $field_options = array();
  foreach (content_fields() as $field_name => $field) {
    if ($field['type'] == 'filefield') {
      $field_options[$field_name] = $field['widget']['label'];
    }
  }

  $form['anki_deck_content_field'] = array(
    '#type' => 'select',
    '#title' => t('Deck Content Field'),
    '#default_value' => anki_deck_content_field(),
    '#options' => $field_options,
    '#description' => t('The CCK filefield which holds the deck file.'),
  );

  return system_settings_form($form);
}

