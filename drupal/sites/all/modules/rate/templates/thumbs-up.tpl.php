<?php
/**
 * @file
 * Rate widget theme
 */

print theme('rate_button', $links[0]['text'], $links[0]['href'], 'rate-thumbs-up-btn-up');

if ($mode == RATE_FULL || $mode == RATE_CLOSED || $mode == RATE_DISABLED) {
  $info = array();
  if ($mode == RATE_CLOSED) {
    $info[] = t('Voting is closed.');
  }
  if ($results['user_vote']) {
    $info[] = format_plural($results['count'], 'Only you voted.', '@count users have voted, including you.');
  }
  else {
    $info[] = format_plural($results['count'], '@count user has voted.', '@count users have voted.');
  }
  print '<div class="rate-info">' . implode(' ', $info) . '</div>';
}

?>