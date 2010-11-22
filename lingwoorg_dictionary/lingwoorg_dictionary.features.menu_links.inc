<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function lingwoorg_dictionary_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: primary-links:dictionary
  $menu_links['primary-links:dictionary'] = array(
    'menu_name' => 'primary-links',
    'link_path' => 'dictionary',
    'router_path' => 'dictionary',
    'link_title' => 'Dictionary',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
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
  t('Dictionary');


  return $menu_links;
}
