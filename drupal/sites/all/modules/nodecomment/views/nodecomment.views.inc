<?php
// $Id: nodecomment.views.inc,v 1.1.2.1.2.6 2009/06/15 21:58:03 merlinofchaos Exp $
/**
 * @file
 * Provide views data and handlers for nodecomment.module
 */

/**
 * Implementation of hook_views_data()
 */
function nodecomment_views_data() {
  // since comments are nodes we keep with the Node group
  $data['node_comments']['table']['group']  = t('Node comment');

	// ----------------------------------------------------------------
  // node table -- basic table information.

	$data['node_comments']['table']['join'] = array(
		// node_comments links to node
		'node' => array(
			'left_field' => 'nid',
			'field' => 'cid',
		),
	);

	// ----------------------------------------------------------------
  // node_comments table -- fields

  $data['node_comments']['cid'] = array(
    'title' => t('Nid'),
    'help' => t('The node ID of the comment.'),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['node_comments']['nid'] = array(
    'title' => t('Original Parent Nid'),
    'help' => t('The original node the comment is a reply to.'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Node'),
    ),
  );

  $data['node_comments']['nid_or_self'] = array(
    'title' => t('Original Parent Nid or Self'),
    'help' => t('The original node the comment is a reply to, or the original node. This allows searching across nodes that may or may not be comments.'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'real field' => 'nid',
      'handler' => 'nodecomment_handler_relationship_nid_or_self',
      'label' => t('Node'),
    ),
  );

  $data['node_comments']['pid'] = array(
    'title' => t('Parent Nid'),
    'help' => t('The node of the parent comment. Could be another comment.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'relationship' => array(
      'title' => t('Parent comment'),
      'help' => t('The parent comment.'),
      'base' => 'comments',
      'field' => 'cid',
      'handler' => 'views_handler_relationship',
      'label' => t('Parent comment'),
    ),
  );

  $data['node_comments']['name'] = array(
    'title' => t('Author'),
    'help' => t('The name of the poster. This does not query the user table, it includes only data in the node_comment table.'),
    'field' => array(
      'handler' => 'views_handler_field_username_comment',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['node_comments']['homepage'] = array(
    'title' => t("Author's website"),
    'help' => t("The website address of the comment's author. Can be a link. The homepage can also be linked with the Name field. Will be empty if posted by a registered user."),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['node_comments']['thread'] = array(
    'field' => array(
      'title' => t('Depth'),
      'help' => t('Display the depth of the comment if it is threaded.'),
      'handler' => 'views_handler_field_comment_depth',
    ),
    'sort' => array(
      'title' => t('Thread'),
      'help' => t('Sort by the threaded order. This will keep child comments together with their parents.'),
      'handler' => 'views_handler_sort_comment_thread',
    ),
  );

  // link to reply to comment
  $data['node_comments']['replyto_comment'] = array(
    'field' => array(
      'title' => t('Reply-to link'),
      'help' => t('Provide a simple link to reply to the comment.'),
      'handler' => 'views_handler_field_comment_link_reply',
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_data()
 */
function nodecomment_views_data_alter(&$data) {
  $data['node']['nodecomment_link'] = array(
    'field' => array(
      'title' => t('Node comment link'),
      'help' => t('If the node is a comment, provide a link to the parent node at the right location. Otherwise provide a normal link to the node.'),
      'handler' => 'nodecomment_handler_field_link',
    ),
  );
  $data['node']['nodecomment_author'] = array(
    'field' => array(
      'title' => t('Node comment author'),
      'help' => t('Provide a link to the author of the node. This works properly with node comment where the author may not be an actual user.'),
      'handler' => 'nodecomment_handler_field_author',
    ),
  );

  // replace comment.module's "new comments" handler with ours:
  $data['node']['new_comments']['field']['handler'] = 'nodecomment_handler_field_node_new_comments';

  // we may also need to make our own version of uid_touch.
}

/**
 * Implementation of hook_views_plugins
 */
function nodecomment_views_plugins() {
  return array(
    'display' => array(
      'nodecomment_comments' => array(
        'title' => t('Nodecomments'),
        'handler' => 'nodecomment_plugin_display_comments',
        'path' => drupal_get_path('module', 'nodecomment') .'/views',
        'theme' => 'views_view',
        'theme path' => drupal_get_path('module', 'views') .'/theme',
        'help' => t('Display a view in the "comments" location'),
        'use ajax' => TRUE,
        'use pager' => TRUE,
        'accept attachments' => TRUE,
        'admin' => t('Nodecomments'),
        'help topic' => 'display-nodecomments',
      ),
    ),
    'style' => array(
      'nodecomment_threaded' => array(
        'path' => drupal_get_path('module', 'nodecomment') .'/views',
        'theme path' => drupal_get_path('module', 'nodecomment') .'/views',
        'title' => t('Node comments threaded'),
        'help' => t('Display the comment with a threaded comment view.'),
        'handler' => 'nodecomment_plugin_style_threaded',
        'theme' => 'nodecomment_threaded',
        'uses row plugin' => TRUE,
        'type' => 'normal',
      ),
    ),
    'cache' => array(
      'nodecomment_comments' => array(
        'path' => drupal_get_path('module', 'nodecomment') .'/views',
        'title' => t('Nodecomment thread'),
        'help' => t('Cache a single thread of nodecomments based on a nid.'),
        'handler' => 'nodecomment_plugin_cache_comments',
        'uses options' => TRUE,
        'help topic' => 'cache-nodecomment',
      ),
    ),
  );
}

/**
 * Register handlers
 */
function nodecomment_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'nodecomment') .'/views',
    ),
    'handlers' => array(
      'views_handler_field_username_comment' => array(
        'parent' => 'views_handler_field',
      ),
      'views_handler_field_comment_depth' => array(
        'parent' => 'views_handler_field'
      ),
      'views_handler_field_username_comment' => array(
        'parent' => 'views_handler_field'
      ),
      'views_handler_field_comment_link' => array(
        'parent' => 'views_handler_field'
      ),
      'views_handler_field_comment_link_reply' => array(
        'parent' => 'views_handler_field_comment_link'
      ),
      'views_handler_sort_comment_thread' => array(
        'parent' => 'views_handler_sort'
      ),
      'nodecomment_handler_relationship_nid_or_self' => array(
        'parent' => 'views_handler_relationship'
      ),
      'nodecomment_handler_field_link' => array(
        'parent' => 'views_handler_field_node_link'
      ),
      'nodecomment_handler_field_author' => array(
        'parent' => 'views_handler_field_user_name'
      ),
      'nodecomment_handler_field_node_new_comments' => array(
        'parent' => 'views_handler_field_node_new_comments'
      ),
    ),
  );
}
