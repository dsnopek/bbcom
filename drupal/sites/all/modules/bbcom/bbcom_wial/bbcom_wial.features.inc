<?php

/**
 * Implementation of hook_flag_default_flags().
 */
function bbcom_wial_flag_default_flags() {
  $flags = array();
  // Exported flag: "Words I Am Learning".
  $flags['wial'] = array(
    'content_type' => 'node',
    'title' => 'Words I Am Learning',
    'global' => '0',
    'types' => array(
      '0' => 'entry',
    ),
    'flag_short' => 'Add to "Words I Am Learning"',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Remove from "Words I Am Learning"',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        '0' => '2',
        '1' => '8',
      ),
      'unflag' => array(
        '0' => '2',
        '1' => '8',
      ),
    ),
    'weight' => 0,
    'show_on_page' => 1,
    'show_on_teaser' => 1,
    'show_on_form' => 0,
    'access_author' => '',
    'i18n' => '0',
    'flag_confirmation' => '',
    'unflag_confirmation' => '',
    'module' => 'bbcom_wial',
    'locked' => array(
      '0' => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;

}

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
            '#type' => 'condition',
            '#settings' => array(
              'code' => 'return isset($user->bbcom_wial[\'auto_add\']) ? (bool)$user->bbcom_wial[\'auto_add\'] : TRUE;',
              'vars' => array(
                '0' => 'user',
              ),
            ),
            '#name' => 'rules_condition_custom_php',
            '#info' => array(
              'label' => 'The user wants words to be automatically added',
              'label callback' => FALSE,
              'module' => 'PHP',
              'eval input' => array(
                '0' => 'code',
              ),
            ),
          ),
        ),
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

/**
 * Implementation of hook_views_api().
 */
function bbcom_wial_views_api() {
  return array(
    'api' => '2',
  );
}
