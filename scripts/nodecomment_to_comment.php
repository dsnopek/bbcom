<?php

// Convert from nodecoment back to normal comments without losing our cid's or 
// firing any of the internal (bad) Drupal events...

$res = db_query("SELECT c.*, r.title as subject, r.body as comment, r.format, n.created as timestamp, n.status as status FROM {node_comments} c JOIN {node} n ON c.cid = n.nid JOIN {node_revisions} r ON n.vid = r.vid");

while ($comment = db_fetch_object($res)) {
  // status has the reverse meaning for comments
  $comment->status = !$comment->status;

  print "cid: " . $comment->cid . "\n";
  print "nid: " . $comment->nid . "\n";

  // insert the new comment into the comments table
  db_query("REPLACE INTO {comments} (cid, pid, nid, uid, subject, comment, hostname, timestamp, status, format, thread, name, mail, homepage) VALUES (%d, %d, %d, %d, '%s', '%s', '%s', %d, %d, %d, '%s', '%s', '%s', '%s')", 
    $comment->cid, $comment->pid, $comment->nid, $comment->uid, $comment->subject, $comment->comment, $comment->hostname, $comment->timestamp, $comment->status, $comment->format, $comment->thread, $comment->name, $comment->mail, $comment->homepage);

  // updated node statistics?
  _comment_update_node_statistics($comment->nid);

  // preemptively remove the node_comments entry so that we won't activate it's delete handler
  db_query("DELETE FROM {node_comments} WHERE cid = %d", $comment->cid);

  // delete the old node
  _ttt_node_delete($comment->cid);
}

// clear the page and block caches
cache_clear_all();

// copied from node_delete() - removes messages, cache clear and access check
function _ttt_node_delete($nid) {
  // Clear the cache before the load, so if multiple nodes are deleted, the
  // memory will not fill up with nodes (possibly) already removed.
  $node = node_load($nid, NULL, TRUE);

  db_query('DELETE FROM {node} WHERE nid = %d', $node->nid);
  db_query('DELETE FROM {node_revisions} WHERE nid = %d', $node->nid);
  db_query('DELETE FROM {node_access} WHERE nid = %d', $node->nid);

  // Call the node-specific callback (if any):
  node_invoke($node, 'delete');
  node_invoke_nodeapi($node, 'delete');

  // Remove this node from the search index if needed.
  if (function_exists('search_wipe')) {
    search_wipe($node->nid, 'node');
  }
}

