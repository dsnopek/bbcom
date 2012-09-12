<?php
// $Id: upgrade_status.compare.inc,v 1.3 2010/09/05 00:25:24 sun Exp $

/**
 * @file
 * Code required only when comparing available updates to existing data.
 */

module_load_include('inc', 'update', 'update.compare');

/**
 * Given the installed projects and the available release data retrieved from
 * remote servers, calculate the current status.
 *
 * This function is the heart of the update status feature. It iterates over
 * every currently installed project. For each one, it first checks if the
 * project has been flagged with a special status like "unsupported" or
 * "insecure", or if the project node itself has been unpublished. In any of
 * those cases, the project is marked with an error and the next project is
 * considered.
 *
 * If the project itself is valid, the function decides what major release
 * series to consider. The project defines what the currently supported major
 * versions are for each version of core, so the first step is to make sure
 * the current version is still supported. If so, that's the target version.
 * If the current version is unsupported, the project maintainer's recommended
 * major version is used. There's also a check to make sure that this function
 * never recommends an earlier release than the currently installed major
 * version.
 *
 * Given a target major version, it scans the available releases looking for
 * the specific release to recommend (avoiding beta releases and development
 * snapshots if possible). This is complicated to describe, but an example
 * will help clarify. For the target major version, find the highest patch
 * level. If there is a release at that patch level with no extra ("beta",
 * etc), then we recommend the release at that patch level with the most
 * recent release date. If every release at that patch level has extra (only
 * betas), then recommend the latest release from the previous patch
 * level. For example:
 *
 * 1.6-bugfix <-- recommended version because 1.6 already exists.
 * 1.6
 *
 * or
 *
 * 1.6-beta
 * 1.5 <-- recommended version because no 1.6 exists.
 * 1.4
 *
 * It also looks for the latest release from the same major version, even a
 * beta release, to display to the user as the "Latest version" option.
 * Additionally, it finds the latest official release from any higher major
 * versions that have been released to provide a set of "Also available"
 * options.
 *
 * Finally, and most importantly, it keeps scanning the release history until
 * it gets to the currently installed release, searching for anything marked
 * as a security update. If any security updates have been found between the
 * recommended release and the installed version, all of the releases that
 * included a security fix are recorded so that the site administrator can be
 * warned their site is insecure, and links pointing to the release notes for
 * each security update can be included (which, in turn, will link to the
 * official security announcements for each vulnerability).
 *
 * This function relies on the fact that the .xml release history data comes
 * sorted based on major version and patch level, then finally by release date
 * if there are multiple releases such as betas from the same major.patch
 * version (e.g. 5.x-1.5-beta1, 5.x-1.5-beta2, and 5.x-1.5). Development
 * snapshots for a given major version are always listed last.
 *
 * The results of this function are expensive to compute, especially on sites
 * with lots of modules or themes, since it involves a lot of comparisons and
 * other operations. Therefore, we cache the results into the {cache_update}
 * table using the 'update_project_data' cache ID. However, since this is not
 * the data about available updates fetched from the network, it is ok to
 * invalidate it somewhat quickly. If we keep this data for very long, site
 * administrators are more likely to see incorrect results if they upgrade to
 * a newer version of a module or theme but do not visit certain pages that
 * automatically clear this cache.
 *
 * @param $available
 *  Array of data about available project releases.
 *
 * @see upgrade_status_get_available()
 * @see update_get_projects()
 * @see update_process_project_info()
 */
