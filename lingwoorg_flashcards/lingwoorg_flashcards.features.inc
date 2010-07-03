<?php

/**
 * Implementation of hook_rules_defaults().
 */
function lingwoorg_flashcards_rules_defaults() {
  module_load_include('inc', 'lingwoorg_flashcards', 'lingwoorg_flashcards.defaults');
  $args = func_get_args();
  return call_user_func_array('_lingwoorg_flashcards_rules_defaults', $args);
}
