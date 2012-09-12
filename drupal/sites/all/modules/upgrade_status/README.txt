/* $Id: README.txt,v 1.4 2010/09/04 23:53:07 sun Exp $ */

-- SUMMARY --

Upgrade Status module was designed to provide an easy way tell when your website
is ready to be upgraded to the next Drupal version. The module will compile a
list of your projects along with a status, which can be one of the following:

* Available: A stable release of this project is available.
* In development: A development release of this project is available, which can
  be installed for testing purposes.
* Not ported yet: There are no releases available for this project.

Clicking on any of the modules' boxes will expand the area and show you a link
to download the new version of the project, as well as read its release notes.

If one or more of your installed modules are not yet ported to a new Drupal
major version, you should

1) search the modules' issue queue for already existing issues that might
   contain a patch, and test that patch yourself.  See
   http://drupal.org/patch/apply for further information.

2) go ahead and install Coder module (http://drupal.org/project/coder), use its
   code review rules for migrating a module to the new Drupal version, create a
   patch, and file a new issue against that project with your patch attached.
   See http://drupal.org/patch/create for further information.

If you've checked your projects out from CVS, you will need the CVS Deploy
module (http://drupal.org/project/cvs_deploy) in order for this module to be
able to read versions.


For a full description visit the project page:
  http://drupal.org/project/upgrade_status
Bug reports, feature suggestions and latest developments:
  http://drupal.org/project/issues/upgrade_status


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- USAGE --

* Go to Administer >> Reports >> Available updates >> Upgrade status and check
  the status of your installed modules.


-- CONTACT --

Current maintainers:
* Daniel F. Kudwien (sun) - dev@unleashedmind.com


This project has been sponsored by:
* UNLEASHED MIND
  Specialized in consulting and planning of Drupal powered sites, UNLEASHED
  MIND offers installation, development, theming, customization, and hosting
  to get you started. Visit http://www.unleashedmind.com for more information.