function upgrade_status_calculate_project_data($available) {
  // Retrieve the projects from cache, if present.
  // US: Directly use private cache getter to skip Update's cache invalidation.
  $projects = _update_cache_get('upgrade_status_project_data');
  // If $projects is empty, then the cache must be rebuilt.
  // Otherwise, return the cached data and skip the rest of the function.
  if (!empty($projects)) {
    return $projects->data;
  }
  $projects = update_get_projects();
  update_process_project_info($projects);
  $settings = variable_get('upgrade_status_settings', array());
  foreach ($projects as $project => $project_info) {
    if (isset($available[$project])) {

      // If the project status is marked as something bad, there's nothing
      // else to consider.
      if (isset($available[$project]['project_status'])) {
        switch ($available[$project]['project_status']) {
          case 'insecure':
            $projects[$project]['status'] = UPDATE_NOT_SECURE;
            if (empty($projects[$project]['extra'])) {
              $projects[$project]['extra'] = array();
            }
            $projects[$project]['extra'][] = array(
              'class' => 'project-not-secure',
              'label' => t('Project not secure'),
              'data' => t('This project has been labeled insecure by the Drupal security team, and is no longer available for download. Immediately disabling everything included by this project is strongly recommended!'),
            );
            break;
          // US: Maintainers are doing lots of nightmares with in development
          // releases, so we have to take unpublished, revoked, and unsupported
          // into account.
          case 'unpublished':
          case 'revoked':
          case 'unsupported':
            break;
          case 'not-fetched':
            $projects[$project]['status'] = UPDATE_NOT_FETCHED;
            $projects[$project]['reason'] = t('Failed to fetch available update data');
            break;

          default:
            // Assume anything else (e.g. 'published') is valid and we should
            // perform the rest of the logic in this function.
            break;
        }
      }

      if (!empty($projects[$project]['status'])) {
        // We already know the status for this project, so there's nothing
        // else to compute. Just record everything else we fetched from the
        // XML file into our projects array and move to the next project.
        $projects[$project] += $available[$project];
        continue;
      }

      // Figure out the target major version.
      $existing_major = $project_info['existing_major'];
      $supported_majors = array();
      if (isset($available[$project]['supported_majors'])) {
        $supported_majors = explode(',', $available[$project]['supported_majors']);
      }
      elseif (isset($available[$project]['default_major'])) {
        // Older release history XML file without supported or recommended.
        $supported_majors[] = $available[$project]['default_major'];
      }

      if (in_array($existing_major, $supported_majors)) {
        // Still supported, stay at the current major version.
        $target_major = $existing_major;
      }
      elseif (isset($available[$project]['recommended_major'])) {
        // Since 'recommended_major' is defined, we know this is the new XML
        // format. Therefore, we know the current release is unsupported since
        // its major version was not in the 'supported_majors' list. We should
        // find the best release from the recommended major version.
        $target_major = $available[$project]['recommended_major'];
        // US: Projects may port from 6.x-1.x to 7.x-2.x to change their APIs.
#        $projects[$project]['status'] = UPDATE_NOT_SUPPORTED;
      }
      elseif (isset($available[$project]['default_major'])) {
        // Older release history XML file without recommended, so recommend
        // the currently defined "default_major" version.
        $target_major = $available[$project]['default_major'];
      }
      else {
        // Malformed XML file? Stick with the current version.
        $target_major = $existing_major;
      }

      // US: Some projects are renumbering to 1.x with each new core version.
#      $target_major = max($existing_major, $target_major);

      $version_patch_changed = '';
      $patch = '';

      // US: Of course, not yet ported don't have any releases. ;)
#      // Defend ourselves from XML history files that contain no releases.
#      if (empty($available[$project]['releases'])) {
#        $projects[$project]['status'] = UPDATE_UNKNOWN;
#        $projects[$project]['reason'] = t('No available releases found');
#        continue;
#      }
      foreach ($available[$project]['releases'] as $version => $release) {
        // US: insecure, unpublished, revoked, unsupported have no meaning.

        // See if this is a higher major version than our target and yet still
        // supported. If so, record it as an "Also available" release.
        if ($release['version_major'] > $target_major) {
          if (in_array($release['version_major'], $supported_majors)) {
            if (!isset($available[$project]['also'])) {
              $available[$project]['also'] = array();
            }
            if (!isset($available[$project]['also'][$release['version_major']])) {
              $available[$project]['also'][$release['version_major']] = $version;
            }
          }
          // US: Some projects are renumbering to 1.x with each new core version.
#          continue;
        }

        // Look for the 'latest version' if we haven't found it yet. Latest is
        // defined as the most recent version for the target major version.
        if (!isset($available[$project]['latest_version'])
            && $release['version_major'] == $target_major) {
          $available[$project]['latest_version'] = $version;
        }

        // Look for the development snapshot release for this branch.
        if (!isset($available[$project]['dev_version'])
            && $release['version_major'] == $target_major
            && isset($release['version_extra'])
            && $release['version_extra'] == 'dev') {
          $available[$project]['dev_version'] = $version;
        }

        // Look for the 'recommended' version if we haven't found it yet (see
        // phpdoc at the top of this function for the definition).
        if (!isset($available[$project]['recommended'])
            && $release['version_major'] == $target_major
            && isset($release['version_patch'])) {
          if ($patch != $release['version_patch']) {
            $patch = $release['version_patch'];
            $version_patch_changed = $release['version'];
          }
          if (empty($release['version_extra']) && $patch == $release['version_patch']) {
            $available[$project]['recommended'] = $version_patch_changed;
          }
        }

        // US: Don't stop searching, even if we hit the currently installed version.

        // US: Ignore dev snapshot handling.

        // US: Ignore security updates.
      }

      // If we were unable to find a recommended version, then make the latest
      // version the recommended version if possible.
      if (!isset($available[$project]['recommended']) && isset($available[$project]['latest_version'])) {
        $available[$project]['recommended'] = $available[$project]['latest_version'];
        // US: No recommended version means there's a dev snapshot.
        $projects[$project]['status'] = UPGRADE_STATUS_DEVELOPMENT;
        $projects[$project]['reason'] = t('In development');
      }

      // Stash the info about available releases into our $projects array.
      $projects[$project] += $available[$project];

      //
      // Check to see if we need an update or not.
      //

      // US: Skip security update status handling.

      // US: Check new Drupal core improvements, regardless of what's figured
      // out below.
      if (upgrade_status_moved_into_core($projects, $project)) {
        $projects[$project]['status'] = UPGRADE_STATUS_CORE;
        $projects[$project]['reason'] = t('In core');
      }

      if (isset($projects[$project]['status'])) {
        // If we already know the status, we're done.
        continue;
      }

      // If we don't know what to recommend, there's nothing we can report.
      // Bail out early.
      if (!isset($projects[$project]['recommended'])) {
        $projects[$project]['status'] = UPDATE_UNKNOWN;
        $projects[$project]['reason'] = t('No available releases found');
        continue;
      }

      // US: Ignore dev snapshot handling.

      // Figure out the status, based on what we've seen and the install type.
      // Note: If we were not yet able to assign a status, this project already
      // provides a stable release.
      switch ($projects[$project]['install_type']) {
        case 'official':
        case 'dev':
          $projects[$project]['status'] = UPGRADE_STATUS_STABLE;
          $projects[$project]['reason'] = t('Available');
          break;

        default:
          $projects[$project]['status'] = UPDATE_UNKNOWN;
          $projects[$project]['reason'] = t('Invalid info');
      }
    }
    // US: A project without releases may be in core.
    elseif (upgrade_status_moved_into_core($projects, $project)) {
      $projects[$project]['status'] = UPGRADE_STATUS_CORE;
      $projects[$project]['reason'] = t('In core');
    }
    else {
      $projects[$project]['status'] = UPDATE_UNKNOWN;
      $projects[$project]['reason'] = t('No available releases found');
    }
  }
  // Give other modules a chance to alter the status (for example, to allow a
  // contrib module to provide fine-grained settings to ignore specific
  // projects or releases).
  drupal_alter('update_status', $projects);
  // US: Same for us, afterwards.
  drupal_alter('upgrade_status', $projects);

  // Cache the site's update status for at most 1 hour.
  _update_cache_set('upgrade_status_project_data', $projects, time() + 3600);
  return $projects;
}

