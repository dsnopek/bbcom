<?php

print "DO NOT USE ME!\n";
exit(1);

function _rename_field($old_name, $new_name) {
  $args = array(
    ':old_name' => $old_name,
    ':new_name' => $new_name
  );

  db_query("UPDATE {field_config} SET field_name = :new_name WHERE field_name = :old_name", $args);
  db_query("UPDATE {field_config_instance} SET field_name = :new_name WHERE field_name = :old_name", $args);
  db_query("RENAME TABLE `field_data_$old_name` TO `field_data_$new_name`");
  db_query("RENAME TABLE `field_revision_$old_name` TO `field_revision_$new_name`");

  drupal_flush_all_caches();
}

$renames = array(
  'field_profile_image' => 'field_user_picture',
);

foreach ($renames as $old_name => $new_name) {
  _rename_field($old_name, $new_name);
}

