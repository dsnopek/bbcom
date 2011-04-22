<?php
/**
 * @file
 * Messaging Framework - Admin UI
 */

/**
 * Build template description for admin pages
 */
function messaging_template_admin_info($template) {
  drupal_add_css(drupal_get_path('module', 'messaging_template') . '/messaging_template.css');
  $info = messaging_template_message_group($template);
  $output = '<div id="messaging-template-info" class="clear-block">';
  $output .= '<div id="template-tree">';
  $output .= messaging_template_format_tree(messaging_template_get_branch($template));
  $output .= '</div>';
  $output .= '<div id="template-description">';
  $description = t('These are the message parts for %group.', array('%group' => $info['name']));
  if (!empty($info['description'])) {
    $description .= ' ' . $info['description'];
  }
  $output .= '<p>' . $description . '</p>';
  if (!empty($info['help'])) {
    $output .= '<p>' . $info['help'] . '</p>';
  }
  if (!empty($info['fallback'])) {
    $output .= '<p>' . t('The fallback template from which message parts will be taken if left blank is <a href="@template_edit">%template_name</a>', array('@template_edit' => url('admin/messaging/template/edit/' . $info['fallback']), '%template_name' => messaging_template_message_group($info['fallback'], 'name'))) . '</p>';
  }
  $output .= '</div></div>';

  return $output;
}

/*
 * Overview of message parts and sending methods
 */
function messaging_template_admin_template() {
  $output = '';

  // Now build table which will have some indentation
  $output .= messaging_template_format_table(messaging_template_get_tree());

  // List sending methods
  $rows = array();
  messaging_method_list();
  foreach (messaging_method_info() as $method => $info) {
    $rows[] = array(
      '<strong>'. $info['title'] .'</strong>',
      !empty($info['description']) ? check_plain($info['description']) : ''
    );
  }
  $output .= theme('box', t('Sending methods'), theme('table', NULL, $rows));
  return $output;
}

/**
 * Format template tree as table rows
 */
function messaging_template_format_table($tree, $depth = 0) {
  $rows = array();
  foreach ($tree as $key => $template) {
    $options['attributes'] = !empty($template['description']) ? array('title' => $template['description']) : array();
    $rows[] = array(
      theme('indentation', $depth) . l($template['name'], 'admin/messaging/template/edit/'.$key, $options),
      ($parts = module_invoke_all('messaging', 'message keys', $key)) ? implode(', ', $parts) : '',
    );
    if (!empty($template['children'])) {
      $rows = array_merge($rows, messaging_template_format_table($template['children'], $depth + 1));
    }
  }
  if ($depth) {
    return $rows;
  }
  else {
    $header = array(t('Message groups'), t('Parts')); 
    return theme('table', $header, $rows);    
  }
}

/**
 * Format template tree small tree
 */
function messaging_template_format_tree($tree, $depth = 0) {
  $items = array();
  foreach ($tree as $key => $template) {
    $item = array();
    $options['attributes'] = !empty($template['description']) ? array('title' => $template['description']) : array();
    $item['data'] = l($template['name'], 'admin/messaging/template/edit/'.$key, $options);
    if (!empty($template['children'])) {
      $item['children'] = messaging_template_format_tree($template['children'], $depth + 1);
    }
    $items[] = $item;
  }
  return $depth ? $items : theme('item_list', $items);
}

/**
 * Build fallback tree
 */
function messaging_template_get_tree() {
  // List message groups from all modules
  $templates = module_invoke_all('messaging', 'message groups');
  $tree = array();
  foreach ($templates as $key => &$template) {
    if (empty($template['fallback'])) {
      $tree[$key] = &$template;
    }
    else {
      $templates[$template['fallback']]['children'][$key] = &$template;
    }
  }
  return $tree;
}

/**
 * Build fallback sub-tree (only parents and children of given template)
 */
