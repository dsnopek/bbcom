<?php

/**
 * Implementation of hook_content_default_fields().
 */
function bbcom_exercises_content_default_fields() {
  $fields = array();

  // Exported field: field_content_exercises
  $fields['content-field_content_exercises'] = array(
    'field_name' => 'field_content_exercises',
    'type_name' => 'content',
    'display_settings' => array(
      'label' => array(
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'nodereference',
    'required' => '0',
    'multiple' => '0',
    'module' => 'nodereference',
    'active' => '1',
    'referenceable_types' => array(
      'quiz' => 'quiz',
      'content' => 0,
      'entry' => 0,
      'forum_reply' => 0,
      'forum' => 0,
      'language' => 0,
      'long_answer' => 0,
      'matching' => 0,
      'multichoice' => 0,
      'page' => 0,
      'quiz_directions' => 0,
      'rwiki' => 0,
      'scale' => 0,
      'short_answer' => 0,
      'story' => 0,
      'truefalse' => 0,
      'wiki' => 0,
    ),
    'advanced_view' => '--',
    'advanced_view_args' => '',
    'widget' => array(
      'autocomplete_match' => 'contains',
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'nid' => NULL,
          '_error_element' => 'default_value_widget][field_content_exercises][0][nid][nid',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Exercises',
      'weight' => '4',
      'description' => '',
      'type' => 'nodereference_autocomplete',
      'module' => 'nodereference',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Exercises');

  return $fields;
}
