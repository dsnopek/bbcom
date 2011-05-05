<?php

/**
 * Implementation of hook_rules_defaults().
 */
function bbcom_wial_rules_defaults() {
  return array(
    'rules' => array(
      'bbcom_wial_1' => array(
        '#type' => 'rule',
        '#set' => 'event_lingwo_korpus_lookup',
        '#label' => 'When a user looks up a word from the annotation reader, it should be added to their WIAL list.',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          'bbcom_wial' => 'bbcom_wial',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'The user wants words to be automatically added',
              'label callback' => FALSE,
              'module' => 'PHP',
              'eval input' => array(
                '0' => 'code',
              ),
            ),
            '#name' => 'rules_condition_custom_php',
            '#settings' => array(
              'code' => '$val = isset($user->bbcom_wial[\'auto_add\']) ? (bool)$user->bbcom_wial[\'auto_add\'] : TRUE;
drupal_set_message(\'auto_add: \' . $val);
return isset($user->bbcom_wial[\'auto_add\']) ? (bool)$user->bbcom_wial[\'auto_add\'] : TRUE;',
              'vars' => array(
                '0' => 'user',
              ),
            ),
            '#type' => 'condition',
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#type' => 'action',
            '#settings' => array(
              'flag' => 'wial',
              'permission_check' => 1,
              '#argument map' => array(
                'object' => 'node',
                'flagging_user' => 'user',
              ),
            ),
            '#name' => 'flag_rules_action_flag_node',
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
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
    ),
  );
}

/**
 * Implementation of hook_views_api().
 */
function bbcom_wial_views_api() {
  return array(
    'api' => '2',
  );
}
