<?php
/**
 * @file
 * Rate widget theme
 */
?>

<?php foreach ($links as $link): ?>
  <?php print theme('rate_button', $link['text'], $link['href'], 'rate-emotion-btn'); ?>
  <div class="rate-emotion-votes"><?php print $link['votes']; ?></div>
<?php endforeach; ?>

<?php

if ($mode == RATE_FULL || $mode == RATE_CLOSED) {
  $info = array();
  if ($mode == RATE_CLOSED) {
    $info[] = t('Voting is closed.');
  }
  if ($results['user_vote']) {
    $info[] = t('You voted \'@option\'.', array('@option' => t($results['user_vote'])));
  }
  if ($info) {
    print '<div class="rate-info">' . implode(' ', $info) . '</div>';
  }
}

?>