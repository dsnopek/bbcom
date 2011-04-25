<?php

$res = db_query("SELECT nid FROM {node} WHERE type = 'forum' OR type = 'forum_reply'");
while ($obj = db_fetch_object($res)) {
  print "$obj->nid\n";
  _nodecomment_update_node_statistics($obj->nid);
}

