<?php
// $Id: nodecomment_handler_field_link.inc,v 1.1.2.2 2009/06/10 05:41:20 merlinofchaos Exp $
/**
 * Field handler to present a link to the node comment
 */
class nodecomment_handler_field_link extends views_handler_field_node_link  {
  function construct() {
    parent::construct();
    $this->additional_fields['parent_nid'] = array('table' => 'node_comments', 'field' => 'nid');
    $this->additional_fields['thread'] = array('table' => 'node_comments', 'field' => 'thread');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();

    $node_comments = $this->query->ensure_table('node_comments', $this->relationship);

    $def = array(
      'table' => 'node',
      'field' => 'nid',
      'left_table' => $node_comments,
      'left_field' => 'nid',
    );

    $join = new views_join();

    $join->definition = $def;
    $join->construct();
    $join->adjusted = TRUE;

    // Add more info to figure out what page the comment is on.
    $this->parent_node = $this->query->add_table('node', $this->relationship, $join);
    $this->aliases['parent_type'] = $this->query->add_field($this->parent_node, 'type');
  }

  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    $nid = $values->{$this->aliases['nid']};
    $this->options['alter']['make_link'] = TRUE;

    if (!empty($values->{$this->aliases['parent_nid']})) {
      // Fake up two nodes so we can get the target page:
      $comment = new stdClass();
      $comment->nid = $values->{$this->aliases['nid']};
      $comment->thread = $values->{$this->aliases['thread']};

      $parent = new stdClass();
      $parent->nid = $values->{$this->aliases['parent_nid']};
      $parent->type = $values->{$this->aliases['parent_type']};

      if ($pageno = nodecomment_page_count($comment, $parent)) {
        $this->options['alter']['query'] = 'page=' . $pageno;
      }

      $this->options['alter']['path'] = "node/$parent->nid";
      $this->options['alter']['fragment'] = 'comment-' . $nid;
    }
    else {
      $this->options['alter']['path'] = "node/$nid";
      $this->options['alter']['fragment'] = '';
    }

    $this->options['alter']['alter_text'] = TRUE;
    if (empty($this->options['alter']['text'])) {
      $this->options['alter']['text'] = $text;
    }
    return $text;
  }
}
