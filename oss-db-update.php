<?php

function variable_move($old, $new) {
  $val = variable_get($old, NULL);
  if (!is_null($value)) {
    variable_set($new, $val);
    variable_del($old);
  }
}

function variable_mass_move($old_base, $new_base) {
  $res = db_query("SELECT name FROM variable WHERE name LIKE '%s_%%'", $old_base);
  while ($obj = db_fetch_object($res)) {
    $old = $obj->name;
    $new = preg_replace("/^$old_base/", $new_base, $old);
    //print ("$old => $new\n");
    variable_move($old, $new);
  }
}

function rename_lingwo_entry() {
  db_query("INSERT INTO lingwo_entry (language, pos, headword, nid, entry_hash) SELECT language, pos, headword, nid, entry_hash FROM lingwo_dictionary_entry");
  variable_move("lingwo_dictionary_entry_content_type", "lingwo_entry_content_type");
  variable_mass_move("lingwo_dictionary", "lingwo_entry");
}

function populate_lingwo_entry_translation() {
  db_query("INSERT INTO lingwo_entry_translation (nid, tnid) SELECT nid, tnid FROM node WHERE type = 'entry' AND tnid <> 0 AND tnid <> nid AND tnid IS NOT NULL");
}

rename_lingwo_entry();
//populate_lingwo_entry_translation();

