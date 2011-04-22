<?php
// $Id: nodecomment.convert.inc,v 1.1.2.3 2010/06/27 16:19:10 crea Exp $

/**
 * @file
 * Contains comment conversion pages and routines for the Node Comments module.
 */

/**
 * Get the number of comments (or nodes) that need to be converted.
 *
 * @param $node_type
 *   The node type for the nodes that are being commented on.
 * @param $previous_comment_type
 *   The node type previously used as a comment. Use an empty string to indicate
 *   the previous type was normal Drupal comments.
 */
function nodecomment_convert_count($node_type, $previous_comment_type = '') {
  if ($previous_comment_type == '') {
    return db_result(db_query("SELECT count(*) FROM {comments} c INNER JOIN {node} n ON c.nid = n.nid WHERE n.type = '%s'", $node_type));
  }
  else {
    return db_result(db_query("SELECT count(*) FROM {node_comments} c INNER JOIN {node} n ON c.nid = n.nid INNER JOIN {node} nc ON c.cid = nc.nid WHERE n.type = '%s' AND nc.type = '%s'", $node_type, $previous_comment_type));
  }
}

/**
 * Get the total of all comment conversions that need to take place.
 *
 * @param $convert_counts
 *   If the list of conversions has already been retrieved from the
 *   nodecomment_convert_counts() function, the values may be passed in here
 *   for efficiency.
 */
function nodecomment_convert_count_total($convert_counts = NULL) {
  if (!isset($convert_counts)) {
    $convert_counts = nodecomment_convert_counts();
  }

  $total = 0;
  foreach ($convert_counts as $node_type => $comment_types) {
    foreach ($comment_types as $comment_type => $count) {
      $total += $count;
    }
  }

  return $total;
}

/**
 * Get a list of all conversions that need to take place for all types.
 */
function nodecomment_convert_counts() {
  $convert_counts = array();
  $node_types = node_get_types('names');
  $comment_types = array_merge(array('' => 'normal comments'), $node_types);
  foreach ($node_types as $node_type => $node_type_name) {
    $current_comment_type = nodecomment_get_comment_type($node_type);
    foreach ($comment_types as $comment_type => $comment_type_name) {
      if ($comment_type != $current_comment_type && $count = nodecomment_convert_count($node_type, $comment_type)) {
        $convert_counts[$node_type][$comment_type] = $count;
      }
    }
  }
  return $convert_counts;
}

/**
 * Menu callback; Present the user with a form to update the comment types.
 */
function nodecomment_convert_page() {
  $convert_counts = nodecomment_convert_counts();
  $form = empty($convert_counts) ? '' : drupal_get_form('nodecomment_convert_form', $convert_counts);
  return theme('nodecomment_convert_page', $convert_counts, $form);
}

/**
 * Theme the list of items that need to be converted with the convert form.
 *
 * @param $convert_counts
 *   A list of all node types and comment types within that need to be
 *   converted. May be empty if no conversions are necessary.
 * @param $form
 *   A completely rendered form to be printed.
 */
function theme_nodecomment_convert_page($convert_counts, $form) {
  $output = '';

  if (empty($convert_counts)) {
    $output .= '<p>'. t('There are currently no comments that need to be converted.') .'</p>';
  }
  else {
    $output .= '<p>'. t('The following comment types need to be converted:') .'</p>';

    $convert_list = array();
    foreach ($convert_counts as $node_type => $comment_types) {
      $new_comment_type = nodecomment_get_comment_type($node_type);
      $new_comment_string = empty($new_comment_type) ? t('normal') : t('@type node', array('@type' => $new_comment_type));

      $node_type_output = '';
      $node_type_output .= '<strong>'. $node_type .'</strong>';

      $node_type_list = array();
      foreach ($comment_types as $old_comment_type => $count) {
        $old_comment_string = empty($old_comment_type) ? t('normal') : t('@type node', array('@type' => $old_comment_type));
        $node_type_list[] = t('There are @count %old_comment_type comments that need to be converted to %new_comment_type comments.', array('@count' => $count, '%old_comment_type' => $old_comment_string, '%new_comment_type' => $new_comment_string));
      }
      $node_type_output .= theme('item_list', $node_type_list);

      $convert_list[] = $node_type_output;
    }
    $output .= theme('item_list', $convert_list);
  }

  // Show the form for doing the conversion.
  $output .= $form;

  return $output;
}

