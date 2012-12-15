<?php
// $Id$

/**
 * @file
 * Provides Rules integration for the Votingapi module 

 * @addtogroup rules
 * @{
 */
 
//Load wrapper functions for each content type
module_load_include('inc', 'voting_rules', 'voting_rules.content-types');

/**
 * Implementation of hook_rules_data_type_info().
 */
function vote_rules_data_type_info() {
  return array(
    'vote' => array(
      'label' => t('Vote'),
      'class' => 'rules_data_type_vote',
      'savable' => FALSE,
      'identifiable' => FALSE,
    ),
    'vote_results' => array(
      'label' => t('Vote Results'),
      'class' => 'rules_data_type_vote_results',
      'savable' => FALSE,
      'identifiable' => FALSE,
    ),
  );
}

function voting_rules_content_types() {
  return array('node', 'user', 'comment'); 
}


/**
 * Implementation of hook_rules_event_info().
 */
function voting_rules_rules_event_info() {
  $events = array();
  $content_types = voting_rules_content_types(); 

    $events['voting_rules_insert'] = array(
      'label' => t('User votes on content of any type'),
      'module' => 'Votingapi',
      'arguments' => array(
        'vote' => array('type' => 'vote', 'label' => t('The vote')),
        'user' => array('type' => 'user', 'label' => t('The user who voted')),
      ),
      'redirect' => FALSE,
    );

    $events['voting_rules_delete'] = array(
      'label' => t('User deletes a vote on content of any type'),
      'module' => 'Votingapi',
      'arguments' => array(
        'vote' => array('type' => 'vote', 'label' => t('The vote')),
      ),
      'redirect' => FALSE,
    );
    
    $events['voting_rules_results'] = array(
      'label' => t('Votes are calculated for content of any type'),
      'module' => 'Votingapi',
      'arguments' => array(
        'vote_results' => array('type' => 'vote_results', 'label' => t('The results of the vote')),
      ),
      'redirect' => FALSE,
    );
  foreach ($content_types as $content_type) { 
    $events['voting_rules_insert_'. $content_type] = array(
      'label' => t('User votes on a @type', array('@type' => $content_type)),
      'module' => 'Votingapi',
      'arguments' => array(
        'vote' => array('type' => 'vote', 'label' => t('The vote')),
        $content_type => array('type' => $content_type, 'label' => t("The @type being voted on", array('@type' => $content_type))),
        'user' => array('type' => 'user', 'label' => t('The user who voted')),
      ),
      'redirect' => FALSE,
    );

    $events['voting_rules_delete_'. $content_type] = array(
      'label' => t('User deletes a vote on a @type', array('@type' => $content_type)),
      'module' => 'Votingapi',
      'arguments' => array(
        'vote' => array('type' => 'vote', 'label' => t('The vote')),
        $content_type => array('type' => $content_type, 'label' => t("The @type being voted on", array('@type' => $content_type))),
        'user' => array('type' => 'user', 'label' => t('The user who voted')),
      ),
      'redirect' => FALSE,
    );
    
    $events['voting_rules_results_'. $content_type] = array(
      'label' => t('Votes are calculated for a @type', array('@type' => $content_type)),
      'module' => 'Votingapi',
      'arguments' => array(
        'vote_results' => array('type' => 'vote_results', 'label' => t('The results of the vote')),
        $content_type => array('type' => $content_type, 'label' => t("The @type being voted on", array('@type' => $content_type))),
        'user' => array('type' => 'user', 'label' => t('The user who voted')),
      ),
      'redirect' => FALSE,
    );
  }
  return $events;
}

/**
 * Implementation of hook_rules_condition_info().
 */
