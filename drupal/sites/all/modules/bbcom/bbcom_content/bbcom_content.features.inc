<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function bbcom_content_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_imagecache_default_presets().
 */
function bbcom_content_imagecache_default_presets() {
  $items = array(
    'large' => array(
      'presetname' => 'large',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '560',
            'height' => '560',
            'upscale' => 0,
          ),
        ),
      ),
    ),
    'medium' => array(
      'presetname' => 'medium',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '320',
            'height' => '320',
            'upscale' => 0,
          ),
        ),
      ),
    ),
    'small' => array(
      'presetname' => 'small',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '160',
            'height' => '160',
            'upscale' => 0,
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_node_info().
 */
function bbcom_content_node_info() {
  $items = array(
    'content' => array(
      'name' => t('Content'),
      'module' => 'features',
      'description' => t('A piece of content used to study.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Content'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function bbcom_content_views_api() {
  return array(
    'api' => '2',
  );
}
