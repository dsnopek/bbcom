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
    // TODO: this isn't the best hint ever, but it works.
    return array('id' => $nid, 'revid' => $vid);
  }

  // TODO: get all the entries for this content item!
  $entries = array();
  
  $content = array(
    'id' => ''.$node->nid,
    'revid' => ''.$node->vid,
    'title' => $node->title,
    'body' => $node->body,
    'entries' => $entries
  );

  return $content;
}