function voting_rules_rules_condition_info() {
  foreach (voting_rules_content_types() as $content_type) {
    $conditions['voting_rules_condition_check_vote_value_'. $content_type] = array(
        'label' => t('Check the value of the vote on a @type', array('@type' => $content_type)),
        'arguments' => array(
          'vote' => array('type' => 'vote', 'label' => t('Vote')),
          $content_type => array('type' => $content_type, 'label' => ucwords($content_type) .' being voted on'),
        ),
        'module' => 'Votingapi',
      );
    $conditions['voting_rules_condition_check_vote_tag_'. $content_type] = array(
        'label' => t('Check the tag of a vote on a @type', array('@type' => $content_type)),
        'arguments' => array(
          'vote' => array('type' => 'vote', 'label' => t('Vote')),
          $content_type => array('type' => $content_type, 'label' => ucwords($content_type) .' being voted on'),
        ),
        'module' => 'Votingapi',
      );
    $conditions['voting_rules_condition_check_results_'. $content_type] = array(
        'label' => t('Evaluate the results of a vote on a @type', array('@type' => $content_type)),
        'arguments' => array(
          'vote_results' => array('type' => 'vote_results', 'label' => t('Results of the Vote')),
          $content_type => array('type' => $content_type, 'label' => ucwords($content_type) .' being voted on'),
        ),
        'module' => 'Votingapi',
      );
  }
  return $conditions;
}


/**
 * Condition: Check the tag of a vote 
 */
function voting_rules_condition_check_vote_tag($vote, $settings) {
  if (is_array($settings)) {
    switch($settings['operator']) {
      case '>':
        return ($vote['tag'] > $settings['tag']);
      break;
      case '>=':
        return ($vote['tag'] >= $settings['tag']);
      break;
      case '<':
        return ($vote['tag'] < $settings['tag']);
      break;
      case '<=':
        return ($vote['tag'] <= $settings['tag']);
      break;
      case '==':
        return ($vote['tag'] == $settings['tag']);
      break;
      case '!=':
        return ($vote['tag'] != $settings['tag']);
      break;
    } 
  }
}


/**
 *  Condition: Evaluate the results of the vote
 */
function voting_rules_condition_check_results($vote_results, $settings) {
  foreach ($vote_results as $data) {
    if ($data['function'] == $settings['function']) {
      return eval('return '. $data['value'] . $settings['operator'] . $settings['value'] .';'); 
    }
  }
}

/**
 * Condition: Check the value of an individual vote 
 */
function voting_rules_condition_check_vote_value($vote, $settings) {
  $string = 'return '. $vote['value'] . $settings['operator'] . $settings['value'] .';';
  return eval($string);
}

/**
 * Voting Rules condition check vote value configuration form
 */
function voting_rules_condition_check_vote_tag_form($settings, &$form) {
  $form['settings']['vote'] = array(
    '#value' => '<strong>'. t("Vote tag") .'</strong>',
  );
  $form['settings']['operator'] = voting_rules_operator_form_element($settings);
  $form['settings']['value'] = array(
    '#type' => 'textfield',
    '#default_value' => $settings['value'],
    '#required' => TRUE,
  );
}

/**
 * Voting Rules condition check vote value configuration form
 */
function voting_rules_condition_check_vote_value_form($settings, &$form) {
  $form['settings']['vote'] = array(
    '#value' => '<strong>'. t("Vote value") .'</strong>',
  );
  $form['settings']['operator'] = voting_rules_operator_form_element($settings);
  $form['settings']['value'] = array(
    '#type' => 'textfield',
    '#default_value' => $settings['value'],
    '#required' => TRUE,
  );
}

/**
 * Voting Rules condition check vote results configuration form
 */
function voting_rules_condition_check_results_form($settings, &$form) {
  $form['settings']['function'] = array(
    '#title' => t("Results"),
    '#type' => 'select',
    '#options' => array('average' => t("Average vote"), 'sum' => t("Sum of all votes"), 'count' => t("Number of votes")),
    '#default_value' => $settings['function'],
  );
  $form['settings']['operator'] = voting_rules_operator_form_element($settings);
  $form['settings']['value'] = array(
    '#title' => t("Value"),
    '#type' => 'textfield',
    '#default_value' => $settings['value'],
  );
}

/**
 * Helper function that returns the >,>=,<,<=,=== select form element
 */
