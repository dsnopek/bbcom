<?php
/**
* Return an array of the modules to be enabled when this profile is installed.
*
* @return
*  An array of modules to be enabled.
*/
function bibliobird_profile_modules() {
  return array(
    // Enable required core modules first.
    'block',
    'filter',
    'node',
    'system',
    'user',
    'dblog',
    // Enable optional core modules next.
    'comment',
    'help',
    'menu',
    'taxonomy',
    'path',
    'locale',
    'translation',
    'forum',
    'php',
    'search',
    'update',
    // Contrib:
    'admin',
    'adminrole',
    //'cacherouter',
    'content',
    'content_multigroup',
    'fieldgroup',
    'filefield',
    'imagefield',
    'optionwidgets',
    'text',
    'ctools',
    'context',
    'context_ui',
    'date_api',
    'features',
    'filefield_paths',
    'flag',
    //'smtp',
    'i18nstrings',
    'i18n',
    'i18nblocks',
    'i18ncontent',
    'l10n_update',
    'i18nmenu',
    'i18nviews',
    'translation_helpers',
    'advanced_forum',
    'author_pane',
    //'backup_migrate',
    //'backup_migrate_files',
    'checkbox_validate',
    'comment_notify',
    'diff',
    'insert',
    'less',
    'logintoboggan',
    'masquerade',
    //'mollom',
    'nodecomment',
    'nodecomment_notify',
    'node_export',
    'remember_me',
    'scheduler',
    'spambot',
    'strongarm',
    'token',
    'watchdog_rules',
    'wikitools',
    'rules',
    'rules_admin',
    'rules_scheduler',
    'voting_rules',
    'services',
    'services_keyauth',
    'xmlrpc_server',
    'system_service',
    'user_service',
    //'googleanalytics',
    'flowplayer3',
    'swftools',
    'jquery_ui',
    'userpoints',
    'userpoints_rules',
    'views',
    'views_bulk_operations',
    'views_access_php',
    'views_ui',
    'rate',
    'votingapi',
    // Lingwo
    'lingwo_entry',
    'lingwo_senses',
    'lingwo_fields',
    'lingwo_language',
    'lingwo_korpus',
    'lingwo_data',
    // For development
    'devel',
    'coder',
    // Bibliobird features are installed during a profile task.
  );
}

/**
* Return a description of the profile for the initial installation screen.
*
* @return
*   An array with keys 'name' and 'description' describing this profile.
*/
function bibliobird_profile_details() {
  return array(
    'name' => 'BiblioBird',
    'description' => 'Setup a development version of BiblioBird.',
  );
}

/**
 * Helper function defines the bibliobird modules.
 */
function _bibliobird_profile_modules() {
  return array(
    'bbcom_content',
    'bbcom_dictionary',
    //'bbcom_device',
    'bbcom_embed',
    'bbcom_flashcards',
    'bbcom_forum',
    'bbcom_login',
    'bbcom_marketing',
    'bbcom_misc',
    'bbcom_news',
    //'bbcom_notifications',
    'bbcom_wial',
    'bbcom_wiki',
  );
}

/**
 * Implementation of hook_profile_task_list().
 */
function bibliobird_profile_task_list() {
  $tasks['bibliobird-modules-batch'] = st('Set up bibliobird features');
  $tasks['bibliobird-cleanup'] = st('Cleanup tasks');
  return $tasks;
}

/**
 * Implementation of hook_profile_tasks().
 */
