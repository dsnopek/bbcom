<?php

/**
 * Implementation of hook_strongarm().
 */
function lingwoorg_content_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'allowed_html_1';
  $strongarm->value = '<a> <em> <strong> <cite> <code> <ul> <ol> <li> <dl> <dt> <dd> <word>';

  $export['allowed_html_1'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_content';
  $strongarm->value = array(
    'title' => -5,
    'menu' => -4,
    'language' => -3,
    'body_field' => -2,
    'revision_information' => 1,
    'comment_settings' => 2,
  );

  $export['content_extra_weights_content'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'flowplayer3_mediaplayer';
  $strongarm->value = array(
    'clip' => array(
      'autoPlay' => 'false',
      'autoBuffering' => 'false',
      'scaling' => 'scale',
      'start' => '',
      'duration' => '',
      'accelerated' => 'false',
      'bufferLength' => '',
      'provider' => '',
      'fadeInSpeed' => '',
      'fadeOutSpeed' => '',
      'linkUrl' => '',
      'linkWindow' => '_blank',
      'live' => 'false',
      'cuePointMultiplier' => '',
    ),
    'controls' => array(
      'backgroundGradient' => 'medium',
      'progressGradient' => 'medium',
      'bufferGradient' => 'none',
      'sliderGradient' => 'none',
      'autoHide' => 'fullscreen',
      'play' => 'true',
      'volume' => 'true',
      'mute' => 'true',
      'time' => 'true',
      'stop' => 'false',
      'playlist' => 'false',
      'fullscreen' => 'false',
      'scrubber' => 'true',
    ),
    'canvas' => array(
      'height' => '24',
      'width' => '560',
      'backgroundImage' => '',
      'backgroundRepeat' => 'repeat',
      'backgroundGradient' => 'low',
      'border' => '',
      'borderRadius' => '',
    ),
  );

  $export['flowplayer3_mediaplayer'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'flowplayer3_mediaplayer_file';
  $strongarm->value = 'flowplayer-3.1.5.swf';

  $export['flowplayer3_mediaplayer_file'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'flowplayer3_mediaplayer_stream_plugin';
  $strongarm->value = '';

  $export['flowplayer3_mediaplayer_stream_plugin'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'i18n_node_content';
  $strongarm->value = 1;

  $export['i18n_node_content'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_content';
  $strongarm->value = 1;

  $export['language_content_type_content'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'lingwo_korpus_annotation_reader_block_footer_id';
  $strongarm->value = 'footer';

  $export['lingwo_korpus_annotation_reader_block_footer_id'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'lingwo_korpus_annotation_reader_block_footer_margin';
  $strongarm->value = '15';

  $export['lingwo_korpus_annotation_reader_block_footer_margin'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'lingwo_korpus_field_remote_url';
  $strongarm->value = 'field_remote_url';

  $export['lingwo_korpus_field_remote_url'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'lingwo_korpus_text_content_type';
  $strongarm->value = 'content';

  $export['lingwo_korpus_text_content_type'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'swftools_embed_method';
  $strongarm->value = 'swfobject2_replace';

  $export['swftools_embed_method'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'swftools_mp3_display';
  $strongarm->value = 'flowplayer3_mediaplayer';

  $export['swftools_mp3_display'] = $strongarm;
  return $export;
}