function voting_rules_operator_form_element($settings) {
  return array(
    '#title' => t("Operation"),
    '#type' => 'select',
    '#default_value' => $settings['operator'],
    '#required' => TRUE,
    '#options' => array(
      '' => t("Select"), '>' => t("Greater than"), '>=' => t("Greater than / equal to"), '<' => t("Less than"),
      '<=' => t("Less than / equal to"), '===' => t("Equal to"),  '!=' => t("Not equal to")
    ),
  );
}

/**
 * Voting Rules check vote value configuration form validation function
 */
function voting_rules_condition_check_vote_value_validate($form, $form_state) {
  if (!is_numeric($form_state['values']['settings']['value'])) {
    form_set_error('settings][value', 'Value must be numeric.');
  }
}

/**
 * Voting Rules check vote result configuration form validation function
 */
function voting_rules_condition_check_results_validate($form, $form_state) {
  if (!is_numeric($form_state['values']['settings']['value'])) {
    form_set_error('settings][value', 'Value must be numeric.');
  }
}

/**
 * Implementation of hook_rules_action_info().
 */
function voting_rules_rules_action_info() {
  foreach (voting_rules_content_types() as $content_type) {
    $actions['voting_rules_action_cast_vote_'. $content_type] = array(
      'label' => t('Cast Vote on a @type', array('@type' => $content_type)),
      'arguments' => array(
        $content_type => array('type' => $content_type, 'label' => t("The @type being voted on", array('@type' => $content_type))),
      ),
      'module' => 'Votingapi',
    );
  }

  $actions['voting_rules_action_load_votes'] = array(
    'label' => t('Load Votes'),
    'arguments' => array(
    ),
    'new variables' => array(
      'votes_loaded' => array(
        'type' => 'votes',
        'label' => t('Votes Loaded from Database'),
        'save' => TRUE,
        'label callback' => 'voting_rules_action_load_votes_variable_label',
      ),  
    ),  
    'module' => 'Votingapi',
  );

  return $actions;
}

function voting_rules_action_load_votes_variable_label($settings) {
  //  return t('New content of type @type', array('@type' => $settings['type']));
  return t("Loaded votes");
}

/**
 *  Action: Cast Vote on a node
 */
function voting_rules_action_cast_vote_node($node, $settings) {
  if ($settings['vote_uid'] == 'current') {
    global $user;
    $settings['vote_uid'] = $user->uid;
  }
  $vote = array(
    'content_type' => 'node',
    'content_id'   => $node->nid,
    'value_type'   => $settings['vote_value_type'],
    'value'        => $settings['vote_value'],
    'uid'          => $settings['vote_uid'],
    'tag'          => $settings['vote_tag'],
  );
  $votes = array($vote);

  votingapi_set_votes($votes);

}

/**
 *  Set Votes Action Configuration Form
 */
function voting_rules_action_cast_vote_form($settings, &$form) {
  $form['settings']['vote_value_type'] = array(
    '#title' => t("Vote value type"),
    '#type' => 'textfield',
    '#default_value' => $settings['vote_value_type'] ? $settings['vote_value_type'] : 'percent',
    '#required' => TRUE,
    '#description' => t("i.e. 'percent', 'points'")
  );
  $form['settings']['vote_value'] = array(
    '#title' => t("Vote value"),
    '#type' => 'textfield',
    '#default_value' => $settings['vote_value'],
    '#required' => TRUE,
  );
  $form['settings']['vote_tag'] = array(
    '#title' => t("Tag"),
    '#type' => 'textfield',
    '#default_value' => $settings['vote_tag'] ? $settings['vote_tag'] : 'vote',
    '#required' => TRUE,
  );
  $form['settings']['vote_uid'] = array(
    '#title' => t("User Id casting the vote"),
    '#type' => 'textfield',
    '#default_value' => $settings['vote_uid'],
    '#description' => t("Use 'current' for uid of current user when event occurs"),
  );
  
}


/**
 * @}
 */
