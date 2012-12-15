<?php

variable_set('bibliobird_profile_role_titles', variable_get('bbcom_profile_role_titles', array()));

$i = 0;
foreach (db_query("SELECT n.*, b.body_value AS body, b.body_format AS format FROM {node} n JOIN {field_data_body} b ON n.vid = b.revision_id WHERE n.type = 'profile'") as $obj) {
  $nid = $obj->nid;
  $vid = $obj->vid;
  $uid = $obj->uid;

  print "uid: $uid\n";

  $user = entity_metadata_wrapper('user', user_load($uid));

  if (!empty($obj->body)) {
    //print_r($user->field_user_about->getPropertyInfo());
    $user->field_user_about->value = $obj->body;
  }

  $picture = db_query("SELECT * FROM {content_type_profile} p JOIN {files} f ON f.fid = p.field_profile_image_fid  WHERE p.nid = :nid", array(':nid' => $nid))->fetch(); 
  if (!empty($picture->fid)) {
    print "fid: $picture->fid\n";
    //print_r($user->field_user_picture->file->getPropertyInfo());
    $user->field_user_picture->file = file_load($picture->fid);
  }

  if (count($user->field_profile_languages) == 0) {
    $res = db_query("SELECT * FROM {content_field_profile_learning_language} a JOIN {content_field_profile_learning_level} b ON a.vid = b.vid AND a.delta = b.delta WHERE a.vid = :vid ORDER BY a.delta", array(':vid' => $vid));
    foreach ($res as $index => $lang_data) {
      if (!empty($lang_data->field_profile_learning_language_value)) {
        $row = entity_create('field_collection_item', array('field_name' => 'field_profile_languages'));
        $row->setHostEntity($user->type(), $user->value());

        $user->field_profile_languages[$index] = $row;
        $user->field_profile_languages[$index]->field_profile_languages_language = $lang_data->field_profile_learning_language_value;
        $user->field_profile_languages[$index]->field_profile_languages_level = $lang_data->field_profile_learning_level_value;
      }
    }
  }

  $user->save();

  /*
  if (++$i > 2) {
    break;
  }
  */
}

