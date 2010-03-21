<?php

/**
 * Helper to implementation of hook_rules_defaults().
 */
function _lingwoorg_wial_rules_defaults() {
  return array(
    'rules' => array(
      'lingwoorg_wial_1' => array(
        '#type' => 'rule',
        '#set' => 'event_lingwo_korpus_lookup',
        '#label' => 'When a user looks up a word from the annotation reader, it should be added to their WIAL list.',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          'lingwoorg_wial' => 'lingwoorg_wial',
        ),
        '#status' => 'default',
        '#conditions' => array(),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Flag The node looked up., under "Words I Am Learning"',
              'base' => 'flag_rules_action_flag',
              'label callback' => 'flag_rules_action_flag_label',
              'arguments' => array(
                'flag' => array(
                  'type' => 'flag',
                  'label' => 'Flag',
                  'flag_type' => 'node',
                ),
                'object' => array(
                  'type' => 'node',
                  'label' => 'Flagged content',
                ),
                'flagging_user' => array(
                  'type' => 'user',
                  'label' => 'User on whose behalf to flag',
                  'description' => 'For non-global flags, this is the user on whose behalf to flag the object. In addition, if checked below, the access permissions to the flag are checked against this user.',
                ),
              ),
              'module' => 'Flag',
            ),
            '#name' => 'flag_rules_action_flag_node',
            '#settings' => array(
              'flag' => 'wial',
              'permission_check' => 1,
              '#argument map' => array(
                'object' => 'node',
                'flagging_user' => 'user',
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
