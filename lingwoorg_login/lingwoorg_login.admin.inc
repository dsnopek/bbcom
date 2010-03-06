<?php

/**
 * @file
 * Admin pages
 */

function lingwoorg_login_batches_list() {
  return 'list';
}

function lingwoorg_login_batches_add(&$form_state) {
  $form['num'] = array(
    '#title' => t('Number of codes'),
    '#type' => 'textfield',
    '#default_value' => 10,
    '#description' => t('The number of codes to generate in this batch.'),
    '#required' => TRUE,
  );
  $form['expires'] = array(
    '#title' => t('Expires'),
    '#type' => 'date_text',
    '#description' => t('The date when the codes in this batch expire.'),
    '#required' => TRUE,
  );
  $form['note'] = array(
    '#title' => t('Note'),
    '#type' => 'textfield',
    '#size' => 60,
    '#maxlength' => 255,
    '#description' => t('A short note to go along with this batch.  For example, "<em>To hand out in the month of 2010-03</em>" or "<em>For my class (group-26)</em>".'),
  );

  $form['create'] = array(
    '#type' => 'submit',
    '#value' => t('Create batch'),
  );

  return $form;
}

