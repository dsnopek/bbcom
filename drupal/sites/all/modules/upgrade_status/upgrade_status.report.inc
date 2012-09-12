<?php
// $Id: upgrade_status.report.inc,v 1.2 2010/09/05 00:18:42 sun Exp $

/**
 * @file
 * Code required only when rendering the available updates report.
 */

module_load_include('inc', 'update', 'update.report');

/**
 * Menu callback. Generate a page about the upgrade status of projects.
 */
function upgrade_status_status() {
  // US: Don't pass TRUE to check cache only.
  if ($available = upgrade_status_get_available()) {
    module_load_include('inc', 'upgrade_status', 'upgrade_status.compare');
    $data = upgrade_status_calculate_project_data($available);
    return theme('upgrade_status_report', $data);
  }
  else {
    // US: Inlined _update_no_data().
    $destination = drupal_get_destination();
    return theme('upgrade_status_report', t('No information is available about currently installed modules. You can <a href="@check_manually">check manually</a>. Please note that checking for available upgrades can take a long time, so please be patient.', array(
      '@check_manually' => url('admin/reports/updates/upgrade/check', array('query' => array('query' => $destination))),
    )));
  }
}

/**
 * Theme project status report.
 *
 * @ingroup themeable
 */
function theme_upgrade_status_report($data) {
  $last = variable_get('upgrade_status_last_check', 0);
  $output = '<p>'. t("Clicking on any of the modules' boxes will expand the area and show you a link to download the new version of the project, as well as read its release notes.") .'</p>';
  $output .= '<div class="upgrade-status checked">'. ($last ? t('Last checked: @time ago', array('@time' => format_interval(time() - $last))) : t('Last checked: never'));
  $output .= ' <span class="check-manually">('. l(t('Check manually'), 'admin/reports/updates/upgrade/check') .')</span>';
  $output .= "</div>\n";

  // US: Load Update module's data as fallback in case there is no release info
  // for a project (so we can link to the project).
  $current_data = update_get_available(TRUE);

  $output .= drupal_get_form('upgrade_status_core_version_form');

  if (!is_array($data)) {
    $output .= '<p>'. $data .'</p>';
    return $output;
  }

  // US: Move 'drupal' to the top.
  $data = array('drupal' => $data['drupal']) + $data;

  $header = array();
  $rows = array();

  foreach ($data as $project) {
    // Skip upgrade_status. Doesn't make sense to display it.
    if ($project['name'] == 'upgrade_status') {
      continue;
    }
    switch ($project['status']) {
      case UPDATE_CURRENT:
      case UPGRADE_STATUS_STABLE:
      case UPGRADE_STATUS_CORE:
        $class = 'ok';
        $icon = theme('image', 'misc/watchdog-ok.png', t('ok'), t('ok'));
        break;
      case UPDATE_UNKNOWN:
      case UPDATE_NOT_FETCHED:
        $class = 'unknown';
        // US: Unknown means not ported; thus, use an error icon.
        $icon = theme('image', 'misc/watchdog-error.png', t('error'), t('error'));
        break;
      // US: Not used/possible here.
      case UPDATE_NOT_SECURE:
      case UPDATE_REVOKED:
      case UPDATE_NOT_SUPPORTED:
        $class = 'error';
        $icon = theme('image', 'misc/watchdog-error.png', t('error'), t('error'));
        break;
      // US: It doesn't make sense to output a whole page of warning symbols,
      // so we just colorize as warning without icon. This allows us to warn
      // about projects moved into core and other important info.
      case UPGRADE_STATUS_DEVELOPMENT:
        $class = 'warning';
        $icon = '';
        break;
      case UPDATE_NOT_CHECKED:
      case UPDATE_NOT_CURRENT:
      default:
        $class = 'warning';
        $icon = theme('image', 'misc/watchdog-warning.png', t('warning'), t('warning'));
        break;
    }
    // US: Special handling for project moved into core.
    if (!empty($project['in_core_note'])) {
      $icon = theme('image', 'misc/watchdog-warning.png', t('warning'), t('warning'));
    }

    // US: Compact layout.
    $row = '<div class="project">';
    $row .= '<div class="version-status">';
    switch ($project['status']) {
      case UPDATE_NOT_SECURE:
        $row .= '<span class="security-error">'. t('Security update required!') .'</span>';
        break;
      case UPDATE_REVOKED:
        $row .= '<span class="revoked">'. t('Revoked!') .'</span>';
        break;
      case UPGRADE_STATUS_DEVELOPMENT:
      // Although unsupported releases should actually be unsupported, we treat
      // them like development releases, since many maintainers merely use this
      // additional flag to hide the release from novice Drupal users.
      case UPDATE_NOT_SUPPORTED:
        $row .= '<span class="not-current">'. t('In development') .'</span>';
        break;
      case UPGRADE_STATUS_STABLE:
        $row .= '<span class="current">'. t('Available') .'</span>';
        break;
      case UPGRADE_STATUS_CORE:
        $row .= '<span class="current">'. t('In core') .'</span>';
        break;
      default:
        $row .= check_plain($project['reason']);
        break;
    }
    $row .= '<span class="icon">'. $icon .'</span>';
    $row .= "</div>\n";

    // Collapse-trigger icon.
    $row .= theme('image', 'misc/menu-collapsed.png', '', '', array('class' => 'collapse-icon'));

    $row .= '<span class="project-title">';
    if (isset($project['title'])) {
      if (isset($project['link'])) {
        $row .= l($project['title'], $project['link']);
      }
      else {
        $row .= check_plain($project['title']);
      }
    }
    // Couldn't find this project's data for the next version of Drupal core.
    // Let's try the current one instead.
    elseif (isset($current_data[$project['name']]) && isset($current_data[$project['name']]['title'])) {
      if (isset($current_data[$project['name']]['link'])) {
        $row .= l($current_data[$project['name']]['title'], $current_data[$project['name']]['link']);
      }
      else {
        $row .= check_plain($current_data[$project_name]['title']);
      }
    }
    // Otherwise, just print the name.
    else {
      $row .= check_plain($project['name']);
    }
    $row .= '</span>';
    $row .= '<span class="existing-version">';
    $row .= ' '. check_plain($project['existing_version']);
    if ($project['install_type'] == 'dev' && !empty($project['datestamp'])) {
      $row .= ' <span class="version-date">('. format_date($project['datestamp'], 'custom', 'Y-M-d') .')</span>';
    }
    $row .= '</span>';

    // Compact layout. 31/05/2008 sun
    $row .= "<div class=\"details-wrapper\">\n";
    if (!empty($project['in_core_note'])) {
      $row .= "<div class=\"core-notice\">\n";
      $row .= '<p>'. t('In Drupal core since @version', array('@version' => $project['in_core_since'])) .'</p>';
      $row .= '<p>'. $project['in_core_note'] .'</p>';
      $row .= '</div>';
    }
    $row .= "<div class=\"versions\">\n";

    if (isset($project['recommended'])) {
      if ($project['status'] != UPDATE_CURRENT || $project['existing_version'] !== $project['recommended']) {

        // First, figure out what to recommend.
        // If there's only 1 security update and it has the same version we're
        // recommending, give it the same CSS class as if it was recommended,
        // but don't print out a separate "Recommended" line for this project.
        if (!empty($project['security updates']) && count($project['security updates']) == 1 && $project['security updates'][0]['version'] === $project['recommended']) {
          $security_class = ' version-recommended version-recommended-strong';
        }
        else {
          $security_class = '';
          $version_class = 'version-recommended';
          // Apply an extra class if we're displaying both a recommended
          // version and anything else for an extra visual hint.
          if ($project['recommended'] !== $project['latest_version']
              || !empty($project['also'])
              || ($project['install_type'] == 'dev'
                 && isset($project['dev_version'])
                 && $project['latest_version'] !== $project['dev_version']
                 && $project['recommended'] !== $project['dev_version'])
              || (isset($project['security updates'][0])
                 && $project['recommended'] !== $project['security updates'][0])
              ) {
            $version_class .= ' version-recommended-strong';
          }
          $row .= theme('upgrade_status_version', $project['releases'][$project['recommended']], t('Recommended version:'), $version_class);
        }

        // Now, print any security updates.
        if (!empty($project['security updates'])) {
          foreach ($project['security updates'] as $security_update) {
            $row .= theme('upgrade_status_version', $security_update, t('Security update:'), 'version-security'. $security_class);
          }
        }
      }

      if ($project['recommended'] !== $project['latest_version']) {
        $row .= theme('upgrade_status_version', $project['releases'][$project['latest_version']], t('Latest version:'), 'version-latest');
      }
      if ($project['install_type'] == 'dev'
          && $project['status'] != UPDATE_CURRENT
          && isset($project['dev_version'])
          && $project['recommended'] !== $project['dev_version']) {
        $row .= theme('upgrade_status_version', $project['releases'][$project['dev_version']], t('Development version:'), 'version-latest');
      }
    }

    if (isset($project['also'])) {
      foreach ($project['also'] as $also) {
        $row .= theme('upgrade_status_version', $project['releases'][$also], t('Also available:'), 'version-also-available');
      }
    }

    $row .= "</div>\n"; // versions div.

    $row .= "<div class=\"info\">\n";
    if (!empty($project['extra'])) {
      $row .= '<div class="extra">'."\n";
      foreach ($project['extra'] as $key => $value) {
        $row .= '<div class="'. $value['class'] .'">';
        $row .= check_plain($value['label']) .': ';
        $row .= theme('placeholder', $value['data']);
        $row .= "</div>\n";
      }
      $row .= "</div>\n";  // extra div.
    }

    $row .= '<div class="includes">';
    sort($project['includes']);
    $row .= t('Includes: %includes', array('%includes' => implode(', ', $project['includes'])));
    $row .= "</div>\n";

    if (!empty($project['base_themes'])) {
      $row .= '<div class="basethemes">';
      sort($project['base_themes']);
      // We use !dependencies and manually call theme('placeholder') here to
      // avoid breakding the D6 string freeze. This identical string is
      // already in modules/system/system.admin.inc.
      $row .= t('Depends on: !dependencies', array('!dependencies' => theme('placeholder', implode(', ', $project['base_themes']))));
      $row .= "</div>\n";
    }

    if (!empty($project['sub_themes'])) {
      $row .= '<div class="subthemes">';
      sort($project['sub_themes']);
      // We use !required and manually call theme('placeholder') here to avoid
      // breakding the D6 string freeze. This identical string is already in
      // modules/system/system.admin.inc.
      $row .= t('Required by: !required', array('!required' => theme('placeholder', implode(', ', $project['sub_themes']))));
      $row .= "</div>\n";
    }

    $row .= "</div>\n"; // info div.

    if (!isset($rows[$project['project_type']])) {
      $rows[$project['project_type']] = array();
    }
    $row_key = isset($project['title']) ? drupal_strtolower($project['title']) : drupal_strtolower($project['name']);
    $rows[$project['project_type']][$row_key] = array(
      'class' => $class,
      'data' => array($row),
    );
  }

  $project_types = array(
    'core' => t('Drupal core'),
    'module' => t('Modules'),
    'theme' => t('Themes'),
    'disabled-module' => t('Disabled modules'),
    'disabled-theme' => t('Disabled themes'),
  );
  foreach ($project_types as $type_name => $type_label) {
    if (!empty($rows[$type_name])) {
      ksort($rows[$type_name]);
      $output .= "\n<h3>". $type_label ."</h3>\n";
      $output .= theme('table', $header, $rows[$type_name], array('class' => 'upgrade-status'));
    }
  }
  drupal_add_css(drupal_get_path('module', 'upgrade_status') .'/upgrade_status.css');
  drupal_add_js(drupal_get_path('module', 'upgrade_status') .'/upgrade_status.js');
  return $output;
}