function messaging_template_get_branch($find, $tree = NULL) {
  $tree = $tree ? $tree : messaging_template_get_tree();
  if ($find && isset($tree[$find])) {
    $template = $tree[$find];
    if (!empty($template['children'])) {
      $template['children'] = messaging_template_get_branch(NULL, $template['children']);
    }
    return array($find => $template);
  }
  // Not in this set, explore children
  foreach ($tree as $key => &$template) {
    if (!$find) {
      // Already found, we are just removing sub-chilren
      $template['children'] = NULL;
    }
    elseif (!empty($template['children']) && ($subtree = messaging_template_get_branch($find, $template['children']))) {
      $template['children'] = $subtree;
      return array($key => $template);
    }
  }
  return $tree;
}

/**
 * Message groups edit page
 */
function messaging_template_admin_template_edit($group, $langcode = NULL) {
  $breadcrumb[] = l(t('Home'), '<front>');
  $breadcrumb[] = l(t('Message templates'), 'admin/messaging/template');
  drupal_set_breadcrumb($breadcrumb);
  $language_list = language_list();
  $language = $language_list[$langcode];
  $output = '';
  if ($group && $language) {
    drupal_set_title(t('Template for %name', array('%name' => $group['name'])));
    // Build navigation tree
    $output .= drupal_get_form('messaging_template_admin_message_form', $group, $language);
  }
  return $output;
}

/**
 * Edit message formats
 */
