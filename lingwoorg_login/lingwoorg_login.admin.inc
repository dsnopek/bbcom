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

function lingwoorg_login_batches_add_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['num'])) {
    form_set_error('num', t('The number of codes must really be a valid number.'));
  }
  if ($form_state['values']['num'] > 50) {
    form_set_error('num', t('The number of codes can\'t be greater than 50.'));
  }
}

// Returns a random 4-character base32 number
function _lingwoorg_login_random_base32() {
  $base32 = new Base32();
  return $base32->fromDec(rand(0, 1048575), 4);
}

function lingwoorg_login_batches_add_submit($form, &$form_state) {
  // create the batch
  $batch = (object)array(
    'seed'    => _lingwoorg_login_random_base32(),
    'note'    => $form_state['values']['note'],
    'created' => time(),
    'expires' => date_format(date_create($form_state['values']['expires']), 'U'),
  );
  drupal_write_record('lingwoorg_login_reg_batch', $batch);

  // create each code
  for($i = 0; $i < $form_state['values']['num']; $i++) {
    // search for an unused code
    while(TRUE) {
      $code = _lingwoorg_login_random_base32();
      $res = db_query("SELECT 1 FROM {lingwoorg_login_reg_code} WHERE batch_id = %d AND code = '%s'", $batch->batch_id, $code);
      if (!db_fetch_array($res)) break;
    }

    $code = (object)array(
      'batch_id' => $batch->batch_id,
      'code'     => $code,
    );
    drupal_write_record('lingwoorg_login_reg_code', $code);
  }

  drupal_set_message(t('Batch created.'));
  $form_state['redirect'] = 'admin/user/reg_codes/'. $batch->batch_id;
}


function lingwoorg_login_batch_view($batch_id) {
  $res = db_query("SELECT * FROM {lingwoorg_login_reg_batch} WHERE batch_id = %d", $batch_id);
  $batch = db_fetch_object($res);

  if (!$batch) {
    return drupal_not_found();
  }

  $form['created'] = array(
    '#type' => 'item',
    '#title' => t('Created'),
    '#value' => format_date($batch->created),
  );
  $form['expires'] = array(
    '#type' => 'item',
    '#title' => t('Expires'),
    '#value' => format_date($batch->expires),
  );
  if ($batch->note) {
    $form['note'] = array(
      '#type' => 'item',
      '#title' => t('Note'),
      '#value' => $batch->note,
    );
  }

  $output = drupal_render($form);
  $output .= _lingwoorg_login_batch_get_codes($batch);
  return $output;
}

function _lingwoorg_login_batch_get_codes($batch) {
  drupal_set_title(t('Batch #!batch_id', array('!batch_id' => $batch->batch_id)));

  $res = db_query("SELECT * FROM {lingwoorg_login_reg_code} WHERE batch_id = %d", $batch->batch_id);

  $rows = array();
  while ($code = db_fetch_object($res)) {
    $rows[] = array(
      lingwoorg_login_make_code($batch->batch_id, $batch->seed, $code->code),
      $code->uid ? l(t($code->uid), '/user/'. $code->uid) : t('not used')
    );
  }

  if ($rows) {
    $header = array(
      t('Code'),
      t('User'),
    );
    return theme('table', $header, $rows);
  }

  return '<p>'.t('No registration codes in this batch!').'</p>';
}

