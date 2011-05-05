<?php

function _bbcom_subscribe_to_thread($uid, $nid) {
  $account = user_load($uid);
  $subscription = array(
    'type' => 'thread',
    'uid'  => $uid,
    'fields' => array('nid' => $nid),
    'send_method' => notifications_user_setting('send_method', $account),
    'send_interval' => notifications_user_setting('send_interval', $account),
    'event_type' => 'node',
  );
  $subscription = notifications_build_subscription($subscription);
  $ret = notifications_save_subscription($subscription);
}

// nodes users have commented on (via Drupal comment.module)
$res = db_query("SELECT uid, nid FROM {comments} WHERE status = 0");
while ($obj = db_fetch_object($res)) {
  _bbcom_subscribe_to_thread($obj->uid, $obj->nid);
}

// nodes users have commented on (via node_comment.module)
$res = db_query("SELECT c.uid, c.nid FROM {node_comments} c, {node} n WHERE n.nid = c.nid AND status = 1");
while ($obj = db_fetch_object($res)) {
  _bbcom_subscribe_to_thread($obj->uid, $obj->nid);
}

// nodes users have created (exceptions for 'forum_reply' which is really a comment, and 'Importer' who
// has created a ton of nodes, but isn't really a user!)
$res = db_query("SELECT uid, nid FROM {node} WHERE status = 1 AND uid <> 3 AND type <> 'forum_reply'");
while ($obj = db_fetch_object($res)) {
  _bbcom_subscribe_to_thread($obj->uid, $obj->nid);
}

