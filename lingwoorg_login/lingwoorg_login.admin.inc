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
  $value = $base32->fromDec(rand(0, 1048575));
  return str_pad($value, 4, '0', STR_PAD_LEFT);
}

function lingwoorg_login_batches_add_submit($form, &$form_state) {
  require_once(drupal_get_path('module', 'lingwoorg_login') .'/base32.php');

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
  //$form_state['redirect'] = url('admin/user/reg_codes/'. $batch_id);
}