/**
 * Theme the version display of a project.
 *
 * @ingroup themeable
 */
function theme_upgrade_status_version($version, $tag, $class) {
  $output = '';
  $output .= '<table class="version '. $class .'">';
  $output .= '<tr>';
  $output .= '<td class="version-title">'. $tag ."</td>\n";
  $output .= '<td class="version-details">';
  $output .= l($version['version'], $version['release_link']);
  $output .= ' <span class="version-date">('. format_date($version['date'], 'custom', 'Y-M-d') .')</span>';
  $output .= "</td>\n";
  $output .= '<td class="version-links">';
  $links = array();
  $links['update-download'] = array(
    'title' => t('Download'),
    'href' => $version['download_link'],
  );
  $links['update-release-notes'] = array(
    'title' => t('Release notes'),
    'href' => $version['release_link'],
  );
  $output .= theme('links', $links);
  $output .= '</td>';
  $output .= '</tr>';
  $output .= "</table>\n";
  return $output;
}

/**
 * Form to display Drupal core version selection. 
 */
function upgrade_status_core_version_form(&$form_state) {
  $form['upgrade_status_core_version'] = array(
    '#type' => 'select',
    '#title' => t('Target version of Drupal core'),
    '#options' => drupal_map_assoc(array('7.x', '8.x')),
    '#default_value' => variable_get('upgrade_status_core_version', UPGRADE_STATUS_CORE_VERSION),
    '#description' => t('Select the version of Drupal core you wish to check for project status.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Change'),
  );
  return $form;
}

/**
 * Set the new Drupal core version in a variable; refresh project data.
 */
function upgrade_status_core_version_form_submit($form, &$form_state) {
  // Refresh status if a different version than the default has been selected.
  if ($form_state['values']['upgrade_status_core_version'] != UPGRADE_STATUS_CORE_VERSION) {
    variable_set('upgrade_status_core_version', $form_state['values']['upgrade_status_core_version']);
    upgrade_status_refresh();
  }
  // Refresh status if the default version has been selected, but only if it
  // was not selected before (noob protection).
  else if (variable_get('upgrade_status_core_version', UPGRADE_STATUS_CORE_VERSION) != UPGRADE_STATUS_CORE_VERSION) {
    variable_del('upgrade_status_core_version');
    upgrade_status_refresh();
  }
  // ...or just ensure that we've got no stale variable set.
  else {
    variable_del('upgrade_status_core_version');
  }
}

