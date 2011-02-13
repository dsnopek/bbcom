<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function bbcom_forum_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: primary-links:forum
  $menu_links['primary-links:forum'] = array(
    'menu_name' => 'primary-links',
    'link_path' => 'forum',
    'router_path' => 'forum',
    'link_title' => 'Forum',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '5',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Forum');


  return $menu_links;
}
