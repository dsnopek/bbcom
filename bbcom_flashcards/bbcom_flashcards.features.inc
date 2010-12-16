<?php

/**
 * Implementation of hook_rules_defaults().
 */
function bbcom_flashcards_rules_defaults() {
  module_load_include('inc', 'bbcom_flashcards', 'bbcom_flashcards.defaults');
  $args = func_get_args();
  return call_user_func_array('_bbcom_flashcards_rules_defaults', $args);
}
