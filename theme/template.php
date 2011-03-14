<?php
// $Id: template.php,v 1.21 2009/08/12 04:25:15 johnalbin Exp $

/**
 * @file
 * Contains theme override functions and preprocess functions for the theme.
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 *   The template.php file is one of the most useful files when creating or
 *   modifying Drupal themes. You can add new regions for block content, modify
 *   or override Drupal's theme functions, intercept or make additional
 *   variables available to your theme, and create custom PHP logic. For more
 *   information, please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/theme-guide
 *
 * OVERRIDING THEME FUNCTIONS
 *
 *   The Drupal theme system uses special theme functions to generate HTML
 *   output automatically. Often we wish to customize this HTML output. To do
 *   this, we have to override the theme function. You have to first find the
 *   theme function that generates the output, and then "catch" it and modify it
 *   here. The easiest way to do it is to copy the original function in its
 *   entirety and paste it here, changing the prefix from theme_ to STARTERKIT_.
 *   For example:
 *
 *     original: theme_breadcrumb()
 *     theme override: STARTERKIT_breadcrumb()
 *
 *   where STARTERKIT is the name of your sub-theme. For example, the
 *   zen_classic theme would define a zen_classic_breadcrumb() function.
 *
 *   If you would like to override any of the theme functions used in Zen core,
 *   you should first look at how Zen core implements those functions:
 *     theme_breadcrumbs()      in zen/template.php
 *     theme_menu_item_link()   in zen/template.php
 *     theme_menu_local_tasks() in zen/template.php
 *
 *   For more information, please visit the Theme Developer's Guide on
 *   Drupal.org: http://drupal.org/node/173880
 *
 * CREATE OR MODIFY VARIABLES FOR YOUR THEME
 *
 *   Each tpl.php template file has several variables which hold various pieces
 *   of content. You can modify those variables (or add new ones) before they
 *   are used in the template files by using preprocess functions.
 *
 *   This makes THEME_preprocess_HOOK() functions the most powerful functions
 *   available to themers.
 *
 *   It works by having one preprocess function for each template file or its
 *   derivatives (called template suggestions). For example:
 *     THEME_preprocess_page    alters the variables for page.tpl.php
 *     THEME_preprocess_node    alters the variables for node.tpl.php or
 *                              for node-forum.tpl.php
 *     THEME_preprocess_comment alters the variables for comment.tpl.php
 *     THEME_preprocess_block   alters the variables for block.tpl.php
 *
 *   For more information on preprocess functions and template suggestions,
 *   please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/node/223440
 *   and http://drupal.org/node/190815#template-suggestions
 */


/**
 * Implementation of HOOK_theme().
 */
function bbcom_theme_theme(&$existing, $type, $theme, $path) {
  return array(
    'lt_username_title' => array(
      'arguments' => array('form_id'),
    ),
    'lt_username_description' => array(
      'arguments' => array('form_id'),
    ),
    'lt_password_description' => array(
      'arguments' => array('form_id'),
    ),
    'bbcom_join_btn' => array(
      'template' => 'bbcom-join-btn',
      'path' => drupal_get_path('theme', 'bbcom_theme') .'/templates',
      'arguments' => array(
        'url' => NULL,
        'label' => NULL,
      ),
    ),
    'bbcom_account_links' => array(
      'arguments' => array('items' => NULL),
      'template' => 'bbcom-account-links',
      'path' => drupal_get_path('theme', 'bbcom_theme') .'/templates',
    ),
    'bbcom_language_flag' => array(
      'arguments' => array('lang' => NULL),
    ),
    'lingwo_entry_search_form' => array(
      'arguments' => array('form' => NULL),
      'template' => 'lingwo-entry-search-form',
      'path' => drupal_get_path('theme', 'bbcom_theme') .'/templates',
    ),
  );
}

/*
 * Customize Login form using the theme hooks provided by logintoboggan.
 */

function bbcom_theme_lt_username_title($form_id) {
  return t('Username or e-mail');
}

function bbcom_theme_lt_username_description($form_id) {
  return '';
}

function bbcom_theme_lt_password_description($form_id) {
  return '';
}