/**
 * Return status and notice about modules moved into Core.
 *
 * Assign custom upgrade information for certain modules.
 *
 * @param $projects
 *   Array of projects from upgrade_status_calculate_project_data().
 * @param $project
 *   Project name to check.
 * @return
 *   TRUE if module has been moved into core.
 *
 * @todo Handle partial core additions, e.g.
 *   - Token
 *   - CTools: AJAX framework
 *   - Taxonomy image
 *   - Better Formats, Filter by node type
 *   For cleanly coded and separated modules, allowing to target sub-modules in
 *   a project might be sufficient already.
 *
 * @todo Check whether 'help' keys make sense. Users having those modules
 *   installed should already know what the modules are doing. This help info
 *   was kept, because the work was done, but it might not make sense at all.
 */
function upgrade_status_moved_into_core(&$projects, $project) {
  $core = TRUE;
  switch ($project) {
    case 'ahah_helper':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['help'] = t('Allows Drupal modules to implement AHAH/AJAX functionality without touching JavaScript. Entirely covered by the new AJAX framework in Drupal 7, which is based on <a href="@ctools-url">CTools</a>.', array(
        '@ctools-url' => 'http://drupal.org/project/ctools',
      ));
      break;

    case 'autoload':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['help'] = t('Allows Drupal modules to lazy-load any class that has not been loaded yet. A major performance improvement in Drupal 7, and the only part of the <a href="@issue-url">Registry</a> that was introduced earlier, but later removed from Drupal 7.', array(
        '@issue-url' => 'http://drupal.org/node/221964',
      ));
      break;

    case 'auto_menutitle':
    case 'automaticmenu':
    case 'automenu':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['help'] = t('When creating new content, Drupal 7 automatically takes over the title of the content as menu link title, while still allowing you to alter it.');
      break;

    case 'admin_hover':
    case 'block_edit':
    case 'contextual':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['help'] = t('The new <a href="@issue-url">Contextual links</a> module in Drupal 7 allows you to manage page elements from the page you are looking at, i.e., you have direct access to "Edit" and "Delete" pages for content, or "Configure" pages for blocks, "List links" for menu blocks, etc. Every module can integrate with Contextual module.', array(
        '@issue-url' => 'http://drupal.org/node/473268',
      ));
      break;

    case 'adminrole':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['help'] = t('The <a href="@issue-url">administration role</a> is based on regular Drupal user roles and every user role can be configured to be the administration role. All new permissions (and only new) are automatically granted to the configured administration role. Permissions can still be removed from the role.', array(
        '@issue-url' => 'http://drupal.org/node/480660',
      ));
      break;

    case 'block_node_visibility':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'blocks404':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'button_style':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'canonical_url':
    case 'shortlink':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'cck':
    case 'content':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_note'] = t('You still need <a href="@cck-url">CCK</a> for the Nodereference and Userreference field types, and to upgrade your fields to Drupal 7. There is an overall CCK to field <a href="@upgrade-issue-url">upgrade path discussion</a>. Contributed modules may use <a href="@field-convert-url">Field converter</a> as dependency to properly upgrade their non-field data to fields. Custom programmed field type modules, which may be obsolete now, might be easier to upgrade using <a href="@migrate-url">Migrate</a> module.', array(
        '@upgrade-issue-url' => 'http://drupal.org/node/366364',
        '@cck-url' => 'http://drupal.org/project/cck',
        '@field-convert-url' => 'http://drupal.org/project/field_convert',
        '@migrate-url' => 'http://drupal.org/project/migrate',
      ));
      break;

    case 'checkbox_validate':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'comment_display':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'config_perms':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'content_taxonomy':
    case 'field_taxonomy':
    case 'term_fields':
    case 'taxidermy':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    // @todo Allow to target a module (not project).
    case 'date_timezone':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'dbtng':
    case 'transaction':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['help'] = t('Drupal 7 implements an entirely new <a href="@issue-url">database layer</a>, which allows Drupal to work with any database. Modules are able to alter most database queries and there is support for transactions.', array(
        '@issue-url' => 'http://drupal.org/node/225450',
      ));
      break;

    case 'edit_term':
    case 'taxonomy_intro':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'elements':
    case 'element_themehook':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['help'] = t('In Drupal 7, almost all content is generated as "renderable array", which allows to consider any element on a page as atomic, alterable, and themeable element that can be still be altered until it is rendered into a string.');
      break;

    case 'hook_file':
    case 'storage_api':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['help'] = t('Drupal 7 natively uses PHP 5 stream wrappers, which allow to store and access files in almost any location, even remotely. There is also a clean separation between the public and private filesystem, and both can be used at the same time.');
      break;

    case 'filefield':
    // @todo Allow to target a module (not project).
    case 'upload':
    case 'upload_element':
    case 'upload_preview':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_note'] = t('<a href="@issue-url">Upload</a> module has been replaced with File field.', array(
        '@issue-url' => 'http://drupal.org/node/563000',
      ));
      break;

    case 'filter_default':
    case 'input_format_permissions':
    case 'user_default_filter':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'image':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_note'] = t('The most common use-case of <a href="@image-url">Image</a> module, an image field type, is contained in Drupal core. The Image project is required to <a href="@upgrade-issue-url">upgrade</a> existing data, and its main image module has been renamed to image_node module, since Drupal core took over the namespace. Image Node module is required for use-cases like restricting access, direct image access through a URL, attached data like comments, votes, or geo coordinates.', array(
        '@image-url' => 'http://drupal.org/project/image',
        '@upgrade-issue-url' => 'http://drupal.org/node/513096',
      ));
      break;

    case 'imageapi':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_note'] = t('The <a href="@imageapi-url">ImageAPI</a> module for Drupal 7 only provides the ImageMagick toolkit and an unsharpen mask action. Everything else has been moved into Drupal core.', array(
        '@imageapi-url' => 'http://drupal.org/project/imageapi',
      ));
      break;

    case 'imagecache':
    case 'imagefield':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'jq':
    case 'jqp':
    case 'plugins':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_note'] = t('Drupal 7 allows modules to register custom libraries, consisting of JavaScript and CSS files, which can then be loaded at once. External libraries, i.e., code that is not shipped with a module, is not supported by Drupal core and requires the <a href="@libraries-url">Libraries API</a> module.', array(
        '@libraries-url' => 'http://drupal.org/project/libraries',
      ));
      break;

    case 'jquery_cookie':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'jquery_form_update':
    case 'jsalter':
    case 'wysiwygcck':
    case 'tinymce_ahah':
    case 'tinymce_dragdrop':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'jquery_ui':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_note'] = t('Advanced jQuery UI features (like Theme Roller support) are not in Drupal core.');
      break;

    case 'login_security':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_note'] = t('Drupal core provides no UI. If required, the internal variables may be configured using the <a href="@flood-control-url">Flood control</a> module.', array(
        '@flood-control-url' => 'http://drupal.org/project/flood_control',
      ));
      break;

    case 'menuless_nodetype':
    case 'ctm':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'nodepreview_by_type':
    case 'preview':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'permissions_api':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'phpass':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_note'] = t('All stored user passwords will be additionally salted');
      break;

    case 'plugin_manager':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'poormanscron':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'protect_critical_users':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'drupal_queue':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'rdf':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'seven':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'simplecdn':
    case 'abssrc':
    case 'parallel':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_note'] = t('The new File API and handling of JavaScript  and CSS in Drupal 7 allows modules to alter the URLs of all files. Everyone is encouraged to switch to the joined community effort, the <a href="@cdn-url">CDN</a> project.', array(
        '@cdn-url' => 'http://drupal.org/project/cdn',
      ));
      break;

    case 'simpletest':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'tar':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'taxonomy_delegate':
    case 'vocabperms':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'token':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_note'] = t('Drupal core does not provide a user interface to browse tokens (in forms).');
      break;

    case 'url_alter':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'user_cancellation':
    case 'user_delete':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'vertical_tabs':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'view_unpublished':
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'actions':
      $projects[$project]['in_core_since'] = '6.x';
      $projects[$project]['in_core_note'] = t('Please note that the syntax for actions used by the 5.x-1.x and 4.7 versions of Actions module are different to triggers in Drupal 6.x. For further information please refer to the <a href="!project-url">Actions</a> module project page.', array('!project-url' => 'http://drupal.org/project/actions'));
      break;

    case 'ahah_forms':
      $projects[$project]['in_core_since'] = '6.x';
      break;

    case 'autolocale':
      $projects[$project]['in_core_since'] = '6.x';
      break;

    case 'book_bridge':
      $projects[$project]['in_core_since'] = '6.x';
      break;

    case 'find_path':
      $projects[$project]['in_core_since'] = '6.x';
      break;

    case 'html_to_text':
      $projects[$project]['in_core_since'] = '6.x';
      break;

    case 'htmlcorrector':
      $projects[$project]['in_core_since'] = '6.x';
      break;

    case 'javascript_aggregator':
      $projects[$project]['in_core_since'] = '6.x';
      $projects[$project]['in_core_note'] = t('Please note that there is a version of <a href="!project-url">JavaScript Aggregator</a> module for Drupal 6.x, which additionally minifies JavaScript.', array('!project-url' => 'http://drupal.org/project/javascript_aggregator'));
      break;

    case 'openid':
      $projects[$project]['in_core_since'] = '6.x';
      break;

    case 'themesettingsapi':
      $projects[$project]['in_core_since'] = '6.x';
      break;

    case 'update_status':
      $projects[$project]['in_core_since'] = '6.x';
      $projects[$project]['in_core_note'] = t('Please note that some of the advanced settings in the 5.x version of Update status are not present in the update.module in 6.x core, and have been moved into the <a href="!project-url">Update status advanced settings</a> module for Drupal 6.x and beyond.', array('!project-url' => 'http://drupal.org/project/update_advanced'));
      break;

    case 'user_status':
      $projects[$project]['in_core_since'] = '6.x';
      $projects[$project]['in_core_note'] = t('There is no database upgrade path for sites that used the 5.x version of the user_status module to migrate the message templates to the new settings in core. Furthermore, the place-holders available in these templates are different in 6.x core. Therefore, users will have to re-enter their message templates into the core settings at admin/user/settings.');
      break;

    default:
      $core = FALSE;
  }
  return $core;
}

