core = 6.x
api = 2

; Contrib modules

projects[lingwo][type] = module
projects[lingwo][subdir] = contrib
projects[lingwo][download][type] = git
projects[lingwo][download][url] = http://github.com/dsnopek/lingwo

projects[lingwo-old][type] = module
projects[lingwo-old][subdir] = contrib
projects[lingwo-old][download][type] = git
projects[lingwo-old][download][url] = http://github.com/dsnopek/lingwo-old

projects[admin][subdir] = contrib
projects[admin][version] = "2.0"

projects[adminrole][subdir] = contrib
projects[adminrole][version] = "1.3"

projects[advanced_forum][subdir] = contrib
projects[advanced_forum][version] = "2.0-alpha4"

projects[advanced_help][subdir] = contrib
projects[advanced_help][version] = "1.2"

projects[author_pane][subdir] = contrib
projects[author_pane][version] = "2.1"

projects[autoload][subdir] = contrib
projects[autoload][version] = "2.0"

projects[better_formats][subdir] = contrib
projects[better_formats][version] = "1.2"

projects[ctools][subdir] = contrib
projects[ctools][version] = "1.8"

projects[checkbox_validate][subdir] = contrib
projects[checkbox_validate][version] = "2.1"

projects[services][subdir] = contrib
projects[services][version] = "2.4"
projects[services][patch][] = "https://raw.github.com/dsnopek/bbcom/master/drupal-patches/services/services_pressflow.patch"

; We use a particular version on the 3.x-dev stream
projects[cck][subdir] = contrib
projects[cck][type] = module
projects[cck][download][type] = git
projects[cck][download][url] = http://git.drupal.org/project/cck.git
projects[cck][download][revision] = "288c628723c5461c4c692cecad61556e3b395902"

projects[context][subdir] = contrib
projects[context][version] = "3.0"

projects[ctools_block][subdir] = contrib
projects[ctools_block][version] = "1.0"

projects[date][subdir] = contrib
projects[date][version] = "2.8"

projects[devel][subdir] = contrib
projects[devel][version] = "1.22"

projects[diff][subdir] = contrib
projects[diff][version] = "2.1"

projects[features][subdir] = contrib
projects[features][version] = "1.0"

projects[filefield][subdir] = contrib
projects[filefield][version] = "3.9"

projects[filefield_paths][subdir] = contrib
projects[filefield_paths][version] = "1.4"

projects[flag][subdir] = contrib
projects[flag][version] = "1.2"
projects[flag][patch][] = "https://raw.github.com/dsnopek/bbcom/master/drupal-patches/flag/php53-undefined-index.patch"
projects[flag][patch][] = "https://raw.github.com/dsnopek/bbcom/master/drupal-patches/flag/views-not-flagged.patch"

projects[swftools][subdir] = contrib
projects[swftools][version] = "2.5"
projects[swftools][patch][] = "https://raw.github.com/dsnopek/bbcom/master/drupal-patches/swftools/swftools_2_5_php_5_3.patch"

projects[i18n][subdir] = contrib
projects[i18n][version] = "1.7"

projects[i18nviews][subdir] = contrib
projects[i18nviews][version] = "2.0"

projects[imageapi][subdir] = contrib
projects[imageapi][version] = "1.10"

projects[imagecache][subdir] = contrib
projects[imagecache][version] = "2.0-beta12"

projects[imagefield][subdir] = contrib
projects[imagefield][version] = "3.9"

projects[insert][subdir] = contrib
projects[insert][version] = "1.1"

projects[jquery_ui][subdir] = contrib
projects[jquery_ui][version] = "1.3"

projects[l10n_update][subdir] = contrib
projects[l10n_update][version] = "1.0-alpha2"

projects[less][subdir] = contrib
projects[less][version] = "2.2"

projects[lightbox2][subdir] = contrib
projects[lightbox2][version] = "1.11"

projects[logintoboggan][subdir] = contrib
projects[logintoboggan][version] = "1.7"
projects[logintoboggan][patch][] = "https://raw.github.com/dsnopek/bbcom/master/drupal-patches/logintoboggan/logintoboggan-after_update.patch"

projects[masquerade][subdir] = contrib
projects[masquerade][version] = "1.5"

; We use a specific version in the 4.x-dev stream
projects[messaging][subdir] = contrib
projects[messaging][type] = module
projects[messaging][download][type] = git
projects[messaging][download][url] = http://git.drupal.org/project/messaging.git
projects[messaging][download][revision] = "bd5b16bb7c228cad76e9701d5118b03dfa77ced5"
projects[messaging][patch][] = "https://raw.github.com/dsnopek/bbcom/master/drupal-patches/messaging/e6012b4.txt"