/**
 * Confirmation and conversion form for converting node types.
 */
function nodecomment_convert_form(&$form_state, $convert_counts) {
  $form = array();
  $form_state['convert_counts'] = $convert_counts;
  $question = t('Run existing node comment conversions?');
  $path = 'admin/content/types';
  $message = '<strong>'. t('Warning') .':</strong> '. t('Converting between different node types may cause data loss if the destination node type does not have the same properties as the existing comment node type.');
  return confirm_form($form, $question, $path, $message, t('Convert comments'));
}

/**
 * Submit handler for nodecomment_convert_form.
 */
function nodecomment_convert_form_submit(&$form, &$form_state) {
  // Set the batch static variable, but do not run batch_process() since
  // we are inside of a submit handler. FAPI should run it for us.
  nodecomment_convert_batch_set($form_state['convert_counts']);
  $form_state['redirect'] = 'admin/content/types';
}

/**
 * Set the Batch API configuration for converting comments.
 *
 * This function may be used to initiate a comment conversion of all pending
 * comments. If called within a FAPI submit handler, batch_process() is called
 * automatically. Otherwise it will need to be called immediately after calling
 * the nodecomment_convert_batch_set() function.
 *
 * @param $convert_counts
 *   A list of comment types to be converted. Usually the return value from
 *   the nodecomment_convert_counts() function.
 */
function nodecomment_convert_batch_set($convert_counts = NULL) {
  if (!isset($convert_counts)) {
    $convert_counts = nodecomment_convert_counts();
  }

  $batch = array(
    'operations' => array(),
    'finished' => 'nodecomment_convert_batch_finished',
    'title' => t('Converting comments'),
    'file' => drupal_get_path('module', 'nodecomment') .'/nodecomment.convert.inc',
    'init_message' => t('Starting conversion.'),
    'error_message' => t('An error occurred while converting comments.'),
    'progress_message' => t('Processing @comment_count comments.', array('@comment_count' => nodecomment_convert_count_total($convert_counts))),
  );

  foreach ($convert_counts as $node_type => $comment_types) {
    foreach ($comment_types as $comment_type => $count) {
      $batch['operations'][] = array('nodecomment_convert_batch_process', array($node_type, $comment_type, $count));
    }
  }

  batch_set($batch);
}

/**
 * Batch API process function; Convert between comment types.
 */
