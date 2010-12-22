<?php

/**
 * Implementation of hook_rules_defaults().
 */
function bbcom_flashcards_rules_defaults() {
  return array(
    'rules' => array(
      'bbcom_flashcards_1' => array(
        '#type' => 'rule',
        '#set' => 'event_flag_flagged_wial',
        '#label' => 'When an entry flagged WIAL, a flashcard should be created.',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'bbcom_flashcards',
        ),
        '#status' => 'default',
        '#conditions' => array(),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
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
              'module' => 'bbcom_flashcards',
            ),
            '#name' => 'bbcom_flashcards_action_add_to_wial_deck',
            '#settings' => array(
              '#argument map' => array(
                'node' => 'node',
                'user' => 'flagging_user',
              ),
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
      'bbcom_flashcards_2' => array(
        '#type' => 'rule',
        '#set' => 'event_flag_unflagged_wial',
        '#label' => 'When an entry unflagged WIAL, its flashcard should be removed.',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'bbcom_flashcards',
        ),
        '#status' => 'default',
        '#conditions' => array(),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
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
              'module' => 'bbcom_flashcards',
            ),
            '#name' => 'bbcom_flashcards_action_remove_from_wial_deck',
            '#settings' => array(
              '#argument map' => array(
                'node' => 'node',
                'user' => 'flagging_user',
              ),
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
    ),
  );
}
