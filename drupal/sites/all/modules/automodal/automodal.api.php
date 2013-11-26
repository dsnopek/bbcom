<?php

/**
 * Alter the settings for an automodal selector.
 *
 * When an automodal selector is added to a page the settings for the selector
 * can be altered just before they are added to the page.
 *
 * @param array $settings
 *   An array of settings for the selector.
 *   @see automodal_add()
 * @param string $selector
 *   A CSS 3 selector that will be passed to jQuery. This can be used to test
 *   for specific selectors.
 */
function hook_automodal_alter(&$settings, $selector) {
  if ($selector == '.automodal') {
    $settings['width'] = 900;
  }
}

/**
 * Alter the arguments passed into modalframe_close_dialog() from automodal.
 *
 * When automodal closes a window arguments are passed through
 * modalframe_close_dialog() and on to the onSubmit handlers. These are all
 * functions living under Drupal.automodal.onSubmitCallback. Each of these
 * functions has the arguments of args and statusMessages passed in. The same
 * arguments are passed into each of these so the names should be namespaced.
 *
 * New functions can be added under Drupal.automodal.onSubmitCallback and will
 * be executed.
 *
 * @param array $args
 *   An an associative array of information to be passed into the JavaScript
 *   onSubmit callbacks. Can include the following special automodal keys:
 *   - close: A boolean thats TRUE by default. Set to FALSE to prevent close.
 *   - redirect: The URL to redirect to after close.
 *   - reload: A boolean to indicate if the parent should be reloaded or not.
 */
function hook_automodal_close_args_alter(array &$args, $form, $form_state) {
  $args['foo'] = 'bar';
}
