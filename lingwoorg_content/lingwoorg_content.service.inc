<?php

/**
 * @file
 * The actual callbacks for this service
 */

/*
 * lingwoorg_content.get_content_item
 */
function lingwoorg_content_service_get_content_item($nid, $vid = '') {
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
  
  $content = array(
    'id' => ''.$node->nid,
	'language' => $node->language,
    'revid' => ''.$node->vid,
    'title' => $node->title,
    'body' => $node->body,
    'entries' => $entries
  );

  if (!empty($node->field_audio[0]['filepath'])) {
  	$content['audio'] = file_create_url($node->field_audio[0]['filepath']);
  }

  return $content;
}

