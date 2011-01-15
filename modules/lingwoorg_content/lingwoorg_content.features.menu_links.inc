<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function lingwoorg_content_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: primary-links:content
  $menu_links['primary-links:content'] = array(
    'menu_name' => 'primary-links',
    'link_path' => 'content',
    'router_path' => 'content',
    'link_title' => 'Content',
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
    'weight' => '0',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Content');


  return $menu_links;
}
