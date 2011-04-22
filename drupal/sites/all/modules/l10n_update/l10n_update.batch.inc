<?php 
// $Id: l10n_update.batch.inc,v 1.6.2.1 2010/09/19 15:56:45 jareyero Exp $
/**
 * @file
 *   Reusable API for creating and running l10n update batches
 */
include_once 'l10n_update.check.inc';

/**
 * Create a batch to just download files
 * 
 * All sources must have a 'fileurl'
 */
function l10n_update_batch_download($sources) {  
  foreach ($updates as $update) {
    $operations[] = array('_l10n_update_batch_download', array($update));
  }
  return _l10n_update_create_batch($operations);
}

/**
 * Create a batch to just import files
 * 
 * All sources must have a 'filepath'
 */
function l10n_update_batch_import($sources, $import_mode) {
  foreach ($updates as $update) {
    $operations[] = array('_l10n_update_batch_import', array($update, $import_mode));
  }
  return _l10n_update_create_batch($operations);
}

/**
 * Create a big batch for multiple projects and languages
 * 
 * @param $updates
 *   Array of update sources to be run
 * @param $mode
 *   Import mode
 */
function l10n_update_batch_multiple($updates, $import_mode) {
  foreach ($updates as $update) {
    if ($update->type == 'download') {
      $operations[] = array('_l10n_update_batch_download', array($update));
      $operations[] = array('_l10n_update_batch_import', array(NULL, $import_mode));
    }
    else {
      $operations[] = array('_l10n_update_batch_import', array($update, $import_mode));
    }
    // This one takes always parameters from results
    $operations[] = array('_l10n_update_batch_history', array(NULL));
  }
  if (!empty($operations)) {
    return _l10n_update_create_batch($operations);
  }
}

/**
 * Create batch stub for this module
 * 
 * @return $batch
 */
function _l10n_update_create_batch($operations = array()) {
  $t = get_t();
  $batch = array(
    'operations'    => $operations,
    'title'         => $t('Updating translation.'),
    'init_message'  => $t('Downloading and importing files.'),
    'error_message' => $t('Error importing interface translations'),
    'file'          => drupal_get_path('module', 'l10n_update') . '/l10n_update.batch.inc',
    'finished'      => '_l10n_update_batch_finished',
  );
  return $batch;
}


/**
 * Batch download file
 * 
 * @param $update
 * @param $context
 * @return unknown_type
 */
function _l10n_update_batch_download($update, &$context) {
  $t = get_t();
  if (l10n_update_source_download($update)) {
    $context['message'] = $t('Importing downloaded translation: %url.', array('%url' => $update->fileurl));
    $context['results'][] = $update;
  }
  else {
    drupal_set_message($t('Failed download from %url', array('%url' => $update->fileurl)), 'error');
  }  
}

/**
 * Batch process: Update the download history table
 */
function _l10n_update_batch_history($update, &$context) {
  if ($update = _l10n_update_batch_param($update, $context)) {
    l10n_update_source_history($update);
    $context['results'][] = $update;
  }
}

/**
 * Batch import translation file
 * 
 * This takes a file parameter or continues from previous batch which should have downloaded a file
 * 
 * @param $file
 *   File to be imported. If empty, the file will be taken from $context['results']
 * @param $locale
 * @param $mode
 * @param $context
 */
function _l10n_update_batch_import($file, $mode, &$context) {
  $t = get_t();
  if ($file = _l10n_update_batch_param($file, $context)) {
    if (l10n_update_source_import($file, $mode)) {
      $context['results'][] = $file;
      if ($file->type == 'download') {
        drupal_set_message($t('Successfully downloaded and imported translation from %url', array('%url' => $file->fileurl)));        
      }
      else {
        drupal_set_message($t('Imported translation file %name.', array('%name' => $file->filepath)));
      }
    }
    else {
      drupal_set_message($t('Failed import of translation file %name.', array('%name' => $file->filepath)), 'error');
    }
  }
}

/**
 * Get parameter from results of previous batch if not present
 */
function _l10n_update_batch_param($param, &$context) {
  if (isset($param)) {
    return $param;
  }
  elseif (!empty($context['results'])) {
    return array_pop($context['results']);
  }
}

/**
 * Batch finished callback, set result message
 * 
 * @param $success
 * @param $results
 * @return unknown_type
 */
function _l10n_update_batch_finished($success, $results) {
  $t = get_t();
  if ($success) {
    drupal_set_message($t('Successfully imported translations.'));
  }
  else {
    drupal_set_message($t('Error importing translations.'), 'error');
  }
}
