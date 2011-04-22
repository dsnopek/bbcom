<?php
// $Id: views_access_php.views.inc,v 1.1.2.1 2010/05/22 13:50:26 kratib Exp $

/**
 * Implementation of hook_views_plugins().
 */
function views_access_php_views_plugins() {
  return array(
    'access' => array(
      'php' => array(
        'title' => t('PHP code'),
        'help' => t('Access will be granted if PHP code returns TRUE.'),
        'handler' => 'views_plugin_access_php',
        'path' => drupal_get_path('module', 'views_access_php'),
        'uses options' => TRUE,
      ),
    ),
  );
}

