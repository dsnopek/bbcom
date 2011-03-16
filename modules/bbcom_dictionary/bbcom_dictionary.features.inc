<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function bbcom_dictionary_context_default_contexts() {
  $items = array();

  $items['bbcom-section-dictionary'] = array(
    'namespace' => 'bbcom',
    'attribute' => 'section',
    'value' => 'dictionary',
    'description' => 'Working in the dictionary',
    'node' => array(
      '0' => 'entry',
    ),
    'path' => array(
      'dictionary' => 'dictionary',
      'dictionary/*' => 'dictionary/*',
    ),
    'menu' => 'dictionary',
    'block' => array(
      'lingwo_entry_0' => array(
        'module' => 'lingwo_entry',
        'delta' => '0',
        'weight' => 20,
        'region' => 'header',
        'status' => '0',
        'label' => 'Lingwo Dictionary: Search block',
        'type' => 'context_ui',
      ),
      'bbcom_dictionary_0' => array(
        'module' => 'bbcom_dictionary',
        'delta' => '0',
        'weight' => -21,
        'region' => 'right',
        'status' => '0',
        'label' => 'BiblioBird.com Dictionary: Add Content Block',
        'type' => 'context_ui',
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_ctools_plugin_api().
 */
function bbcom_dictionary_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_rules_defaults().
 */
function bbcom_dictionary_rules_defaults() {
  return array(
    'rules' => array(
      'bbcom_dictionary_1' => array(
        '#type' => 'rule',
        '#set' => 'event_comment_publish',
        '#label' => 'Give users points for commenting on dictionary entries',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'bbcom_dictionary',
          '1' => 'gamification',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Commented content is Dictionary entry',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
            '#name' => 'rules_condition_content_is_type',
            '#settings' => array(
              'type' => array(
                'entry' => 'entry',
              ),
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#type' => 'condition',
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#type' => 'action',
            '#settings' => array(
              'points' => '2',
              'description' => 'Wrote comment on dictionary entry',
              'operation' => 'comment',
              'entity_type' => 'comment',
              'entity_id' => '[comment:comment-cid]',
              'tid' => '6',
              '#argument map' => array(
                'user' => 'comment_author',
              ),
              '#eval input' => array(
                'token_rules_input_evaluator' => array(
                  'entity_id' => array(
                    '0' => 'comment',
                  ),
                ),
              ),
            ),
            '#name' => 'userpoints_action_grant_points',
            '#info' => array(
              'label' => 'Grant points to a user',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'points',
                '1' => 'entity_id',
              ),
            ),
          ),
        ),
        '#version' => 6003,
      ),
      'bbcom_dictionary_2' => array(
        '#type' => 'rule',
        '#set' => 'event_voting_rules_insert_node',
        '#label' => 'Give users points for voting on dictionary entries',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'bbcom_dictionary',
          '1' => 'gamification',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'The node being voted on is Dictionary entry',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
            '#name' => 'rules_condition_content_is_type',
            '#settings' => array(
              'type' => array(
                'entry' => 'entry',
              ),
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#type' => 'condition',
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#type' => 'action',
            '#settings' => array(
              'username' => '',
              'userid' => '[vote:uid]',
              '#argument map' => array(
                'user_loaded' => 'user_voting',
              ),
              '#eval input' => array(
                'token_rules_input_evaluator' => array(
                  'userid' => array(
                    '0' => 'vote',
                  ),
                ),
              ),
            ),
            '#name' => 'rules_action_load_user',
            '#info' => array(
              'label' => 'Load a user account',
              'new variables' => array(
                'user_voting' => array(
                  'label' => 'voting user',
                  'label callback' => FALSE,
                  'type' => 'user',
                ),
              ),
              'module' => 'User',
              'eval input' => array(
                '0' => 'userid',
                '1' => 'username',
              ),
            ),
            '#weight' => -10,
          ),
          '1' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Grant points to a user',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'points',
                '1' => 'entity_id',
              ),
            ),
            '#name' => 'userpoints_action_grant_points',
            '#settings' => array(
              'points' => '1',
              'description' => 'Voted on a dictionary entry',
              'operation' => 'entry',
              'entity_type' => 'node',
              'entity_id' => '[node:nid]',
              'tid' => '6',
              '#argument map' => array(
                'user' => 'user_voting',
              ),
              '#eval input' => array(
                'token_rules_input_evaluator' => array(
                  'entity_id' => array(
                    '0' => 'node',
                  ),
                ),
              ),
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
      'bbcom_dictionary_3' => array(
        '#type' => 'rule',
        '#set' => 'event_node_update',
        '#label' => 'Give users points for editing dictionary entries',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'bbcom_dictionary',
          '1' => 'gamification',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#type' => 'condition',
            '#settings' => array(
              'type' => array(
                'entry' => 'entry',
              ),
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#name' => 'rules_condition_content_is_type',
            '#info' => array(
              'label' => 'Updated content is Dictionary entry',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
            '#weight' => 0,
          ),
          '1' => array(
            '#weight' => 0,
            '#type' => 'condition',
            '#settings' => array(
              'code' => 'if ($node->lingwo_senses != $node_unchanged->lingwo_senses) {
  return TRUE;
}
if ($node->lingwo_fields != $node_unchanged->lingwo_fields) {
  return TRUE;
}
return FALSE;',
              'vars' => array(
                '0' => 'node',
                '1' => 'node_unchanged',
              ),
            ),
            '#name' => 'rules_condition_custom_php',
            '#info' => array(
              'label' => 'User has changed the content of the entry',
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
              'label' => 'Grant points to a user',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'points',
                '1' => 'entity_id',
              ),
            ),
            '#name' => 'userpoints_action_grant_points',
            '#settings' => array(
              'points' => '5',
              'description' => 'Changed dictionary entry',
              'operation' => 'entry',
              'entity_type' => 'node',
              'entity_id' => '[node:nid]',
              'tid' => '6',
              '#argument map' => array(
                'user' => 'user',
              ),
              '#eval input' => array(
                'token_rules_input_evaluator' => array(
                  'entity_id' => array(
                    '0' => 'node',
                  ),
                ),
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
function bbcom_dictionary_views_api() {
  return array(
    'api' => '2',
  );
}