function bibliobird_profile_tasks(&$task, $url) {
  $output = '';

  // The profile task is called first.
  if ($task == 'profile') {
    // Begin by creating the page content type.
    $task = 'create-page-type';
  }

  // Create a page content type.
  if ($task == 'create-page-type') {
    $type = array(
      'type' => 'page',
      'name' => st('Page'),
      'module' => 'node',
      'description' => st("Use the <em>Page</em> content type for mostly static content like the \"About us\" section of a website. By default, a <em>page</em> entry does not allow comments and is not featured on the site's home page."),
      'custom' => TRUE,
      'modified' => TRUE,
      'locked' => FALSE,
      'help' => '',
      'min_word_count' => '',
    );

    $type = (object) _node_type_set_defaults($type);
    node_type_save($type);

    // Default page to not be promoted and have comments disabled.
    variable_set('node_options_page', array('status'));
    variable_set('comment_page', COMMENT_NODE_DISABLED);
    // Don't display date and author information for page nodes by default.
    $theme_settings = variable_get('theme_settings', array());
    $theme_settings['toggle_node_info_page'] = FALSE;
    variable_set('theme_settings', $theme_settings);

    // Update the menu router information.
    menu_rebuild();

    // Next is the bibliobird features install task.
    $task = 'bibliobird-modules';
  }

  // Install some more modules and maybe localization helpers too
  if ($task == 'bibliobird-modules') {
    $modules = _bibliobird_profile_modules();
    $files = module_rebuild_cache();
    // Create batch
    foreach ($modules as $module) {
      $batch['operations'][] = array('_install_module_batch', array($module, $files[$module]->info['name']));
    }    
    $batch['finished'] = '_bibliobird_profile_batch_finished'; // The finish op will set the next task.
    $batch['title'] = st('Installing @drupal', array('@drupal' => drupal_install_profile_name()));
    $batch['error_message'] = st('The installation has encountered an error.');

    // Start a batch, switch to 'bibliobird-modules-batch' task. We need to
    // set the variable here, because batch_process() redirects.
    variable_set('install_task', 'bibliobird-modules-batch');
    batch_set($batch);
    batch_process($url, $url);
    // Jut for cli installs. We'll never reach here on interactive installs.
    return;
  }
  // We are running a batch task for this profile so basically do nothing and return page
  if ($task == 'bibliobird-modules-batch') {
    include_once 'includes/batch.inc';
    $output = _batch_page();
  }

  // Our final task, clear caches and revert features.
  if ($task == 'bibliobird-cleanup') {
    // setup languages
    include_once 'includes/locale.inc';
    locale_add_language('pl', 'Polish', 'Polski', LANGUAGE_LTR, '', 'pl', TRUE);
    db_query("UPDATE {languages} SET prefix = 'en' WHERE language = 'en'");
    variable_set('language_negotiation', LANGUAGE_NEGOTIATION_PATH_DEFAULT);
    foreach (array('en', 'pl') as $langcode) {
      node_export_import(file_get_contents("../languages/$langcode.txt"));
    }

    // This isn't actually necessary as there are no node_access() entries,
    // but we run it to prevent the "rebuild node access" message from being
    // shown on install.
    node_access_rebuild();

    // Rebuild key tables/caches
    drupal_flush_all_caches();
    // Enable our themes (default and admin)
    db_query("UPDATE {system} SET status = 1 WHERE type = 'theme' and name ='%s'", 'bbcom_theme');
    db_query("UPDATE {system} SET status = 1 WHERE type = 'theme' and name ='%s'", 'rubik');
    variable_set('theme_default', 'bbcom_theme');
    variable_set('admin_theme', 'rubik');
    // Revert features to be sure everything is setup correctly.
    $revert = array(
      'bbcom_content' => array('variable'),
      'bbcom_dictionary' => array('variable'),
      'bbcom_forum' => array('variable'),
      'bbcom_wiki' => array('variable'),
      'bbcom_wial' => array('variable'),
      'bbcom_marketing' => array('variable'),
    );
    features_revert($revert);

    // Inform installation we are done.
    $task = 'profile-finished';
  }
  return $output;
}

/**
 * Finished callback for the modules install batch.
 *
 * Advance installer task to bibliobird-cleanup.
 */
function _bibliobird_profile_batch_finished($success, $results) {
  variable_set('install_task', 'bibliobird-cleanup');
}

/**
* Perform any final installation tasks for this profile.
*
* @return
*   An optional HTML string to display to the user on the final installation
*   screen.
*/
function bibliobird_profile_final() {
  
}

/**
 * Implements hook_form_alter().
 * Set bibliobird as the default profile.
 * (copied from Atrium: We use system_form_form_id_alter, otherwise we cannot alter forms.)
 */
function system_form_install_select_profile_form_alter(&$form, $form_state) {
  foreach ($form['profile'] as $key => $element) {
    $form['profile'][$key]['#value'] = 'bibliobird';
  }
}
