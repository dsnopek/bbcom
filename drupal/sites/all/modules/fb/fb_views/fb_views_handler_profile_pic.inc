<?php
// $Id: fb_views_handler_profile_pic.inc,v 1.3 2010/03/21 04:44:26 yogadex Exp $
/**
 * @file
 * Render fbml tag for user's picture.
 */

class fb_views_handler_profile_pic extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['fbu'] = 'fbu';
  }

  function render($values) {
    if ($fbu = $values->{$this->field_alias}) {
      return '<div class="picture"><fb:profile-pic uid="' . $fbu . '" linked="yes" size="thumb" /></div>';
    }
  }
}
