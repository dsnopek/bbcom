<?php

/**
 * @file
 * The actual callbacks for this service
 */

function _lingwoorg_device_service_get_revcount($nid) {
  # we default to 1, because having any translation counts for 1 (this is helpful
  # when, for example, an entry is already on the device BUT not for a specific translation
  # and then that translation is added).
  $res = db_query("SELECT COUNT(*) FROM {node_revisions} WHERE nid = %s", $nid);
  if ($res && $row = db_fetch_array($res)) {
    return $row[0];
  }
  # return 0 because we can't even find data for this translation, which can't be a good
  # sign!
  return 0;
}

// TODO: should probably be in lingwo_data!
function lingwoorg_device_service_export_entry($nid) {
  $node = node_load($nid);
  $base_revid = $node->vid;
  $entry = array(
    'language' => $node->language,
    'pos' => lingwo_dictionary_entry_get_pos($node),
    'headword' => $node->title,
    # TODO: maybe a timestamp would be better?  We would just take the newest timestamp from
    # among the source and all the translations?
    'revid' => 0+$node->vid,
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

    # take the translation revisions into account with the overall revid
    # TODO: See note above about how a timestamp would be good instead...
    $entry['revid'] += _lingwoorg_device_service_get_revcount($tnode->nid);

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
function lingwoorg_device_service_get_content_item($software_version, $device_name, $nid, $vid = '') {
  if ($device_name != 'device_1') {
    return services_error("Unknown device name", 400);
  }

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

/*
 * lingwoorg_device.pull_update
 */
function lingwoorg_device_service_pull_update($software_version, $device_name) {
  # TODO: make this really work!  For now, just give everything to 'device_1' and 
  # no one else.
  if ($device_name == 'device_1') {
    $res = db_query("SELECT nid, vid FROM {node} WHERE type = 'content' AND status = 1");
    while ($row = db_fetch_array($res)) {
      $content[] = implode(':', $row);
    }
    return array(
      'users' => array('Test User'),
      'content' => $content,
      #'software_update' => '',
    );
  }
  return services_error("Unknown device name", 400);
}

/*
 * lingwoorg_device.push_update
 */
function lingwoorg_device_service_push_update($software_version, $device_name, $data) {
  global $user;

  if ($device_name != 'device_1') {
    return services_error("Unknown device name", 400);
  }

  $flag = flag_get_flag('wial');
  foreach ($data as $username => $user_data) {
    $push_user = user_load(array('name' => $username));
    # TODO: implement teacher accounts
    if ($user->uid != 0 && $push_user->uid != $user->uid) {
      return services_error("Cannot update data for another user!", 403);
    }
    if (empty($push_user)) {
      continue;
    }
    $wial_add = $user_data['wial_add'];
    foreach ($wial_add as $entry_spec) {
      list($language, $pos, $headword) = explode(':', $entry_spec);
      $nid = lingwo_dictionary_entry_lookup($headword, $language, $pos);
      if ($nid) {
        $flag->flag('flag', $nid, $push_user);
      }
    }
  }
  return TRUE;
}

