<?php

function populate_lingwo_entry_info() {
  $cur = variable_get('oss_db_update', 0);
  $ret = db_query("SELECT n.nid, le.pos FROM {node} n, {lingwo_entry} le WHERE n.type = '%s' AND le.nid = n.tnid AND n.nid > %d ORDER BY n.nid LIMIT 50", LingwoEntry::$settings->content_type, $cur); 
  while($obj = db_fetch_object($ret)) {
    $node = node_load($obj->nid);
    $node->pos = $obj->pos;
    $entry = LingwoEntry::fromNode($node);
    $entry_hash = _lingwo_entry_generate_hash($entry);
    $info = (object)array(
      'nid'             => $entry->nid,
      'language'        => implode('-', $entry->getLanguages()),
      'source_language' => $entry->sourceLanguage,
      'target_language' => $entry->targetLanguage,
      'pos'             => $obj->pos,
      'translation'     => $entry->isTranslation(),
      'entry_hash'      => $entry_hash,
    );
    drupal_write_record('lingwo_entry_info', $info);
    node_load(NULL, NULL, TRUE);
    $cur = $entry->nid;
  }
  variable_set('oss_db_update', $cur);
}

populate_lingwo_entry_info();

