<?php 
// $Id: l10n_update.drush.inc,v 1.1.2.1 2010/09/19 16:25:57 jareyero Exp $

/**
 * @file
 * Update translations via drush
 */

/**
 * Implementation of hook_drush_help().
 */
function l10n_update_drush_help($section) {
  switch ($section) {
    case 'drush:l10n-update':
      return dt('Check for new translations and import updates.');
  }
}

/**
 * Implementation of hook_drush_command().
 */
function l10n_update_drush_command() {
  $commands['l10n-update'] = array(
    'callback' => 'l10n_update_drush_update',
    'description' => 'Updates translations.',
    'arguments' => array(
      'check' => 'Number of translation files to check.',
      'limit' => 'Maximum number of updates to do'
    ),
  );
  return $commands;
}

/**
 * Drush command callback: update translations
 * 
 * @param $count
 *   Number of package translations to check
 * @param $limit
 *   Maximum number of updates to do. We check $count translations but we stop after we do $limit updates.
 * @param $before
 *   Number of days, check only updates that haven't been checked for this time
 */
function l10n_update_drush_update($count = 10, $limit = 10, $before = NULL) {
  $before = isset($before) ? $before : variable_get('l10n_update_check_frequency', 0);
  module_load_include('check.inc', 'l10n_update');
  list($checked, $updated) = l10n_update_check_translations($count, time() - $before * 24 * 3600, $limit);
  return dt('Checked @checked translations, updated @updated.', array('@checked' => count($checked), '@updated' => count($updated)));
}