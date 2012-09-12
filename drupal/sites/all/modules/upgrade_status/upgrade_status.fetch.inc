<?php
// $Id: upgrade_status.fetch.inc,v 1.1 2010/09/04 23:53:07 sun Exp $

/**
 * @file
 * Code required only when fetching information about available updates.
 */

module_load_include('inc', 'update', 'update.fetch');

/**
 * Callback to manually check the update status without cron.
 */
function upgrade_status_manual_status() {
  if (_upgrade_status_refresh()) {
    drupal_set_message(t('Attempted to fetch information about all available new releases and upgrades.'));
  }
  else {
    drupal_set_message(t('Unable to fetch any information about available new releases and updates.'), 'error');
  }
  drupal_goto('admin/reports/updates/upgrade');
}

/**
 * Fetch project info via XML from a central server.
 */
function _upgrade_status_refresh() {
  static $fail = array();
  global $base_url;
  module_load_include('inc', 'upgrade_status', 'upgrade_status.compare');

  // Uncomment to debug available releases calculations.
//  $cache = _update_cache_get('upgrade_status_available_releases');
//  if ($cache) {
//    return $cache->data;
//  }
//  dsm("REFETCHED");

  // US: Attempt to fix timeouts. Remove in D7.
  @set_time_limit(240);

  // Since we're fetching new available update data, we want to clear
  // our cache of both the projects we care about, and the current update
  // status of the site. We do *not* want to clear the cache of available
  // releases just yet, since that data (even if it's stale) can be useful
  // during update_get_projects(); for example, to modules that implement
  // hook_system_info_alter() such as cvs_deploy.
  _update_cache_clear('upgrade_status_project_projects');
  _update_cache_clear('upgrade_status_project_data');

  $available = array();
  $data = array();
#  $site_key = md5($base_url . drupal_get_private_key());
  $projects = update_get_projects();

  $version = variable_get('upgrade_status_core_version', UPGRADE_STATUS_CORE_VERSION);

  // Now that we have the list of projects, we should also clear our cache of
  // available release data, since even if we fail to fetch new data, we need
  // to clear out the stale data at this point.
  _update_cache_clear('upgrade_status_available_releases');
  $max_fetch_attempts = variable_get('update_max_fetch_attempts', UPDATE_MAX_FETCH_ATTEMPTS);
  
  foreach ($projects as $key => $project) {
    // US: No site key to avoid hi-jacking module usage statistics.
    $url = _upgrade_status_build_fetch_url($project, $version);
    $fetch_url_base = _update_get_fetch_url_base($project);
    if (empty($fail[$fetch_url_base]) || count($fail[$fetch_url_base]) < $max_fetch_attempts) {
      $xml = drupal_http_request($url);
      if (isset($xml->data)) {
        $data[] = $xml->data;
      }
      else {
        // Connection likely broken; prepare to give up.
        $fail[$fetch_url_base][$key] = 1;
      }
    }
    else {
      // Didn't bother trying to fetch.
      $fail[$fetch_url_base][$key] = 1;
    }
  }

  if ($data) {
    $parser = new update_xml_parser;
    $available = $parser->parse($data);
  }
  if (!empty($available) && is_array($available)) {
    // Record the projects where we failed to fetch data.
    foreach ($fail as $fetch_url_base => $failures) {
      foreach ($failures as $key => $value) {
        $available[$key]['project_status'] = 'not-fetched';
      }
    }
    $frequency = variable_get('upgrade_status_check_frequency', 1);
    _update_cache_set('upgrade_status_available_releases', $available, time() + (60 * 60 * 24 * $frequency));
    watchdog('upgrade_status', 'Attempted to fetch information about all available new releases and updates.', array(), WATCHDOG_NOTICE, l(t('view'), 'admin/reports/updates/upgrade'));
  }
  else {
    watchdog('upgrade_status', 'Unable to fetch any information about available new releases and updates.', array(), WATCHDOG_ERROR, l(t('view'), 'admin/reports/updates/upgrade'));
  }
  // Whether this worked or not, we did just (try to) check for updates.
  variable_set('upgrade_status_last_check', time());
  return $available;
}

/**
 * Generates the URL to fetch information about project updates.
 *
 * This figures out the right URL to use, based on the project's .info file
 * and the global defaults. Appends optional query arguments when the site is
 * configured to report usage stats.
 *
 * @param $project
 *   The array of project information from update_get_projects().
 * @param $version
 *   The target version of Drupal core you wish to query.
 *
 * @see upgrade_status_refresh()
 * @see update_get_projects()
 */
function _upgrade_status_build_fetch_url($project, $version) {
  $name = $project['name'];
  $url = _update_get_fetch_url_base($project);
  $url .= '/'. $name .'/'. $version;
  return $url;
}

