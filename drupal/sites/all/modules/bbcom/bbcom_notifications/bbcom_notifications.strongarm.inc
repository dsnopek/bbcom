<?php

/**
 * Implementation of hook_strongarm().
 */
function bbcom_notifications_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'messaging_default_method';
  $strongarm->value = 'mail';

  $export['messaging_default_method'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'messaging_method_enabled';
  $strongarm->value = array(
    'mail' => 'mail',
  );

  $export['messaging_method_enabled'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_content_per_type';
  $strongarm->value = '0';

  $export['notifications_content_per_type'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_content_type';
  $strongarm->value = array(
    'taxonomy' => 'taxonomy',
    'thread' => 'thread',
  );

  $export['notifications_content_type'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_default_auto';
  $strongarm->value = 1;

  $export['notifications_default_auto'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_default_send_interval';
  $strongarm->value = 0;

  $export['notifications_default_send_interval'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_digest_methods';
  $strongarm->value = array(
    0 => 'simple',
    3600 => 'short',
    43200 => 'short',
    86400 => 'short',
    604800 => 'long',
  );

  $export['notifications_digest_methods'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_event_enabled';
  $strongarm->value = array(
    'node-insert' => 1,
    'node-content-insert' => 1,
    'node-forum_reply-insert' => 0,
    'node-story-insert' => 1,
    'node-rwiki-insert' => 1,
    'node-wiki-insert' => 1,
    'node-language-insert' => 1,
    'node-forum-insert' => 1,
    'node-entry-insert' => 1,
    'node-page-insert' => 1,
    'node-update' => 1,
    'node-content-update' => 1,
    'node-forum_reply-update' => 0,
    'node-story-update' => 1,
    'node-rwiki-update' => 1,
    'node-wiki-update' => 1,
    'node-language-update' => 1,
    'node-forum-update' => 0,
    'node-entry-update' => 1,
    'node-page-update' => 1,
    'node-comment' => 1,
    'node-content-comment' => 1,
    'node-forum_reply-comment' => 1,
    'node-story-comment' => 1,
    'node-rwiki-comment' => 1,
    'node-wiki-comment' => 1,
    'node-language-comment' => 1,
    'node-forum-comment' => 1,
    'node-entry-comment' => 1,
    'node-page-comment' => 1,
  );

  $export['notifications_event_enabled'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_event_template';
  $strongarm->value = array(
    'node-insert' => 'notifications-event-node-insert',
    'node-content-insert' => 'notifications-event-node-insert',
    'node-forum_reply-insert' => 'notifications-content-forum_reply-insert',
    'node-story-insert' => 'notifications-event-node-insert',
    'node-rwiki-insert' => 'notifications-event-node-insert',
    'node-wiki-insert' => 'notifications-event-node-insert',
    'node-language-insert' => 'notifications-event-node-insert',
    'node-forum-insert' => 'notifications-event-node-insert',
    'node-entry-insert' => 'notifications-event-node-insert',
    'node-page-insert' => 'notifications-event-node-insert',
    'node-update' => 'notifications-event-node-update',
    'node-content-update' => 'notifications-event-node-update',
    'node-forum_reply-update' => 'notifications-event-node-update',
    'node-story-update' => 'notifications-event-node-update',
    'node-rwiki-update' => 'notifications-event-node-update',
    'node-wiki-update' => 'notifications-event-node-update',
    'node-language-update' => 'notifications-event-node-update',
    'node-forum-update' => 'notifications-event-node-update',
    'node-entry-update' => 'notifications-event-node-update',
    'node-page-update' => 'notifications-event-node-update',
    'node-comment' => 'notifications-event-node-comment',
    'node-content-comment' => 'notifications-event-node-comment',
    'node-forum_reply-comment' => 'notifications-content-forum-comment',
    'node-story-comment' => 'notifications-event-node-comment',
    'node-rwiki-comment' => 'notifications-event-node-comment',
    'node-wiki-comment' => 'notifications-event-node-comment',
    'node-language-comment' => 'notifications-event-node-comment',
    'node-forum-comment' => 'notifications-content-forum-comment',
    'node-entry-comment' => 'notifications-event-node-comment',
    'node-page-comment' => 'notifications-event-node-comment',
  );

  $export['notifications_event_template'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_send_immediate';
  $strongarm->value = 0;

  $export['notifications_send_immediate'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_send_intervals';
  $strongarm->value = array(
    0 => 'Immediately',
    3600 => 'Every hour',
    43200 => 'Twice a day',
    86400 => 'Daily',
    604800 => 'Weekly',
  );

  $export['notifications_send_intervals'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_sender';
  $strongarm->value = '1';

  $export['notifications_sender'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_sendself';
  $strongarm->value = 0;

  $export['notifications_sendself'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_tags_showsubscribed';
  $strongarm->value = 0;

  $export['notifications_tags_showsubscribed'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_tags_vocabularies';
  $strongarm->value = array(
    2 => 2,
    1 => 0,
    4 => 0,
    3 => 0,
  );

  $export['notifications_tags_vocabularies'] = $strongarm;
  return $export;
}