function messaging_template_admin_message_form($form_state, $group_info, $language) {
  $group = $group_info['group'];
  $form['group'] = array('#type' => 'value', '#value' => $group);
  $form['language'] = array('#type' => 'value', '#value' => $language);
  $form['module'] = array('#type' => 'value', '#value' => isset($group_info['module']) ? $group_info['module'] : '');
  $keylist = module_invoke_all('messaging', 'message keys', $group, $language);
  $send_methods = array('default' => t('Default'));
  $send_methods += messaging_method_list();
  $form['messages'] = array('#tree' => TRUE);
  // Get list of allowed filter formats for this user
  $formats_all = _messaging_template_filter_formats(TRUE);
  $formats_allowed = _messaging_template_filter_formats(FALSE);
  foreach ($send_methods as $method => $methodname) {
    // Get all texts for this group/method/language
    $texts = messaging_template_message_part($group, NULL, $method, $language, FALSE);
    $form['messages'][$method] = array(
      '#type' => 'fieldset',
      '#title' => $methodname,
      '#collapsible' => TRUE,
      '#collapsed' => $texts ? FALSE : TRUE,
      //'#theme' => 'messaging_template_admin_texts',
      
    );   
    foreach ($keylist as $key => $keyname) {
      $elements = array(
        '#title' => $keyname,
        '#type' => 'fieldset',
        '#attributes' => array('class' => 'messaging-template-part'),
        //'#theme' => 'messaging_template_part_text',
      );
      $part = isset($texts[$key]) ? $texts[$key] : NULL;
      $text = $part ? $part->message : '';
      $format = $part && !empty($part->format) ? $part->format : 0;
      // Check access to input format and format the form accordingly
      $enabled = isset($formats_allowed[$format]);
      $elements['enabled'] = array('#type' => 'value', '#value' => $enabled);    
      $elements['tpid'] = array('#type' => 'value', '#value' => $part && isset($part->tpid) ? $part->tpid : 0);
      //$elements['title']['#value'] = $keyname;
      // Filter format, if disabled just show this one
      
      $elements['format'] = array(
        '#title' => t('Format'),
        '#type' => 'select',
        '#default_value' => $format,
        '#options' => $enabled ? $formats_allowed : $formats_all,
        '#disabled' => !$enabled,
        //'#prefix' => '<div class="text-part-header">',
        '#prefix' => '<div class="format-select">',
        '#suffix' => '</div>',
        //'#attributes' => array('class' => 'messaging-template-part-format-select'),
      );
      /*
      $elements['title'] = array(
        //'#prefix' => '<div class="text-part-header">',
        '#value' => '<span><strong>' . $keyname . '</strong></span>',
        '#suffix' => '</div>',
      );
      */

      $elements['message'] = array(
        '#type' => 'textarea',
        '#default_value' => $text,
        // Adjust size to actual number of lines
        '#rows' => count(explode("\n", $text)),
        '#disabled' => !$enabled,  
      );   
      $form['messages'][$method][$key] = $elements; 
    }
  }

  // Tokens for text replacement
  if ($tokens = messaging_template_tokens_get_list($group)) {
    $headers = array(t('Token'), t('Replacement value'));
    $rows = array();
    foreach ($tokens as $token => $token_description) {
        $row = array();
        $row[] = '[' . $token . ']';
        $row[] = $token_description;
        $rows[] = $row;
    }
    $form['tokens'] = array(
      '#title' => t('Available tokens'),
      '#type' => 'fieldset',
      '#description' => t('These special strings will be replaced by their real value at run time.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['tokens']['list'] = array(
      '#value' => theme('table', $headers, $rows, array('class' => 'description'))
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
  );
  return $form;
}

/**
 * Get list of all filter formats for this user
 */
function _messaging_template_filter_formats($all = FALSE) {
  $list[0] = t('None');
  if ($all) {
    $result = db_query("SELECT format, name FROM {filter_formats}");
    while($format = db_fetch_object($result)) {
      $list[$format->format] = $format->name;
    }
  }
  else {
    foreach (filter_formats() as $format) {
      $list[$format->format] = $format->name;
    }
  }
  return $list;
}

/**
 * Get list of tokens for text replacement
 *
 * @param $group
 *   Message group to get tokens for
 * @param $tokens
 *
 */
function messaging_template_tokens_get_list($group) {
  // First compile token types for this message group
  $type_list = module_invoke_all('messaging', 'tokens', $group);
  // Add known global tokens, will be always available
  $type_list[] = 'global';

  // Now get token list from token module for each type
  $return = array();
  foreach ($type_list as $type) {
    // This is a shortcut for single tokens for digests, with the form (token, description)
    if (is_array($type)) {
      list($type, $token) = $type;
      $list = token_get_list($type);
      $return[$token] = $list[$type][$token];
    }
    elseif ($list = token_get_list($type)) {
      foreach ($list as $category => $tokens) {
        foreach ($tokens as $token => $description) {
          $return[$token] = $description;
        }
      }
    }
  }
  return $return;
}

/**
 * Process and save message parts
 */
function messaging_template_admin_message_form_submit($form, &$form_state) {
  $group = $form_state['values']['group'];
  $language = $form_state['values']['language'];
  $module = $form_state['values']['module'];
  switch ($form_state['values']['op']) {
    case t('Reset to defaults'):
      db_query("DELETE FROM {messaging_message_parts} WHERE type = '%s' AND language = '%s'", $group, $language->language);
      drupal_set_message('The message templates have been reset to default values.');
      break;
    case t('Save'):
      foreach ($form_state['values']['messages'] as $method => $messages) {
        foreach ($messages as $key => $part) {
          if (!empty($part['enabled'])) {
            if (trim($part['message'])) {
              $part += array('type' => $group, 'msgkey' => $key, 'method' => $method, 'language' => $language->language, 'module' => $module);
              $update = !empty($part['tpid']) ? 'tpid' : array();
              drupal_write_record('messaging_message_parts', $part, $update);
            }
            elseif (!empty($part['tpid'])) {
              // There was a part now empty, delete it
              db_query("DELETE FROM {messaging_message_parts} WHERE tpid = %d", $part['tpid']);
            }
          }
        }
      }
      drupal_set_message('The message templates have been updated.');
      break;
  }
 
}

/**
 * Theme admin texts form
 */
function theme_messaging_template_part_text($elements) {
  $output = '';var_dump($elements);
  //$output = '<div class="messaging-template-part>';
  $output .= drupal_render($elements['#title']);
  $output .= '<div class="format-select">';
  $output .= drupal_render($elements['format']);
  $output .= '</div>';
  $output .= drupal_render($elements['message']);
  //$output .= '</div>';
  $output .= drupal_render($elements);
  return $output;
}