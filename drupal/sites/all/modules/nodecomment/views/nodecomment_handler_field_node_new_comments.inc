<?php
// $Id: nodecomment_handler_field_node_new_comments.inc,v 1.1.2.1 2009/06/15 21:58:03 merlinofchaos Exp $

/**
 * Field handler to display the number of new comments
 */
class nodecomment_handler_field_node_new_comments extends views_handler_field_node_new_comments {
  function pre_render(&$values) {
    global $user;
    if (!$user->uid || empty($values)) {
      return;
    }

    $nids = array(); // for comments
    $nc_nids = array(); // for node_comments
    $ids = array();
    foreach ($values as $id => $result) {
      if (nodecomment_get_comment_type($result->{$this->aliases['type']})) {
        $nc_nids[] = $result->{$this->aliases['nid']};
      }
      else {
        $nids[] = $result->{$this->aliases['nid']};
      }
      $values[$id]->{$this->field_alias} = 0;
      // Create a reference so we can find this record in the values again.
      if (empty($ids[$result->{$this->aliases['nid']}])) {
        $ids[$result->{$this->aliases['nid']}] = array();
      }
      $ids[$result->{$this->aliases['nid']}][] = $id;
    }

    if ($nids) {
      $result = db_query("SELECT n.nid, COUNT(c.cid) as num_comments FROM {node} n INNER JOIN {comments} c ON n.nid = c.nid LEFT JOIN {history} h ON h.nid = n.nid AND h.uid = %d WHERE n.nid IN (" . implode(', ', $nids) . ") AND c.timestamp > GREATEST(COALESCE(h.timestamp, %d), %d) AND c.status = %d GROUP BY n.nid  ", $user->uid, NODE_NEW_LIMIT, NODE_NEW_LIMIT, COMMENT_PUBLISHED);

      while ($node = db_fetch_object($result)) {
        foreach ($ids[$node->nid] as $id) {
          $values[$id]->{$this->field_alias} = $node->num_comments;
        }
      }
    }

    if ($nc_nids) {
      $result = db_query("SELECT n.nid, COUNT(c.cid) as num_comments FROM {node} n INNER JOIN {node_comments} c ON n.nid = c.nid INNER JOIN {node} nc ON c.cid = nc.nid LEFT JOIN {history} h ON h.nid = n.nid AND h.uid = %d WHERE n.nid IN (" . implode(', ', $nc_nids) . ") AND nc.created > GREATEST(COALESCE(h.timestamp, %d), %d) AND nc.status <> 0 GROUP BY n.nid  ", $user->uid, NODE_NEW_LIMIT, NODE_NEW_LIMIT);

      while ($node = db_fetch_object($result)) {
        foreach ($ids[$node->nid] as $id) {
          $values[$id]->{$this->field_alias} = $node->num_comments;
        }
      }

    }

  }

  function render_link($data, $values) {
    if (!empty($this->options['link_to_comment']) && $data !== NULL && $data !== '') {
      $node = new stdClass();
      $node->nid = $values->{$this->aliases['nid']};
      $node->type = $values->{$this->aliases['type']};
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'node/' . $node->nid;
      $this->options['alter']['query'] = nodecomment_new_page_count($values->node_comment_statistics_comment_count, $values->{$this->field_alias}, $node);
      $this->options['alter']['fragment'] = 'new';
    }

    return $data;
  }
}
