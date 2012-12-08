<?php

// tid = 10 is the Newsletter type
$res = db_query("SELECT nid FROM {taxonomy_index} WHERE tid = 10");
foreach ($res as $obj) {
  print 'nid: ' . $obj->nid . "\n";

  $node = node_load($obj->nid);
  $wrapper = entity_metadata_wrapper('node', $node);

  $wrapper->field_site_update_type = 'newsletter';
  $wrapper->save();
}

