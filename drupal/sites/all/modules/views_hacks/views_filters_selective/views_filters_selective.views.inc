<?php
// $Id: views_filters_selective.views.inc,v 1.1.2.1 2010/05/22 13:50:26 kratib Exp $

/**
 * Implementation of hook_views_data_alter().
 */
function views_filters_selective_views_data_alter(&$data) {
  $data['views']['selective'] = array(
    'title' => t('Selective helper'),
    'help' => t('A dummy field to help out with selective filtering.'),
    'field' => array(
      'handler' => 'views_handler_field_filters_selective',
    ),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function views_filters_selective_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'views_filters_selective'),
    ),
    'handlers' => array(
      'views_handler_field_filters_selective' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

