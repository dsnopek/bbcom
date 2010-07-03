<?php

/**
 * Helper to implementation of hook_rules_defaults().
 */
function _lingwoorg_flashcards_rules_defaults() {
  return array(
    'rules' => array(
      'lingwoorg_flashcards_1' => array(
        '#type' => 'rule',
        '#set' => 'event_flag_flagged_wial',
        '#label' => 'When an entry flagged WIAL, a flashcard should be created.',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'lingwoorg_flashcards',
          'lingwoorg_flashcards' => 'lingwoorg_flashcards',
        ),
        '#status' => 'default',
        '#conditions' => array(),
        '#actions' => array(
          '0' => array(
            '#type' => 'action',
            '#settings' => array(
              '#argument map' => array(
                'node' => 'node',
                'user' => 'flagging_user',
              ),
            ),
            '#name' => 'lingwoorg_flashcards_action_add_to_wial_deck',
            '#info' => array(
              'label' => 'Create flashcard in user\'s WIAL deck',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Entry',
                ),
                'user' => array(
                  'type' => 'user',
                  'label' => 'Użytkownik',
                ),
              ),
              'module' => 'lingwoorg_flashcard',
            ),
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
      'lingwoorg_flashcards_2' => array(
        '#type' => 'rule',
        '#set' => 'event_flag_unflagged_wial',
        '#label' => 'When an entry unflagged WIAL, its flashcard should be removed.',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'lingwoorg_flashcards',
          'lingwoorg_flashcards' => 'lingwoorg_flashcards',
        ),
        '#status' => 'default',
        '#conditions' => array(),
        '#actions' => array(
          '0' => array(
            '#type' => 'action',
            '#settings' => array(
              '#argument map' => array(
                'node' => 'node',
                'user' => 'flagging_user',
              ),
            ),
            '#name' => 'lingwoorg_flashcards_action_remove_from_wial_deck',
            '#info' => array(
              'label' => 'Remove flashcard from user\'s WIAL deck',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Entry',
                ),
                'user' => array(
                  'type' => 'user',
                  'label' => 'Użytkownik',
                ),
              ),
              'module' => 'lingwoorg_flashcard',
            ),
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
    ),
  );
}