function bbcom_theme_language_switcher_form(&$form_state) {
  global $language;

  $path = drupal_is_front_page() ? '<front>' : $_GET['q'];
  $languages = language_list('enabled');
  $options = array();
  foreach ($languages[1] as $lang_item) {
    if ($language->language == $lang_item->language) {
      $url = '';
    }
    else {
      $url = check_url(url($path, array('language' => $lang_item, 'absolute' => TRUE)));
    }
    $options[$url] = $lang_item->native;
  }

  $form['my_language_select'] = array(
    '#type' => 'select',
    '#title' => t('My Language'),
    '#options' => $options,
    '#default_value' => '',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Change'),
  );

  return $form;
}

function bbcom_theme_language_switcher_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['my_language_select'])) {
    drupal_goto($form_state['values']['my_language_select']);
  }
}

/*
 * Our language flags
 */
function bbcom_theme_bbcom_language_flag($lang, $content=NULL, $notext=NULL) {
  if ($lang == 'en') {
    $country = 'gb';
  }
  else {
    $country = $lang;
  }
  $src = url(drupal_get_path('theme', 'bbcom_theme') .'/images/flags/'. $country .'.png', array('absolute' => TRUE, 'language' => ''));
  $languages = language_list();
  return '<img class="flag" src="'. $src .'" alt="'. $languages[$lang]->name .'" />';
}

/**
 * Override or insert variables into all templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered (name of the .tpl.php file.)
 */
/* -- Delete this line if you want to use this function
function bbcom_theme_preprocess(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
function bbcom_theme_preprocess_page(&$vars, $hook) {
  global $user;

  $vars['always_right'] = TRUE;
  $vars['inner_title'] = FALSE;

  if (isset($vars['node'])) {
    $func = 'bbcom_theme_preprocess_node_'. $vars['node']->type;
    if (function_exists($func)) {
      $func($vars, $hook);
    }
  }

  $vars['language_switcher'] = drupal_get_form('bbcom_theme_language_switcher_form');

  if ($user->uid) {
    $vars['account_links'] = theme('bbcom_account_links', array(
      theme('bbcom_join_btn', url("wial/{$user->uid}/edit"), t('Words I Am Learning')),
      l(t('My Account'), 'user/'. $user->uid),
      l(t('Sign out'), 'logout')
    ));
  }
  else {
    $login_options = array();
    if ($_GET['q'] != 'user/login') {
      $login_options['query']['destination'] = $_GET['q'];
    }
    $vars['account_links'] = theme('bbcom_account_links', array(
      theme('bbcom_join_btn', url('user/register'), t('JOIN')),
      l(t('Sign in'), 'user/login', $login_options)
    ));
  }

  // Remove the right sidebar when on the marketing pages, or in the forum.
  //if ($_GET['q'] == 'about' || context_get('bbcom', 'section') == 'forum') {
  if (context_get('bbcom', 'section') == 'forum') {
    $vars['always_right'] = FALSE;
  }
}

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
function bbcom_theme_preprocess_node(&$vars, $hook) {
  $func = 'bbcom_theme_preprocess_node_'. $vars['node']->type;
  if (function_exists($func)) {
    $func($vars, $hook);
  }
}

function bbcom_theme_preprocess_node_entry(&$vars, $hook) {
  $node = $vars['node'];
  $entry = LingwoEntry::fromNode($node);

  $pos = $entry->getPos(TRUE);
  $vars['pos'] = $pos;

  $vars['title'] .= ' ('. $pos .')';

  if (!$vars['teaser']) {
    $languages = language_list();
    $image_spec = theme('bbcom_language_flag', $node->language);
    $text_spec = $languages[$node->language]->name;

    $source_node = $entry->getTranslationSource();
    if ($source_node) {
      $image_spec = theme('bbcom_language_flag', $source_node->language) .
                    '<img class="arrow-right" alt=" -&gt; " src="'. url(drupal_get_path('theme', 'bbcom_theme') .'/images/arrow_right.gif', array('absolute' => TRUE, 'language' => TRUE)) .'" />' .
                    $image_spec;
      $text_spec = $languages[$source_node->language]->name .' -&gt; '. $text_spec;
    }
  
    $vars['lang_spec'] = '<span class="langspec deem">'. $image_spec .'</span> ';

    //$vars['title'] = '<span class="langspec deem">'. $image_spec .'</span> '. $vars['title'];// .' ('. $pos .')';
    $vars['head_title'] = '['. $text_spec .'] '. $vars['head_title'];
    //$vars['inner_title'] = TRUE;
  }
}

function bbcom_theme_preprocess_node_webform(&$vars, $hook) {
  $vars['inner_title'] = TRUE;
  unset($vars['submitted']);
}

function bbcom_theme_preprocess_node_content(&$vars, $hook) {
  $node = $vars['node'];

  $languages = language_list();

  $vars['lang_spec'] = '<span class="langspec deem">'. theme('bbcom_language_flag', $node->language) .'</span> ';
  //$vars['title'] = '<span class="langspec deem">'. $vars['lang_spec'] .'</span> '. $vars['title'];
  $vars['head_title'] = '['. $languages[$node->language]->name .'] '. $vars['head_title'];

  if (!$vars['teaser']) {
    $vars['inner_title'] = TRUE;
  }
}

function bbcom_theme_preprocess_node_page(&$vars, $hook) {
  // disable our default, of always including space for a right sidebar
  $vars['always_right'] = FALSE;
  // put the title on the inside
  $vars['inner_title'] = TRUE;
}

/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function bbcom_theme_preprocess_comment(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
function bbcom_theme_preprocess_block(&$vars, $hook) {
  $block = $vars['block'];
  if ($block->module == 'lingwo_entry' && $block->delta == 0) {
    unset($block->subject);
  }
}

/*
 * Mark some of the node form elements as inline
 */
