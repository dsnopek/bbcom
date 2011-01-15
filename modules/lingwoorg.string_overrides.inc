<?php

function _lingwoorg_string_overrides() {
  return array(
   'Remember me' => 'Remember me!',
   'Access Denied / User Login' => 'User Login',
  );
}

// TODO: maybe we should merge with this value rather than overwrite?
$conf['locale_custom_strings_en'] = _lingwoorg_string_overrides();

