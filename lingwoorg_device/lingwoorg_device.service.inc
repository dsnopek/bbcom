<?php

/**
 * @file
 * The actual callbacks for this service
 */

// TODO: should probably be in lingwo_data!
function lingwoorg_device_service_export_entry($nid) {
  $node = node_load($nid);
  $entry = array(
    'language' => $node->language,
    'pos' => lingwo_dictionary_entry_get_pos($node),
    'headword' => $node->title,
    'revid' => $node->vid,
    'fields' => array(),
    'senses' => array(),
    'translations' => array()
  );

  # do up fields
  foreach ($node->field_fields as $field) {
    if (!empty($field['name'])) {
      $entry['fields'][] = array(
        'name' => $field['name'],
        'type' => $field['type'],
        'value' => $field['value'],
        'automatic' => $field['automatic'] == '1' ? TRUE : FALSE,
      );
    }
  }

  # do up the senses
  foreach ($node->field_senses as $orig_sense) {
    $sense = array(
      'id' => $orig_sense['id']
    );
    foreach (array('difference', 'example') as $tmp) {
      if (!empty($orig_sense['data'][$tmp])) {
        $sense[$tmp] = $orig_sense['data'][$tmp];
      }
    }
    $entry['senses'][] = $sense;
  }

  # do up the translations
  foreach (translation_node_get_translations($nid) as $lang => $tnode) {
    if ($tnode->language == $node->language) {
      continue;
    }

    $tnode = node_load($tnode->nid);

    $translations = array();
    foreach ($tnode->field_senses as $sense) {
      $trans = array();

      # copy the properties
      foreach (array('trans', 'clue', 'same_as', 'no_equivalent') as $tmp) {
        if (!empty($sense['data'][$tmp])) {
          $trans[$tmp] = $sense['data'][$tmp];
        }
      }

      # clean-up trans
      if (isset($trans['trans'])) {
        $trans['trans'] = array_filter($trans['trans']);
        if (empty($trans['trans'])) {
          unset($trans['trans']);
        }
      }

      # no equivalent needs to be sure to be boolean
      if ($trans['no_equivalent']) {
        $trans['no_equivalent'] = TRUE;
      }

      # remove trans if its not needed
      if ($trans['same_as'] || $trans['no_equivalent']) {
        unset($trans['trans']);
      }

      if (!empty($trans)) {
        $translations[$sense['id']] = $trans;
      }
    }

    $entry['translations'][$lang] = $translations;
  }

  return $entry;
}

/*
 * lingwoorg_device.get_content_item
 */
function lingwoorg_device_service_get_content_item($nid, $vid = '') {
  $node = node_load($nid);
  if (!$node || $node->type != 'content') {
    return services_error('No such id', 404);
  }
  if ($vid != '' && $node->vid == $vid) {
    // the client should check if the revid's match
    return array('id' => $nid, 'revid' => $vid);
  }

  // TODO: get all the entries for this content item!
  $entries = array();
  $res = db_query("SELECT language, pos, headword FROM {lingwo_korpus_entry} WHERE nid = %d", $nid);
  while ($obj = db_fetch_object($res)) {
    $entry = lingwoorg_device_service_export_entry(
      lingwo_dictionary_entry_lookup($obj->headword, $obj->language, $obj->pos));
    if ($entry) {
      $entries[] = $entry;
    }
  }
  
  $content = array(
    'id' => ''.$node->nid,
    'language' => $node->language,
    'revid' => 0+$node->vid,
    'title' => $node->title,
    'body' => $node->body,
    'entries' => $entries
  );

  if (!empty($node->field_audio[0]['filepath'])) {
    $content['audio'] = file_create_url($node->field_audio[0]['filepath']);
  }

  return $content;
}

