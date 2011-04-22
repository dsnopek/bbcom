<?php
// $Id: nodecomment_plugin_cache_comments.inc,v 1.1.2.2 2009/06/17 19:50:33 merlinofchaos Exp $

/**
 * Cache comment threads in Views.
 */
class nodecomment_plugin_cache_comments extends views_plugin_cache {
  function option_defaults(&$options) {
    $options['argument'] = '';
  }

  function options_form(&$form, &$form_state) {
    $options = array();
    $arguments = $this->view->display_handler->get_handlers('argument');
    foreach ($arguments as $id => $argument) {
      $options[$id] = $argument->ui_name();
    }

    $form['argument'] = array(
      '#type' => 'select',
      '#title' => t('Argument to use'),
      '#description' => t('The argument that contains the node ID. This content will be re-cached whenever that node is updated. If there is no node ID then the cache expiration will default to 1 hour.'),
      '#options' => $options,
      '#default_value' => $this->options['argument'],
    );
  }

  function summary_title() {
    return t('Node comment thread');
  }

  function cache_expire($type) {
    // extract the node ID.
    if (isset($this->view->argument[$this->options['argument']])) {
      $nid = $this->view->argument[$this->options['argument']]->argument;;
      $node = node_load($nid);
      if ($node) {
        return max($node->changed, $node->last_comment_timestamp);
      }
    }

    // default to 1 hour.
    return time() - 3600;
  }

  /**
   * Post process cached output for new strings.
   *
   * The template preprocess will use placeholders for any 'new' output, so
   * that the post process can replace it. This postprocess runs despite caching,
   * so the freshness of comments can always be checked accurately for the
   * logged in user. Without this, the "new" values are incorrect. This can
   * be extended by modules that utlize other values that need to be
   * freshened very easily with hook_views_post_render.
   */
  function post_render(&$output) {
    $tokens = array();
    // First comment checking.
    static $first_new = TRUE;
    if (!isset($this->view->argument[$this->options['argument']])) {
      return;
    }

    $nid = $this->view->argument[$this->options['argument']]->argument;;
    $node = node_load($nid);
    if (!$node) {
      return;
    }

    // Set up tokens for each row.
    foreach ($this->view->result as $id => $row) {
      // we probably shouldn't use node_created directly here, but the display
      // doesn't use any relationship so the chances of this alias failing is
      // much slimmer than other weird things going wrong.
      $new_output = $first = $new_class = '';

      $new = node_mark($node->nid, $row->node_changed);
      if ($new) {
        $new_output = $new ? '<span class="new">' . t('new') . '</span>' : '';
        $new_class = 'comment-new';
        if ($first_new) {
          $first = "<a id=\"new\"></a>";
          $first_new = FALSE;
        }
      }

      $tokens["<!--post:first-new-$row->nid-->"] = $first;
      $tokens["<!--post:new-$row->nid-->"] = $new_output;
      $tokens["<!--post:new-class-$row->nid-->"] = $new_class;
    }

    // Replace
    $output = strtr($output, $tokens);
  }
}