projects[node_export][subdir] = contrib
projects[node_export][version] = "2.24"

projects[nodecomment][subdir] = contrib
projects[nodecomment][version] = "2.0-beta6"
projects[nodecomment][patch][] = "https://raw.github.com/dsnopek/bbcom/master/drupal-patches/nodecomment/nodecomment-1088106.patch"
projects[nodecomment][patch][] = "https://raw.github.com/dsnopek/bbcom/master/drupal-patches/nodecomment/nodecomment-comment-reply.patch"

; We use a specific version in the 4.x-dev stream
projects[notifications][subdir] = contrib
projects[notifications][type] = module
projects[notifications][download][type] = git
projects[notifications][download][url] = http://git.drupal.org/project/notifications.git
projects[notifications][download][revision] = "bf2889b4a23117151b77189a9c8b4ec2cf8f5a32"

projects[onbeforeunload][subdir] = contrib
projects[onbeforeunload][version] = "1.0"

projects[remember_me][subdir] = contrib
projects[remember_me][version] = "2.1"

projects[rules][subdir] = contrib
projects[rules][version] = "1.3"

projects[scheduler][subdir] = contrib
projects[scheduler][version] = "1.8"

projects[strongarm][subdir] = contrib
projects[strongarm][version] = "2.1"

projects[token][subdir] = contrib
projects[token][version] = "1.18"
projects[token][patch][] = "https://raw.github.com/dsnopek/bbcom/master/drupal-patches/token/node-url-language.patch"

projects[translation_helpers][subdir] = contrib
projects[translation_helpers][version] = "1.0"

projects[userpoints][subdir] = contrib
projects[userpoints][version] = "1.2"

projects[uuid][subdir] = contrib
projects[uuid][version] = "1.0-beta2"
projects[uuid][patch][] = "https://raw.github.com/dsnopek/bbcom/master/drupal-patches/uuid/node_get_by_uuid-1261196_0.patch"

projects[uuid_features][subdir] = contrib
projects[uuid_features][version] = "1.0-alpha1"

projects[views][subdir] = contrib
projects[views][version] = "2.16"

projects[views_bulk_operations][subdir] = contrib
projects[views_bulk_operations][version] = "1.10"
projects[views_bulk_operations][patch][] = "https://raw.github.com/dsnopek/bbcom/master/drupal-patches/views_bulk_operations/ie6.patch"

projects[views_hacks][subdir] = contrib
projects[views_hacks][version] = "1.x-dev"

projects[views_php][subdir] = contrib
projects[views_php][version] = "1.x-dev"

projects[voting_rules][subdir] = contrib
projects[voting_rules][version] = "1.0-alpha1"

projects[votingapi][subdir] = contrib
projects[votingapi][version] = "2.3"

projects[wikitools][subdir] = contrib
projects[wikitools][version] = "1.3"
projects[wikitools][patch][] = "https://raw.github.com/dsnopek/bbcom/master/drupal-patches/wikitools/wikitools-inline.diff"

; TODO: maybe I need a site.make file for the site specific stuff?

; TODO: does this really belong in the install profile?
projects[smtp][subdir] = contrib
projects[smtp][version] = "1.0-beta4"

; TODO: does this really belong in the install profile?
projects[mollom][subdir] = contrib
projects[mollom][version] = "1.15"
projects[mollom][patch][] = "https://raw.github.com/dsnopek/bbcom/master/drupal-patches/mollom/disable-author-ip.patch"

; TODO: does this really belong in the install profile?
projects[backup_migrate][subdir] = contrib
projects[backup_migrate][version] = "2.4"

; TODO: does this really belong in the install profile?
; TODO: this actually requires a very specific version!
projects[backup_migrate_files][subdir] = contrib
projects[backup_migrate_files][version] = "1.x-dev"

; Themes

projects[ninesixty][version] = "1.0"

projects[tao][version] = "3.3"
projects[rubik][version] = "3.0-beta3"

; Libraries

libraries[jquery_ui][download][type] = "get"
libraries[jquery_ui][download][url] = "http://jquery-ui.googlecode.com/files/jquery.ui-1.6.zip"
libraries[jquery_ui][directory_name] = "jquery.ui"
libraries[jquery_ui][destination] = "modules/contrib/jquery_ui"

