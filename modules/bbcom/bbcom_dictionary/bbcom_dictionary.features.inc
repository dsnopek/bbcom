<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function bbcom_dictionary_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_rules_defaults().
 */
function bbcom_dictionary_rules_defaults() {
  return array(
    'rules' => array(
      'bbcom_dictionary_7' => array(
        '#type' => 'rule',
        '#set' => 'event_userpoints_event_points_awarded',
        '#label' => 'When user gets 50 editor points, upgrade their role',
        '#active' => 1,
        '#weight' => '5',
        '#categories' => array(
          '0' => 'bbcom_dictionary',
          '1' => 'gamification',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Points >= 50',
              'label callback' => FALSE,
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'amount',
              ),
            ),
            '#name' => 'userpoints_rules_amount',
            '#settings' => array(
              'type' => '6',
              'amount' => '50',
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#type' => 'condition',
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#type' => 'action',
            '#settings' => array(
              'roles' => array(
                '0' => 10,
              ),
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#name' => 'rules_action_user_addrole',
            '#info' => array(
              'label' => 'Add \'editor1\' role',
              'label callback' => FALSE,
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User whos roles should be changed',
                ),
              ),
              'module' => 'User',
            ),
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
      'bbcom_dictionary_8' => array(
        '#type' => 'rule',
        '#set' => 'event_userpoints_event_points_awarded',
        '#label' => 'When user drops below 50 editor points, downgrade their role',
        '#active' => 1,
        '#weight' => '5',
        '#categories' => array(
          '0' => 'bbcom_dictionary',
          '1' => 'gamification',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#info' => array(
              'label' => 'Points >= 50',
              'label callback' => FALSE,
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'amount',
              ),
            ),
            '#weight' => 0,
            '#name' => 'userpoints_rules_amount',
            '#settings' => array(
              'type' => '6',
              'amount' => '50',
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#type' => 'condition',
            '#negate' => 1,
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#type' => 'action',
            '#settings' => array(
              'roles' => array(
                '0' => 10,
              ),
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#name' => 'rules_action_user_removerole',
            '#info' => array(
              'label' => 'Remove \'editor1\' role',
              'label callback' => FALSE,
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User whos roles should be changed',
                ),
              ),
              'module' => 'User',
            ),
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
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
      'bbcom_dictionary_4' => array(
        '#type' => 'rule',
        '#set' => 'event_voting_rules_delete_node',
        '#label' => 'Retract points when users delete a vote',
        '#active' => 1,
        '#weight' => '-1',
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
              'label' => 'The node being voted on is Dictionary entry',
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
        ),
        '#actions' => array(
          '0' => array(
            '#weight' => -10,
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
            '#name' => 'rules_action_load_user',
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
            '#type' => 'action',
          ),
          '1' => array(
            '#type' => 'action',
            '#settings' => array(
              'points' => '-1',
              'description' => 'Retracted vote on dictionary entry',
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
            '#name' => 'userpoints_action_grant_points',
            '#info' => array(
              'label' => 'Retract points from a user',
              'label callback' => FALSE,
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
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
      'bbcom_dictionary_5' => array(
        '#type' => 'rule',
        '#set' => 'event_comment_delete',
        '#label' => 'Retract points when comment on dictionary entry is deleted',
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
              'label' => 'Commented content is Dictionary entry',
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
              'points' => '-2',
              'description' => 'Deleted comment on dictionary entry',
              'operation' => 'delete',
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
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
      'bbcom_dictionary_6' => array(
        '#type' => 'rule',
        '#set' => 'event_node_delete_revision',
        '#label' => 'Retract points when dictionary entry revision is deleted',
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
              'label' => 'The deleted revision is Dictionary entry',
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
        ),
        '#actions' => array(
          '0' => array(
            '#weight' => -10,
            '#info' => array(
              'label' => 'Load a user account',
              'new variables' => array(
                'user_revision' => array(
                  'label' => 'user who created the revision',
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
            '#name' => 'rules_action_load_user',
            '#settings' => array(
              'username' => '',
              'userid' => '<?php echo $node->revision_uid; ?>',
              '#argument map' => array(
                'user_loaded' => 'user_revision',
              ),
              '#eval input' => array(
                'rules_input_evaluator_php' => array(
                  'userid' => array(
                    '0' => 'node',
                  ),
                ),
              ),
            ),
            '#type' => 'action',
          ),
          '1' => array(
            '#type' => 'action',
            '#settings' => array(
              'points' => '-5',
              'description' => 'Reverted changes to dictionary entry',
              'operation' => 'entry',
              'entity_type' => 'node',
              'entity_id' => '[node:nid]',
              'tid' => '6',
              '#argument map' => array(
                'user' => 'user_revision',
              ),
              '#eval input' => array(
                'token_rules_input_evaluator' => array(
                  'entity_id' => array(
                    '0' => 'node',
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
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
      'bbcom_dictionary_9' => array(
        '#type' => 'rule',
        '#set' => 'event_userpoints_event_points_awarded',
        '#label' => 'When user drops below 200 editor points, downgrade their role',
        '#active' => 1,
        '#weight' => '4',
        '#categories' => array(
          '0' => 'bbcom_dictionary',
          '1' => 'gamification',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#type' => 'condition',
            '#weight' => 0,
            '#negate' => 1,
            '#settings' => array(
              'type' => '6',
              'amount' => '200',
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#name' => 'userpoints_rules_amount',
            '#info' => array(
              'label' => 'Points >= 200',
              'label callback' => FALSE,
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'amount',
              ),
            ),
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Remove \'editor2\' role',
              'label callback' => FALSE,
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User whos roles should be changed',
                ),
              ),
              'module' => 'User',
            ),
            '#name' => 'rules_action_user_removerole',
            '#settings' => array(
              'roles' => array(
                '0' => 12,
              ),
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#type' => 'action',
          ),
          '1' => array(
            '#type' => 'action',
            '#settings' => array(
              'roles' => array(
                '0' => 10,
              ),
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#name' => 'rules_action_user_addrole',
            '#info' => array(
              'label' => 'Add \'editor1\' role',
              'label callback' => FALSE,
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User whos roles should be changed',
                ),
              ),
              'module' => 'User',
            ),
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
      'bbcom_dictionary_10' => array(
        '#type' => 'rule',
        '#set' => 'event_userpoints_event_points_awarded',
        '#label' => 'When user gets 200 editor points, upgrade their role',
        '#active' => 1,
        '#weight' => '6',
        '#categories' => array(
          '0' => 'bbcom_dictionary',
          '1' => 'gamification',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#info' => array(
              'label' => 'Points >= 200',
              'label callback' => FALSE,
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'amount',
              ),
            ),
            '#name' => 'userpoints_rules_amount',
            '#settings' => array(
              'type' => '6',
              'amount' => '200',
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#type' => 'condition',
            '#weight' => 0,
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Add \'editor2\' role',
              'label callback' => FALSE,
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User whos roles should be changed',
                ),
              ),
              'module' => 'User',
            ),
            '#name' => 'rules_action_user_addrole',
            '#settings' => array(
              'roles' => array(
                '0' => 12,
              ),
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#type' => 'action',
          ),
          '1' => array(
            '#type' => 'action',
            '#settings' => array(
              'roles' => array(
                '0' => 10,
              ),
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#name' => 'rules_action_user_removerole',
            '#info' => array(
              'label' => 'Remove \'editor1\' role',
              'label callback' => FALSE,
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User whos roles should be changed',
                ),
              ),
              'module' => 'User',
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
function bbcom_dictionary_views_api() {
  return array(
    'api' => '2',
  );
}
