<?php

/**
 * Define templates for rate widgets.
 *
 * @return array
 */
function hook_rate_templates() {
  $templates = array();

  $templates['thumbs_up'] = new stdClass();
  $templates['thumbs_up']->value_type = 'points';
  $templates['thumbs_up']->options = array(
    array(1, 'up'),
  );

  $templates['thumbs_up_down'] = new stdClass();
  $templates['thumbs_up_down']->value_type = 'points';
  $templates['thumbs_up_down']->options = array(
    array(1, 'up'),
    array(-1, 'down'),
  );

  $templates['fivestar'] = new stdClass();
  $templates['fivestar']->value_type = 'percent';
  $templates['fivestar']->options = array(
    array(0, '*'),
    array(25, '**'),
    array(50, '***'),
    array(75, '****'),
    array(100, '*****'),
  );

  return $templates;
}

/**
 * Hook on actions done on rate widgets.
 *
 * @param string $op
 * @param object $widget
 * @param array $values Form values from rate_widget_form (on insert and update)
 *   or an arra with the keys "content_type", "content_id"
 *   and "teaser" (on load).
 */
function hook_rate_widget($op, &$widget, $values = NULL) {
  switch ($op) {
    case 'insert':
    case 'update':
      // Elements which are added to the $widget are saved automatically.
      break;
    case 'delete':
      break;
    case 'view':
      // The $widget object may be altered here before display.
      break;
  }
}

/**
 * Alter the vote before it is saved.
 *
 * @param array $vote
 *   array(
 *     'entity_type' => $content_type,
 *     'entity_id' => $content_id,
 *     'value_type' => $widget->value_type,
 *     'value' => $value,
 *     'tag' => $widget->tag,
 *   );
 * @param array $context
 *   array(
 *     'redirect' => &$redirect, // Path. Alter to redirect the user.
 *     'save' => &$save, // Boolean indicating whether the vote must be saved.
 *     'widget' => $widget, // Widget object, unalterable.
 *   );
 */
function hook_rate_vote_alter($vote, $context) {
  // Redirect users to the feedback page when they click on thumbs down.
  if ($context['widget']->name == 'thumbs_up_down' && $vote['value'] == -1) {
    $context['redirect'] = 'feedback';
  }
}