<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'info' => 'Bibliobird.com Marketing: TeamSpeak',
  'subject string' => 'Voice chat',
  'content' => '_bbcom_marketing_block_teamspeak',
);

function _bbcom_marketing_block_teamspeak() {
  global $language;

  $output = '<div id="ts3viewer_980053"> </div>

<script type="text/javascript" src="http://static.tsviewer.com/short_expire/js/ts3viewer_loader.js"></script>

<script type="text/javascript">
<!--
var ts3v_url_1 = "http://www.tsviewer.com/ts3viewer.php?ID=980053&text=000000&text_size=12&text_family=1&js=1&text_s_weight=bold&text_s_style=normal&text_s_variant=normal&text_s_decoration=none&text_s_color_h=525284&text_s_weight_h=bold&text_s_style_h=normal&text_s_variant_h=normal&text_s_decoration_h=underline&text_i_weight=normal&text_i_style=normal&text_i_variant=normal&text_i_decoration=none&text_i_color_h=525284&text_i_weight_h=normal&text_i_style_h=normal&text_i_variant_h=normal&text_i_decoration_h=underline&text_c_weight=normal&text_c_style=normal&text_c_variant=normal&text_c_decoration=none&text_c_color_h=525284&text_c_weight_h=normal&text_c_style_h=normal&text_c_variant_h=normal&text_c_decoration_h=underline&text_u_weight=bold&text_u_style=normal&text_u_variant=normal&text_u_decoration=none&text_u_color_h=525284&text_u_weight_h=bold&text_u_style_h=normal&text_u_variant_h=normal&text_u_decoration_h=none";
ts3v_display.init(ts3v_url_1, 980053, 100);
-->
</script>';

  $output .= '<div>' . t('Want to join our voice chat?  <a href="!url">Click here</a> for more information!', array('!url' => url('vchat'))) . '</div>';

  return $output;
}