function nodecomment_convert_batch_process($node_type, $previous_comment_type, $convert_count, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_nid'] = 0;
    $context['sandbox']['max'] = $convert_count;
  }

  if (!isset($context['results']['converted_comments'])) {
    $context['results']['converted_comments'] = 0;
  }

  // Limit the number of node comment conversions to do at once.
  $limit = 50;

  // Build a collection of items to convert, either comments or nodes.
  // The "SUBSTRING(thread, 1, (LENGTH(thread) - 1))" portion of the query
  // is taken from comment.module which gets all the children of threads
  // first, then work up the tree doing the parents last, this prevents
  // nodecomment from deleting the whole thread prematurely.
  $items = array();
  if ($previous_comment_type == '') {
    $result = db_query_range("SELECT c.* FROM {comments} c INNER JOIN {node} n ON c.nid = n.nid WHERE n.nid >= %d AND n.type = '%s' ORDER BY n.nid ASC, SUBSTRING(thread, 1, (LENGTH(thread) - 1)) DESC", $context['sandbox']['current_nid'], $node_type, 0, $limit);
    while ($comment = db_fetch_object($result)) {
      $items[] = $comment;
    }
  }
  else {
    $result = db_query_range("SELECT cid FROM {node_comments} c INNER JOIN {node} n ON c.nid = n.nid WHERE n.nid >= %d AND n.type = '%s' ORDER BY n.nid ASC, SUBSTRING(thread, 1, (LENGTH(thread) - 1)) DESC", $context['sandbox']['current_nid'], $node_type, 0, $limit);
    while ($row = db_fetch_object($result)) {
      $items[] = node_load($row->cid);
    }
  }

  // Now do some general conversion based on the type we're converting to.
  $comment_type = nodecomment_get_comment_type($node_type);
  foreach ($items as $item) {

    if ($comment_type == '') {
      // Converting the item into a normal comment.
      $comment = drupal_clone($item);
      if (!isset($item->cid)) {
        $comment->cid = NULL;
        $comment->nid = $item->comment_target_nid;
        $comment->pid = $item->comment_target_cid;
        $comment->subject = $item->title;
        $comment->comment = $item->body;
        $comment->timestamp = $item->created;
        $comment->status = $item->moderate;
      }

	  // Make comment_upload.module happy - convert each file entry to array instead of object.
      if (isset($comment->files) && count($comment->files)) {
        $new_files = array();
        foreach ($comment->files as $key => $file) {
		  $file = (array)$file;
          $file['new'] = TRUE;	// Flag file as new for comment_upload.module, so it will register it.
          $new_files[$key] = $file;
        }
        $comment->files = $new_files;
      }

      // Insert the new comment.
      $edit = (array) $comment;
      db_query("INSERT INTO {comments} (nid, pid, uid, subject, comment, format, hostname, timestamp, status, thread, name, mail, homepage) VALUES (%d, %d, %d, '%s', '%s', %d, '%s', %d, %d, '%s', '%s', '%s', '%s')", $edit['nid'], $edit['pid'], $edit['uid'], $edit['subject'], $edit['comment'], $edit['format'], $edit['hostname'], $edit['timestamp'], $edit['status'], $edit['thread'], $edit['name'], $edit['mail'], $edit['homepage']);
      $edit['cid'] = db_last_insert_id('comments', 'cid');

      // Tell the other modules a new comment has been submitted.
      comment_invoke_comment($edit, 'insert');

      // Update statistics.
      _comment_update_node_statistics($edit['nid']);

      // If converting from node comments to comments, delete the node.
      // It'd be nice to just use node_delete() here, but we don't want to
      // flood the user with hundreds of "Node X was deleted" messages.
      $node = node_load($item->nid);
      db_query('DELETE FROM {node} WHERE nid = %d', $node->nid);
      db_query('DELETE FROM {node_revisions} WHERE nid = %d', $node->nid);

      // Prevent file attachments from being deleted.
      $node->files = array();
      if (module_exists('upload')) {
        db_query('DELETE FROM {upload} WHERE nid = %d', $node->nid);
      }

      // Call the node-specific callback (if any):
      node_invoke($node, 'delete');
      node_invoke_nodeapi($node, 'delete');

      // Remove this node from the search index if needed.
      if (function_exists('search_wipe')) {
        search_wipe($node->nid, 'node');
      }
    }
    else {
      // Converting the item into a node comment.
      $node = drupal_clone($item);
      if (isset($item->cid)) {
        $node->comment_target_nid = $item->nid;
        $node->comment_target_cid = $item->pid;
        $node->nid = NULL;
        $node->teaser = $item->comment;
        $node->body = $item->comment;
        $node->title = $item->subject;
        $node->created = $item->timestamp;
        $node->changed = $item->timestamp;
        $node->status = 1;
        $node->moderate = module_exists('modr8') ? $item->status : 0;
        // Ask comment_upload.module to load files
        if (function_exists('comment_upload_load_files')) {
          $node->files = comment_upload_load_files($item->cid);
        }
      }
      $node->type = $comment_type;

	  // Make upload.module happy - convert each file entry to object instead of array.
      if (isset($node->files) && count($node->files)) {
        $new_files = array();
        foreach ($node->files as $key => $file) {
		  $file = (object)$file;
          $file->new = TRUE;	// Flag file as new for upload.module, so it will register it.
          $new_files[$key] = $file;
        }
        $node->files = $new_files;
      }

      node_save($node);

      // If converting from comments to node comments, delete the comment row.
      if ($previous_comment_type == '' && $comment_type) {
        db_query("DELETE FROM {comments} WHERE cid = %d", $item->cid);

        // Prevent file attachments from being deleted.
        $edit->files = array();
        if (module_exists('comment_upload')) {
          db_query('DELETE FROM {comment_upload} WHERE cid = %d', $item->cid);
        }
        comment_invoke_comment($item, 'delete');
      }
    }

    // Update our progress information.
    $context['sandbox']['progress']++;
    $context['sandbox']['current_nid'] = isset($item->cid) ? $item->nid : $item->comment_target_nid;
    $context['results']['converted_comments']++;
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] <= $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
  else {
    cache_clear_all();
    $context['finished'] = 1;
  }

}

/**
 * Finishing message after completing the conversion.
 */
function nodecomment_convert_batch_finished($success, $results, $operations) {
  drupal_set_message(t('@count comments converted.', array('@count' => $results['converted_comments'])));
}
