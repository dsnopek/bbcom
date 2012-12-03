<?php
/**
 * @file
 *   Contains wrapper functions for each content type
 *   The following ugliness is unavoidable without changing Rules, afaik
 *
 */

/**
 * Condition: Check the tag of a vote on a node 
 */
function voting_rules_condition_check_vote_tag_node($vote, $node, $settings) {
  return voting_rules_condition_check_vote_tag($vote, $settings); 
}

/**
 * Condition: Check the tag of a vote on a user 
 */
function voting_rules_condition_check_vote_tag_user($vote, $user, $settings) {
  return voting_rules_condition_check_vote_tag($vote, $settings);
}

/**
 * Condition: Check the tag of a vote on a comment 
 */
function voting_rules_condition_check_vote_tag_comment($vote, $comment, $settings) {
  return voting_rules_condition_check_vote_tag($vote, $settings);
}


/**
 *  Condition: Evaluate the results of the vote on a node
 */
function voting_rules_condition_check_results_node($vote_results, $node, $settings) {
  return voting_rules_condition_check_results($vote_results, $settings);
}

/**
 *  Condition: Evaluate the results of the vote on a user 
 */
function voting_rules_condition_check_results_user($vote_results, $user, $settings) {
  return voting_rules_condition_check_results($vote_results, $settings);
}

/**
 *  Condition: Evaluate the results of the vote on a comment
 */
function voting_rules_condition_check_results_comment($vote_results, $comment, $settings) {
  return voting_rules_condition_check_results($vote_results, $settings);
}


/**
 * Condition: Check the value of an individual vote on a node 
 */
function voting_rules_condition_check_vote_value_node($vote, $node, $settings) {
  return voting_rules_condition_check_vote_value($vote, $settings); 
}

/**
 * Condition: Check the value of an individual vote on a user 
 */
function voting_rules_condition_check_vote_value_user($vote, $user, $settings) {
  return voting_rules_condition_check_vote_value($vote, $settings); 
}

/**
 * Condition: Check the value of an individual vote on a comment 
 */
function voting_rules_condition_check_vote_value_comment($vote, $comment, $settings) {
  return voting_rules_condition_check_vote_value($vote, $settings); 
}


/**
 * Voting Rules condition check vote value configuration form for nodes
 */
function voting_rules_condition_check_vote_value_node_form($settings, &$form) {
  return voting_rules_condition_check_vote_value_form($settings, &$form);
}

/**
 * Voting Rules condition check vote value configuration form for users 
 */
function voting_rules_condition_check_vote_value_users_form($settings, &$form) {
  return voting_rules_condition_check_vote_value_form($settings, &$form);
}

/**
 * Voting Rules condition check vote value configuration form for comments
 */
function voting_rules_condition_check_vote_value_comments_form($settings, &$form) {
  return voting_rules_condition_check_vote_value_form($settings, &$form);
}

/**
 * Voting Rules condition check vote results configuration form for nodes
 */
function voting_rules_condition_check_results_node_form($settings, &$form) {
  return voting_rules_condition_check_results_form($settings, &$form); 
}

/**
 * Voting Rules condition check vote results configuration form for users
 */
function voting_rules_condition_check_results_user_form($settings, &$form) {
  return voting_rules_condition_check_results_form($settings, &$form); 
}

/**
 * Voting Rules condition check vote results configuration form for comments
 */
function voting_rules_condition_check_results_comment_form($settings, &$form) {
  return voting_rules_condition_check_results_form($settings, &$form); 
}

/**
 * Voting Rules condition check vote tag configuration form for nodes
 */
function voting_rules_condition_check_vote_tag_node_form($settings, &$form) {
  return voting_rules_condition_check_vote_tag_form($settings, &$form);
}

/*
 * Voting Rules condition check vote tag configuration form for users 
 */
function voting_rules_condition_check_vote_tag_user_form($settings, &$form) {
  return voting_rules_condition_check_vote_tag_form($settings, &$form);
}

/*
 * Voting Rules condition check vote tag configuration form for comments
 */
function voting_rules_condition_check_vote_tag_comment_form($settings, &$form) {
  return voting_rules_condition_check_vote_tag_form($settings, &$form);
}

/**
 * Voting Rules check vote value configuration form validation function for nodes
 */
function voting_rules_condition_check_vote_value_node_validate($form, $form_state) {
  return voting_rules_condition_check_vote_value_validate($form, $form_state); 
}

/**
 * Voting Rules check vote value configuration form validation function for users
 */
function voting_rules_condition_check_vote_value_user_validate($form, $form_state) {
  return voting_rules_condition_check_vote_value_validate($form, $form_state);
}

/**
 * Voting Rules check vote value configuration form validation function for comments
 */
function voting_rules_condition_check_vote_value_comment_validate($form, $form_state) {
  return voting_rules_condition_check_vote_value_validate($form, $form_state);
}

//ACTIONS

/**
 *  Set Votes Action Configuration Form for nodes
 */
function voting_rules_action_cast_vote_node_form($settings, &$form) {
  return voting_rules_action_cast_vote_form($settings, &$form); 
}

/**
 *  Set Votes Action Configuration Form for users 
 */
function voting_rules_action_cast_vote_user_form($settings, &$form) {
  return voting_rules_action_cast_vote_form($settings, &$form); 
}

/**
 *  Set Votes Action Configuration Form for nodes
 */
function voting_rules_action_cast_vote_comment_form($settings, &$form) {
  return voting_rules_action_cast_vote_form($settings, &$form); 
}

