<?php

/**
 * @file
 *   Drush hack to work with autoload module
 */
  
// We need to override autoload_class() so it works with Drush.
// The prepend parameter is PHP 5.3 so we better unregister autoload_class() too
spl_autoload_register('_messaging_drush_autoload_class');
spl_autoload_unregister('autoload_class');  

/**
 * Autoload hack, so autoload paths work with drush
 */
function _messaging_drush_autoload_class($class) {
  static $lookup;

  if (!isset($lookup)) {
    $lookup = autoload_get_lookup();
  }

  if (!empty($lookup[$class]) && ($path = realpath($lookup[$class]))) {
    require $path;
  }
}

