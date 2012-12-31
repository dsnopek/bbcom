<?php

/**
 * Implementation of hook_views_default_views().
 */
function bbcom_content_views_default_views() {
  $views = array();

  // Exported view: content_source_entries
  $view = new view;
  $view->name = 'content_source_entries';
  $view->description = 'Lists source entries which are from a particular text.';
  $view->tag = 'bbcom_content';
  $view->base_table = 'lingwo_korpus_entry';
  $view->core = 6;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'entry_hash' => array(
      'label' => 'dictionary entry',
      'required' => 0,
      'id' => 'entry_hash',
      'table' => 'lingwo_korpus_entry',
      'field' => 'entry_hash',
      'relationship' => 'none',
    ),
    'nid' => array(
      'label' => 'entry node',
      'required' => 0,
      'id' => 'nid',
      'table' => 'lingwo_entry_info',
      'field' => 'nid',
      'relationship' => 'entry_hash',
    ),
    'flag_content_rel' => array(
      'label' => 'verified',
      'required' => 0,
      'flag' => 'verified',
      'user_scope' => 'any',
      'id' => 'flag_content_rel',
      'table' => 'node',
      'field' => 'flag_content_rel',
      'relationship' => 'nid',
    ),
  ));
  $handler->override_option('fields', array(
    'headword' => array(
      'label' => 'Headword',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'headword',
      'table' => 'lingwo_korpus_entry',
      'field' => 'headword',
      'relationship' => 'none',
    ),
    'pos' => array(
      'id' => 'pos',
      'table' => 'lingwo_korpus_entry',
      'field' => 'pos',
    ),
    'timestamp' => array(
      'label' => 'Verified',
      'alter' => array(
        'alter_text' => 0,
        'text' => 'True ([timestamp])',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => 'False',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'date_format' => 'small',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'timestamp',
      'table' => 'flag_content',
      'field' => 'timestamp',
      'relationship' => 'flag_content_rel',
    ),
    'nid' => array(
      'label' => 'Nid',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'exclude' => 1,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'relationship' => 'nid',
    ),
    'language' => array(
      'label' => 'Korpus Language',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 1,
      'id' => 'language',
      'table' => 'lingwo_korpus_entry',
      'field' => 'language',
      'relationship' => 'none',
    ),
    'php' => array(
      'label' => 'Status',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'use_php_setup' => 0,
      'php_setup' => '',
      'php_value' => '',
      'php_output' => '<?php
print empty($row->nid) ? t(\'Missing Source\') : t(\'Full\');
?>',
      'use_php_click_sortable' => '0',
      'php_click_sortable' => '',
      'exclude' => 0,
      'id' => 'php',
      'table' => 'views',
      'field' => 'php',
      'relationship' => 'none',
    ),
    'php_1' => array(
      'label' => 'Edit',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'use_php_setup' => 0,
      'php_setup' => '',
      'php_value' => '',
      'php_output' => '<?php
print empty($row->nid) ?
  l(t(\'add source\'), \'node/add/entry\', array(\'query\' => array(
    \'language\' => $row->language,
    \'pos\' => $row->pos,
    \'headword\' => $row->headword
  ))) :
  l(t(\'edit source\'), \'node/\' . $row->nid . \'/edit\');
?>',
      'use_php_click_sortable' => '0',
      'php_click_sortable' => '',
      'exclude' => 0,
      'id' => 'php_1',
      'table' => 'views',
      'field' => 'php',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'nid' => array(
      'default_action' => 'not found',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => '',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'nid',
      'table' => 'lingwo_korpus_entry',
      'field' => 'nid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        2 => 0,
        3 => 0,
        4 => 0,
        6 => 0,
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_user' => 0,
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'webform' => 0,
        'content' => 0,
        'entry' => 0,
        'story' => 0,
        'page' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        1 => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_node_flag_name' => '*relationship*',
      'validate_argument_node_flag_test' => 'flaggable',
      'validate_argument_node_flag_id_type' => 'id',
      'validate_argument_user_flag_name' => '*relationship*',
      'validate_argument_user_flag_test' => 'flaggable',
      'validate_argument_user_flag_id_type' => 'id',
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'flagged' => array(
      'operator' => '=',
      'value' => 'All',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'operator' => '',
        'identifier' => 'verified',
        'label' => 'Verified',
        'optional' => 1,
        'remember' => 0,
      ),
      'id' => 'flagged',
      'table' => 'flag_content',
      'field' => 'flagged',
      'relationship' => 'flag_content_rel',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'php',
    'php_access' => '$parts = explode(\'/\', $_GET[\'q\']);
$node = node_load($parts[1]);
// only show on content nodes!!
return $node->type == \'content\' && user_access(\'translate content\');
',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('header', 'There are <?php
$view = views_get_current_view();
echo $view->total_rows; ?> entries that match this query.');
  $handler->override_option('header_format', '3');
  $handler->override_option('header_empty', 1);
  $handler->override_option('items_per_page', 20);
  $handler->override_option('use_pager', '1');
  $handler->override_option('distinct', 0);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'headword' => 'headword',
      'pos' => 'pos',
      'timestamp' => 'timestamp',
      'nothing_1' => 'nothing_1',
      'nothing' => 'nothing',
      'nid' => 'nid',
      'language' => 'language',
      'nid_1' => 'nid_1',
    ),
    'info' => array(
      'headword' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'pos' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'timestamp' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'nothing_1' => array(
        'separator' => '',
      ),
      'nothing' => array(
        'separator' => '',
      ),
      'nid' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'language' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'nid_1' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => 'headword',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'node/%/entries/source');
  $handler->override_option('menu', array(
    'type' => 'default tab',
    'title' => 'Source',
    'description' => '',
    'weight' => '1',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'tab',
    'title' => 'Entries',
    'description' => '',
    'weight' => '1',
    'name' => 'navigation',
  ));

  $views[$view->name] = $view;

  // Exported view: content_translation_entries
  $view = new view;
  $view->name = 'content_translation_entries';
  $view->description = 'Lists translation entries which are from a particular text.';
  $view->tag = 'bbcom_content';
  $view->base_table = 'lingwo_korpus_entry';
  $view->core = 6;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'entry_hash' => array(
      'label' => 'dictionary entry',
      'required' => 0,
      'id' => 'entry_hash',
      'table' => 'lingwo_korpus_entry',
      'field' => 'entry_hash',
      'relationship' => 'none',
    ),
    'nid' => array(
      'label' => 'entry node',
      'required' => 0,
      'id' => 'nid',
      'table' => 'lingwo_entry_info',
      'field' => 'nid',
      'relationship' => 'entry_hash',
    ),
    'translation' => array(
      'label' => 'Translations',
      'required' => 0,
      'language' => 'current',
      'id' => 'translation',
      'table' => 'node',
      'field' => 'translation',
      'relationship' => 'nid',
    ),
    'flag_content_rel' => array(
      'label' => 'verified',
      'required' => 0,
      'flag' => 'verified',
      'user_scope' => 'any',
      'id' => 'flag_content_rel',
      'table' => 'node',
      'field' => 'flag_content_rel',
      'relationship' => 'translation',
    ),
  ));
  $handler->override_option('fields', array(
    'headword' => array(
      'label' => 'Headword',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'headword',
      'table' => 'lingwo_korpus_entry',
      'field' => 'headword',
      'relationship' => 'none',
    ),
    'pos' => array(
      'id' => 'pos',
      'table' => 'lingwo_korpus_entry',
      'field' => 'pos',
    ),
    'timestamp' => array(
      'label' => 'Verified',
      'alter' => array(
        'alter_text' => 0,
        'text' => 'True ([timestamp])',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => 'False',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'date_format' => 'small',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'timestamp',
      'table' => 'flag_content',
      'field' => 'timestamp',
      'relationship' => 'flag_content_rel',
    ),
    'nid' => array(
      'label' => 'Nid',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'exclude' => 1,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'relationship' => 'translation',
    ),
    'language' => array(
      'label' => 'Korpus Language',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 1,
      'id' => 'language',
      'table' => 'lingwo_korpus_entry',
      'field' => 'language',
      'relationship' => 'none',
    ),
    'nid_1' => array(
      'label' => 'Nid',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'exclude' => 1,
      'id' => 'nid_1',
      'table' => 'node',
      'field' => 'nid',
      'relationship' => 'nid',
    ),
    'php' => array(
      'label' => 'Status',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'use_php_setup' => 0,
      'php_setup' => '',
      'php_value' => '',
      'php_output' => '<?php
global $language;

if (!empty($row->nid)) {
  // we have everything!
  print t(\'Full\');
}
elseif (!empty($row->nid_1)) {
  if ($row->language == $language->language) {
    // we don\'t have everything, but since the korpus language matches the users current
    // language, from her perspective, there is nothing more we car ask for
    print t(\'Partial\');
  }
  else {
    print t(\'Missing Translation\');
  }
}
else {
  print t(\'Missing Source\');
}
?>',
      'use_php_click_sortable' => '0',
      'php_click_sortable' => '',
      'exclude' => 0,
      'id' => 'php',
      'table' => 'views',
      'field' => 'php',
      'relationship' => 'none',
    ),
    'php_1' => array(
      'label' => 'Edit',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'use_php_setup' => 0,
      'php_setup' => '',
      'php_value' => '',
      'php_output' => '<?php
global $language;

if (!empty($row->nid)) {
  // edit the fully existing node (might be source, might be translation)
  print l($row->language == $language->language ?
    t(\'edit source\') : t(\'edit translation\'),
    \'node/\'. $row->nid .\'/edit\'
  );
}
elseif (!empty($row->nid_1)) {
  if ($row->language == $language->language) {
    // edit the source when we are using the language of the korpus (translation is not appropriate)
    print l(t(\'edit source\'), \'node/\'. $row->nid_1 .\'/edit\');
  }
  else {
    // add the translation when we\'ve got a node, but no translation
    print l(t(\'add translation\'), \'node/add/entry\', array(\'query\' => array(
      \'language\'    => $language->language,
      \'pos\'         => $row->pos,
      \'headword\'    => $row->headword,
      \'translation\' => $row->nid_1,
    )));
  }
}

else {
  // if we\'ve got nothing, then add the source
  print l(t(\'add source\'), \'node/add/entry\', array(\'query\' => array(
    \'language\' => $row->language,
    \'pos\'      => $row->pos,
    \'headword\' => $row->headword,
  )));
}
?>',
      'use_php_click_sortable' => '0',
      'php_click_sortable' => '',
      'exclude' => 0,
      'id' => 'php_1',
      'table' => 'views',
      'field' => 'php',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'nid' => array(
      'default_action' => 'not found',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => '',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'nid',
      'table' => 'lingwo_korpus_entry',
      'field' => 'nid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        2 => 0,
        3 => 0,
        4 => 0,
        6 => 0,
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_user' => 0,
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'webform' => 0,
        'content' => 0,
        'entry' => 0,
        'story' => 0,
        'page' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        1 => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_node_flag_name' => '*relationship*',
      'validate_argument_node_flag_test' => 'flaggable',
      'validate_argument_node_flag_id_type' => 'id',
      'validate_argument_user_flag_name' => '*relationship*',
      'validate_argument_user_flag_test' => 'flaggable',
      'validate_argument_user_flag_id_type' => 'id',
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'flagged' => array(
      'operator' => '=',
      'value' => 'All',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'operator' => '',
        'identifier' => 'verified',
        'label' => 'Verified',
        'optional' => 1,
        'remember' => 0,
      ),
      'id' => 'flagged',
      'table' => 'flag_content',
      'field' => 'flagged',
      'relationship' => 'flag_content_rel',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'php',
    'php_access' => '$parts = explode(\'/\', $_GET[\'q\']);
$node = node_load($parts[1]);
// only show on content nodes!!
return $node->type == \'content\' && user_access(\'translate content\');',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('header', 'There are <?php
$view = views_get_current_view();
echo $view->total_rows; ?> entries that match this query.');
  $handler->override_option('header_format', '3');
  $handler->override_option('header_empty', 1);
  $handler->override_option('items_per_page', 20);
  $handler->override_option('use_pager', '1');
  $handler->override_option('distinct', 0);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'headword' => 'headword',
      'pos' => 'pos',
      'timestamp' => 'timestamp',
      'nothing_1' => 'nothing_1',
      'nothing' => 'nothing',
      'nid' => 'nid',
      'language' => 'language',
      'nid_1' => 'nid_1',
    ),
    'info' => array(
      'headword' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'pos' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'timestamp' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'nothing_1' => array(
        'separator' => '',
      ),
      'nothing' => array(
        'separator' => '',
      ),
      'nid' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'language' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'nid_1' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => 'headword',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'node/%/entries/translation');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'Translation',
    'description' => '',
    'weight' => '1',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'tab',
    'title' => 'Translation',
    'description' => '',
    'weight' => '1',
    'name' => 'navigation',
  ));

  $views[$view->name] = $view;

  // Exported view: recent_content
  $view = new view;
  $view->name = 'recent_content';
  $view->description = 'Shows recent content.';
  $view->tag = '';
  $view->base_table = 'node';
  $view->core = 0;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'flag_content_rel' => array(
      'label' => 'promoted',
      'required' => 0,
      'flag' => 'promoted',
      'user_scope' => 'any',
      'id' => 'flag_content_rel',
      'table' => 'node',
      'field' => 'flag_content_rel',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'sticky' => array(
      'id' => 'sticky',
      'table' => 'node',
      'field' => 'sticky',
      'order' => 'DESC',
    ),
    'created' => array(
      'id' => 'created',
      'table' => 'node',
      'field' => 'created',
      'order' => 'DESC',
      'relationship' => 'none',
      'granularity' => 'second',
    ),
  ));
  $handler->override_option('filters', array(
    'promote' => array(
      'id' => 'promote',
      'table' => 'node',
      'field' => 'promote',
      'operator' => '=',
      'value' => '1',
      'group' => 0,
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
    ),
    'status' => array(
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'operator' => '=',
      'value' => '1',
      'group' => 0,
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'content' => 'content',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'language' => array(
      'operator' => 'in',
      'value' => array(
        'en' => 'en',
        'pl' => 'pl',
      ),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'language_op',
        'identifier' => 'language',
        'label' => 'Language',
        'bef_filter_description' => '',
        'bef_format' => 'bef',
        'optional' => 1,
        'single' => 0,
        'remember' => 0,
        'reduce' => 1,
        'bef_select_all_none' => 0,
        'bef_collapsible' => 0,
      ),
      'id' => 'language',
      'table' => 'node',
      'field' => 'language',
      'relationship' => 'none',
    ),
    'field_content_level_value_many_to_one' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'field_content_level_value_many_to_one_op',
        'identifier' => 'level',
        'label' => 'Level',
        'bef_filter_description' => '',
        'bef_format' => 'bef',
        'optional' => 1,
        'single' => 0,
        'remember' => 0,
        'reduce' => 0,
        'bef_select_all_none' => 0,
        'bef_collapsible' => 0,
      ),
      'id' => 'field_content_level_value_many_to_one',
      'table' => 'node_data_field_content_level',
      'field' => 'field_content_level_value_many_to_one',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
      'reduce_duplicates' => 0,
    ),
    'field_content_type_value_many_to_one' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'field_content_type_value_many_to_one_op',
        'identifier' => 'type',
        'label' => 'Type',
        'bef_filter_description' => '',
        'bef_format' => 'bef',
        'optional' => 1,
        'single' => 0,
        'remember' => 0,
        'reduce' => 0,
        'bef_select_all_none' => 0,
        'bef_collapsible' => 0,
      ),
      'id' => 'field_content_type_value_many_to_one',
      'table' => 'node_data_field_content_type',
      'field' => 'field_content_type_value_many_to_one',
      'relationship' => 'none',
      'reduce_duplicates' => 0,
    ),
    'field_audio_list' => array(
      'operator' => '=',
      'value' => 'All',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'operator' => '',
        'identifier' => 'has_audio',
        'label' => 'Has audio',
        'bef_filter_description' => '',
        'bef_format' => 'default',
        'optional' => 1,
        'remember' => 0,
        'bef_select_all_none' => 0,
        'bef_collapsible' => 0,
      ),
      'id' => 'field_audio_list',
      'table' => 'node_data_field_audio',
      'field' => 'field_audio_list',
      'relationship' => 'none',
    ),
    'flagged' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => '',
        'label' => 'Promoted',
        'identifier' => 'promoted',
        'bef_filter_description' => '',
        'bef_format' => 'default',
        'optional' => 1,
        'remember' => 0,
        'bef_select_all_none' => 0,
        'bef_collapsible' => 0,
      ),
      'id' => 'flagged',
      'table' => 'flag_content',
      'field' => 'flagged',
      'relationship' => 'flag_content_rel',
    ),
    'field_content_category_value_many_to_one' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'field_content_category_value_many_to_one_op',
        'identifier' => 'category',
        'label' => 'Category',
        'bef_filter_description' => '',
        'bef_format' => 'bef',
        'optional' => 1,
        'single' => 0,
        'remember' => 0,
        'reduce' => 0,
        'bef_select_all_none' => 0,
        'bef_collapsible' => 1,
      ),
      'id' => 'field_content_category_value_many_to_one',
      'table' => 'node_data_field_content_category',
      'field' => 'field_content_category_value_many_to_one',
      'relationship' => 'none',
      'reduce_duplicates' => 1,
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'access content',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Library');
  $handler->override_option('header_format', '1');
  $handler->override_option('footer_format', '1');
  $handler->override_option('empty', '<p>No texts found! <a href="/content">Click here</a> to return to the full list...</p>');
  $handler->override_option('empty_format', '1');
  $handler->override_option('use_pager', '1');
  $handler->override_option('row_plugin', 'node');
  $handler->override_option('row_options', array(
    'teaser' => 1,
    'links' => 1,
  ));
  $handler->override_option('exposed_block', TRUE);
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->override_option('path', 'content/all');
  $handler->override_option('menu', array(
    'type' => 'default tab',
    'title' => 'All',
    'description' => '',
    'weight' => '-1',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'normal',
    'title' => 'Library',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler = $view->new_display('feed', 'Feed', 'feed');
  $handler->override_option('title', 'Front page feed');
  $handler->override_option('style_plugin', 'rss');
  $handler->override_option('style_options', array(
    'mission_description' => 1,
    'description' => '',
  ));
  $handler->override_option('row_plugin', 'node_rss');
  $handler->override_option('row_options', array(
    'item_length' => 'default',
  ));
  $handler->override_option('path', 'rss.xml');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('displays', array(
    'default' => 'default',
    'page' => 'page',
  ));
  $handler->override_option('sitename_title', '1');
  $handler = $view->new_display('page', 'My texts', 'page_1');
  $handler->override_option('relationships', array());
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'content' => 'content',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'language' => array(
      'operator' => 'in',
      'value' => array(
        'en' => 'en',
        'pl' => 'pl',
      ),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'language_op',
        'identifier' => 'language',
        'label' => 'Language',
        'bef_filter_description' => '',
        'bef_format' => 'bef',
        'optional' => 1,
        'single' => 0,
        'remember' => 0,
        'reduce' => 1,
        'bef_select_all_none' => 0,
        'bef_collapsible' => 0,
      ),
      'id' => 'language',
      'table' => 'node',
      'field' => 'language',
      'relationship' => 'none',
    ),
    'field_content_level_value_many_to_one' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'field_content_level_value_many_to_one_op',
        'identifier' => 'level',
        'label' => 'Level',
        'bef_filter_description' => '',
        'bef_format' => 'bef',
        'optional' => 1,
        'single' => 0,
        'remember' => 0,
        'reduce' => 0,
        'bef_select_all_none' => 0,
        'bef_collapsible' => 0,
      ),
      'id' => 'field_content_level_value_many_to_one',
      'table' => 'node_data_field_content_level',
      'field' => 'field_content_level_value_many_to_one',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
      'reduce_duplicates' => 0,
    ),
    'field_content_type_value_many_to_one' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'field_content_type_value_many_to_one_op',
        'identifier' => 'type',
        'label' => 'Type',
        'bef_filter_description' => '',
        'bef_format' => 'bef',
        'optional' => 1,
        'single' => 0,
        'remember' => 0,
        'reduce' => 0,
        'bef_select_all_none' => 0,
        'bef_collapsible' => 0,
      ),
      'id' => 'field_content_type_value_many_to_one',
      'table' => 'node_data_field_content_type',
      'field' => 'field_content_type_value_many_to_one',
      'relationship' => 'none',
      'reduce_duplicates' => 0,
    ),
    'field_audio_list' => array(
      'operator' => '=',
      'value' => 'All',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'operator' => '',
        'identifier' => 'has_audio',
        'label' => 'Has audio',
        'bef_filter_description' => '',
        'bef_format' => 'default',
        'optional' => 1,
        'remember' => 0,
        'bef_select_all_none' => 0,
        'bef_collapsible' => 0,
      ),
      'id' => 'field_audio_list',
      'table' => 'node_data_field_audio',
      'field' => 'field_audio_list',
      'relationship' => 'none',
    ),
    'field_content_category_value_many_to_one' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'field_content_category_value_many_to_one_op',
        'identifier' => 'category',
        'label' => 'Category',
        'bef_filter_description' => '',
        'bef_format' => 'bef',
        'optional' => 1,
        'single' => 0,
        'remember' => 0,
        'reduce' => 0,
        'bef_select_all_none' => 0,
        'bef_collapsible' => 1,
      ),
      'id' => 'field_content_category_value_many_to_one',
      'table' => 'node_data_field_content_category',
      'field' => 'field_content_category_value_many_to_one',
      'relationship' => 'none',
      'reduce_duplicates' => 1,
    ),
    'uid_current' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'uid_current',
      'table' => 'users',
      'field' => 'uid_current',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'create content content',
  ));
  $handler->override_option('path', 'content/my');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'My texts',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));

  $views[$view->name] = $view;

  // Exported view: search_content
  $view = new view;
  $view->name = 'search_content';
  $view->description = 'Allows searching content items.';
  $view->tag = '';
  $view->base_table = 'node';
  $view->core = 0;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('sorts', array(
    'score' => array(
      'order' => 'DESC',
      'id' => 'score',
      'table' => 'search_index',
      'field' => 'score',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'created' => array(
      'order' => 'DESC',
      'granularity' => 'second',
      'id' => 'created',
      'table' => 'node',
      'field' => 'created',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'content' => 'content',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'status' => array(
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'operator' => '=',
      'value' => '1',
      'group' => 0,
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
    ),
    'keys' => array(
      'operator' => 'required',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'keys_op',
        'identifier' => 'keywords',
        'label' => '',
        'optional' => 1,
        'remember' => 0,
      ),
      'id' => 'keys',
      'table' => 'search_index',
      'field' => 'keys',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'access content',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Content');
  $handler->override_option('header_format', '1');
  $handler->override_option('footer_format', '1');
  $handler->override_option('empty', 'No results found.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('use_pager', '1');
  $handler->override_option('row_plugin', 'node');
  $handler->override_option('row_options', array(
    'teaser' => 1,
    'links' => 1,
  ));
  $handler->override_option('exposed_block', TRUE);
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->override_option('path', 'content/search');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));

  $views[$view->name] = $view;

  return $views;
}
