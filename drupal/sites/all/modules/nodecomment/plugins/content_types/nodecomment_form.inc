<?php

/**
 * Implementation of specially named hook_ctools_content_types()
 */
function nodecomment_nodecomment_form_ctools_content_types() {
    return array(
      'single' => TRUE,
      'title' => t('Nodecomment form'),
      'icon' => 'icon_node.png',
      'description' => t('Form to add a new comment (using nodecomment or comment depending on the content type configuration).'),
      'required context' => new ctools_context_required(t('Node'), 'node'),
      'category' => t('Node'),
    );
}

function nodecomment_nodecomment_form_content_type_render($subtype, $conf, $panel_args, $context) {
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;
  $output = '';
  $block = new stdClass();
  
  // Use comment.module with node_comment_form.inc or nodecomment with custom code depending on what the node requires
  if (module_exists('comment') && !empty($node->comment)) {
    ctools_get_content_type('node_comment_form');
    return ctools_node_comment_form_content_type_render($subtype, $conf, $panel_args, $context);
  }
  else if (module_exists('nodecomment') && !empty($node->node_comment)) {
    // find out what node type serves as the comment type for this node
    $comment_type = variable_get('node_comment_type_'. $node->type, variable_get('node_default_comment_type', ''));
    $comment_type_info = node_get_types('type', $comment_type);
    $block->title = t('Add @type', array('@type' => $comment_type_info->name));
    
    if (empty($node)) {
      $output .= t('@type form here.', array('@type' => $comment_type_info->name));
    }
    else {
      if (user_access("create $comment_type content") && $node->node_comment == COMMENT_NODE_READ_WRITE) {
        ctools_include('form');
        global $user;
        
        // create basic comment node 
        $comment = array(
  	      'uid' => $user->uid,
  	      'name' => $user->name,
  	      'type' => $comment_type,
  	      'comment_target_nid' => $node->nid,
  	    );
        
        $form_state = array(
          'ctools comment alter' => 'nodecomment',
          'args' => array($comment),
        );
        
        // make sure node.pages.inc is loaded
        module_load_include('inc', 'node', 'node.pages');
        
        $output .= ctools_build_form("{$comment_type}_node_form", $form_state);
      }
    }
  }
  
  $block->module = 'nodecomment';
  $block->delta  = $node->nid;
  $block->content = $output;

  return $block;
}

function nodecomment_nodecomment_form_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" comment form', array('@s' => $context->identifier));
}

function nodecomment_nodecomment_form_content_type_edit_form(&$form, &$form_state) {
  // provide a blank form so we have a place to have context setting.
}

/**
 * Alter the comment form to get a little more control over it.
 */
function nodecomment_nodecomment_form_form_alter(&$form, &$form_state) {
  if ($form_state['ctools comment alter'] == 'nodecomment') {
    // force the form to post back to wherever we are.
    $url = parse_url($_GET['q']);
    $form['#submit'][] = 'nodecomment_nodecomment_form_submit';
  }
  else if (!empty($form_state['ctools comment alter'])){
    ctools_get_content_type('node_comment_form');
    ctools_form_comment_form_alter($form, $form_state);
  }
}

function nodecomment_nodecomment_form_submit(&$form, &$form_state) {
  $form_state['redirect'][0] = $_GET['q'];
}
