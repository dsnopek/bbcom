<?php
// $Id: nodecomment_plugin_display_comments.inc,v 1.1.2.2 2009/06/17 19:50:33 merlinofchaos Exp $

class nodecomment_plugin_display_comments extends views_plugin_display {
  function options_summary(&$categories, &$options) {
    parent::options_summary($categories, $options);

    $options['items_per_page']['value'] = t('From node type settings');
  }

  function options_form(&$form, &$form_state) {
    // specifically override these options
    if ($form_state['section'] == 'items_per_page') {
      $form['#title'] .= $this->use_pager() ? t('Items per page') : t('Items to display');

      $form['items_per_page'] = array(
        '#type' => 'value',
        '#value' => 10,
      );
      $form['markup'] = array(
        '#value' => t('The number of items to display will be taken from the node type settings.'),
      );
      $form['offset'] = array(
        '#type' => 'textfield',
        '#title' => t('Offset'),
        '#description' => t('The number of items to skip. For example, if this field is 3, the first 3 items will be skipped and not displayed. Offset can not be used if items to display is 0; instead use a very large number there.'),
        '#default_value' => intval($this->get_option('offset')),
      );
      return;
    }

    // otherwise do the default
    parent::options_form($form, $form_state);

    if ($form_state['section'] == 'items_per_page') {
      $form['style_plugin']['#description'] .= ' ' . t('Important note: the style will be overridden to the nodecomment threaded style if threading is enabled for the node type.');
    }
  }

  function query() {
    // Get the node from the argument handler
    if (empty($this->node)) {
      return;
    }

    // Override our sorting options if necessary
    $node_comments = $this->view->query->ensure_table('node_comments');
    $mode = _comment_get_display_setting('mode', $this->node);
    $order = _comment_get_display_setting('sort', $this->node);

    if ($order == COMMENT_ORDER_NEWEST_FIRST) {
      $sort = 'DESC';
      if ($mode == COMMENT_MODE_FLAT_COLLAPSED || $mode == COMMENT_MODE_FLAT_EXPANDED) {
        $table = 'node';
        $field = 'nid';
      }
      else {
        $table = $node_comments;
        $field = 'thread';
      }
    }
    else if ($order == COMMENT_ORDER_OLDEST_FIRST) {
      $sort = 'ASC';
      if ($mode == COMMENT_MODE_FLAT_COLLAPSED || $mode == COMMENT_MODE_FLAT_EXPANDED) {
        $table = 'node';
        $field = 'nid';
      }
      else {
        // See comment above. Analysis reveals that this doesn't cost too
        // much. It scales much much better than having the whole comment
        // structure.
        $table = NULL;
        $field = "SUBSTRING($node_comments.thread, 1, (LENGTH($node_comments.thread) - 1))";
      }
    }
    $this->view->query->add_orderby($table, $field, $sort, 'nodecomment_sort');
    $this->view->query->add_field('node', 'changed');

    parent::query();
  }

  /**
   * Called by an implementation of hook_views_pre_build() to let us
   * check the argument and make changes based upon global settings.
   */
  function pre_build() {
    if (!empty($this->view->args[0]) && $node = node_load($this->view->args[0])) {
      $this->node = $node;
      // Change items per page.
      $this->view->set_items_per_page(_comment_get_display_setting('comments_per_page', $node));
      $mode = _comment_get_display_setting('mode', $node);
      if ($mode == COMMENT_MODE_THREADED_COLLAPSED || $mode == COMMENT_MODE_THREADED_EXPANDED) {
        $this->set_option('style_plugin', 'nodecomment_threaded');
      }
    }
  }
}
