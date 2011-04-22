<?php

class nodecomment_plugin_style_threaded extends views_plugin_style {
  function render() {
    $divs = 0;
    $last_depth = 0;
    $output = '';
    drupal_add_css(drupal_get_path('module', 'comment') .'/comment.css');

    foreach ($this->view->result as $n) {
      $node = node_load($n->nid);
      $node->view = &$this->view;
      $node->depth = count(explode('.', $node->thread)) - 1;

      if ($node->depth > $last_depth) {
        $divs++;
        $output .= '<div class="indented">';
        $last_depth++;
      }
      else {
        while ($node->depth < $last_depth) {
          $divs--;
          $output .= '</div>';
          $last_depth--;
        }
      }
      $output .= node_view($node);
    }

    for ($i = 0; $i < $divs; $i++) {
      $output .= '</div>';
    }

    return $output;
  }
}