function bbcom_theme_node_form($form) {
  if ($form['#node']->type == LingwoEntry::$settings->content_type) {
    $form['title']['#inline'] = TRUE;
    $form['language']['#inline'] = TRUE;
    $form['pos']['#inline'] = TRUE;
  }

  return theme_node_form($form);
}

/*
 * By default, mark all the lingwo_fields widget elements as inline.
 */
function bbcom_theme_preprocess_lingwo_fields_widget_form(&$variables) {
  foreach (element_children($variables['element']) as $name) {
    $variables['element'][$name]['value']['#inline'] = TRUE;
  }

  // let the template know that its running in widget mode
  $variables['widget'] = TRUE;
  $variables['view'] = FALSE;
}

/*
 * Let the view template know that its operating in view mode.
 */
function bbcom_theme_preprocess_lingwo_fields_view(&$variables) {
  $variables['widget'] = FALSE;
  $variables['view'] = TRUE;
}

/*
 * Change the way forms generate.
 */
function bbcom_theme_form_element($element, $value) {
  // This is also used in the installer, pre-database setup.
  $t = get_t();

  // DRS: my changes here
  $output = '<div class="form-item '.
    ($element['#inline'] ? 'inline-form-item' : 'block-form-item') .'"';

  // DRS: Pretty much original from here on in
  if (!empty($element['#id'])) {
    $output .= ' id="'. $element['#id'] .'-wrapper"';
  }
  $output .= ">\n";
  $required = !empty($element['#required']) ? '<span class="form-required" title="'. $t('This field is required.') .'">*</span>' : '';

  if (!empty($element['#title'])) {
    $title = $element['#title'];
    if (!empty($element['#id'])) {
      $output .= ' <label for="'. $element['#id'] .'">'. $t('!title: !required', array('!title' => filter_xss_admin($title), '!required' => $required)) ."</label>\n";
    }
    else {
      $output .= ' <label>'. $t('!title: !required', array('!title' => filter_xss_admin($title), '!required' => $required)) ."</label>\n";
    }
  }

  $output .= " $value\n";

  if (!empty($element['#description'])) {
    $output .= ' <div class="description">'. $element['#description'] ."</div>\n";
  }

  $output .= "</div>\n";

  return $output;
}

/*
 * Helper function for using in tpl.php files for rendering form elements without titles.
 */
function _bbcom_unset_title(&$element) {
  unset($element['#title']);
  foreach (element_children($element) as $name) {
    _bbcom_unset_title($element[$name]);
  }
}
function bbcom_render_no_title(&$element) {
  #_bbcom_unset_title($element);
  unset($element['value']['#title']);
  return drupal_render($element);
}

