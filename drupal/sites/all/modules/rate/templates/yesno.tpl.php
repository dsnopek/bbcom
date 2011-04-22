<?php
/**
 * @file
 * Rate widget theme
 */
?>

<?php foreach ($links as $link): ?>
  <?php print theme('rate_button', $link['text'], $link['href'], 'rate-yesno-btn'); ?>
<?php endforeach; ?>

<?php

if ($mode == RATE_FULL || $mode == RATE_CLOSED) {
  $info = array();
  if ($mode == RATE_CLOSED) {
    $info[] = t('Voting is closed.');
  }
  if ($results['user_vote']) {
    $info[] = t('You voted \'@option\'.', array('@option' => $results['user_vote']));
  }
  $info[] = format_plural($results['count'], '@count user has voted.', '@count users have voted.');
  print '<div class="rate-info">' . implode(' ', $info) . '</div>';
}

?>