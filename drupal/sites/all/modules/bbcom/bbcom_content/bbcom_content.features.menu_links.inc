<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function bbcom_content_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: primary-links:content
  $menu_links['primary-links:content'] = array(
    'menu_name' => 'primary-links',
    'link_path' => 'content',
    'router_path' => 'content',
    'link_title' => 'Library',
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
    'weight' => '-48',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Library');


  return $menu_links;
}
