<?php

function bbcom_mturk_dictionary_source_senses_form(&$form_state, $entry_hash, $sense_id) {
  $entry = LingwoEntry::fromHash($entry_hash);
  $sense = $entry->senses->getSense($sense_id);
  
  $form = array();
  $form['instructions'] = array(
    '#weight' => -1,
    '#value' => '<p>' . t('Below is a dictionary entry, with one or more senses.  Your job is to help correct one of those senses.') . '</p>',
  );
  // TODO: refactor the theme stuff in lingo_senses
  // TODO: show the sense we are working on highlighted
  // TODO: show the whole thing, but smaller and easier to deal with
  $form['entry'] = array(
    '#title' => t('Full definition'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'value' => array('#value' => theme('lingwo_senses_original', $entry->getNode())),
  );

  $form['definition'] = array(
    '#title' => t('This definition ...'),
    '#type' => 'radios',
    '#options' => array(
      'ok' => t('<strong>Is perfectly clear!</strong> I understand what it means and how it is different from the other senses.'),
      'duplicate' => t('<strong>Is a duplicate of another sense</strong> on this entry.'),
      'errors' => t('<strong>Is unclear or confusing or contains unfriendly characters</strong> - but I can fix it!'),
      'nonsense' => t('<strong>Makes no sense to me</strong>'),
    ),
  );

  // TODO: only show if the 'example' is set to 'duplicate'
  $form['duplicate_sense'] = array(
    '#title' => t('It\'s a duplicate of this sense'),
    '#type' => 'select',
    '#options' => array(),
  );
  
  // TODO: only show if the 'example' is set to 'errors'
  $form['definition_correction'] = array(
    '#title' => t('Corrected definition'),
    '#type' => 'textfield',
  );

  // TODO: only show if the example exists
  $form['example_has_errors'] = array(
    '#title' => t('This example is unclear or confusing or contains unfriendly characters'),
    '#type' => 'checkbox',
  );

  // TODO: only show if the example has errors
  $form['example_correction'] = array(
    '#title' => t('Corrected example'),
    '#type'  => 'textfield',
  );

  return bbcom_mturk_form($form);
}

