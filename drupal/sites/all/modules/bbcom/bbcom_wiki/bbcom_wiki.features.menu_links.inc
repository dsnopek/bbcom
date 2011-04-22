<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function bbcom_wiki_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: primary-links:wiki
  $menu_links['primary-links:wiki'] = array(
    'menu_name' => 'primary-links',
    'link_path' => 'wiki',
    'router_path' => 'wiki',
    'link_title' => 'Wiki',
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
    'weight' => '-45',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Wiki');


  return $menu_links;
}
