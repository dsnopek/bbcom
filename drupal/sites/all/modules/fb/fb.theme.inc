<?php
// $Id: fb.theme.inc,v 1.13 2010/11/02 18:49:00 yogadex Exp $
/**
 * @file
 * Implementation of theme functions.
 *
 * XFBML markup will only work on canvas pages and facebook connect pages.
 */


function theme_fb_login_button($text = '', $options = array()) {
  // Merge in defaults
  $options += array(
    'attributes' => array(),
  );
  if (!isset($options['attributes']['perms'])) {
    // Which permissions to prompt for?
    $perms = array();
    drupal_alter('fb_required_perms', $perms);
    if (count($perms)) {
      $options['attributes']['perms'] = implode(',', $perms);
    }
  }

  $button = '<fb:login-button ' . drupal_attributes($options['attributes']) . '>' .
    ($text ? '<fb:intl>' . check_plain($text) . '</fb:intl>' : '') . '</fb:login-button>';
  return $button;
}


function theme_fb_username($fbu, $object, $orig) {
  if (!$fbu || arg(0) == 'admin') {
    return $orig;
  }

  $wrap_pre = "<span class=fb_hide>$orig</span><span class=fb_show style='display:none'>";
  $wrap_post = "</span>\n";
  $ifcantsee = '';
  if ($object->uid && user_access('access user profiles')) {
    // Provide link if local account.
    $wrap_pre = $wrap_pre . '<a href="' . url('user/' . $object->uid) . '">';
    $wrap_post = '</a>' . $wrap_post;
    $ifcantsee = 'ifcantsee="' . addslashes(check_plain($object->name)) . '"';
  }

  $fbml = "<fb:name linked=false useyou=false uid=\"$fbu\" $ifcantsee></fb:name>";  $output =  $wrap_pre . $fbml . $wrap_post;

  return $output;
}

function theme_fb_user_picture($fbu, $object, $orig) {
  if (!$fbu)
    return $orig;

  $fbml = "<fb:profile-pic linked=false uid=\"$fbu\"></fb:profile-pic>";
  $wrap_pre = '<span class=fb_hide>' . $orig . '</span><span class="fb_show" style="display:none;"><div class="picture">';
  $wrap_post = '</div></span>';
  if ($object->uid && user_access('access user profiles')) {
    // Provide link to local account.
    $wrap_pre = $wrap_pre . '<a href="' . url('user/' . $object->uid) . '">';
    $wrap_post = '</a>' . $wrap_post;
  }
  return $wrap_pre . $fbml . $wrap_post;
}


function theme_fb_fbml_popup($elem) {
  // Hide this markup until javascript shows it.
  $t = '<div class="fb_fbml_popup_wrap" style="display:none;" ' . ">\n";

  $t .= '<a href="#" title="' . check_plain($elem['#title']) . '" ' .
    (isset($elem['#link_attributes']) ? drupal_attributes($elem['#link_attributes']) : '') .
    '>' . check_plain($elem['#link_text']) .'</a>';
  $t .= '<div class="fb_fbml_popup" ' . drupal_attributes($elem['#attributes']) . '>';
  $t .= $elem['#children'];
  $t .= "</div></div>\n";
  return $t;
}
