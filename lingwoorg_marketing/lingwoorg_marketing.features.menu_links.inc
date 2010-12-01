<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function lingwoorg_marketing_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: primary-links:about
  $menu_links['primary-links:about'] = array(
    'menu_name' => 'primary-links',
    'link_path' => 'about',
    'router_path' => 'about',
    'link_title' => 'About',
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
    'weight' => '-1',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('About');


  return $menu_links;
}
