core = 6.x
api = 2

; Drush makefile to create the patched version of Pressflow that we depend on!

; Drupal core
projects[pressflow][type] = core
projects[pressflow][download][type] = git
projects[pressflow][download][url] = https://github.com/pressflow/6.git 
projects[pressflow][patch][] = "https://raw.github.com/dsnopek/bbcom/master/drupal-patches/core/l-hack.patch"
projects[pressflow][patch][] = "https://raw.github.com/dsnopek/bbcom/master/drupal-patches/core/language-hacks.patch"

