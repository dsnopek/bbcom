<?php
// $Id: rate-widget.tpl.php,v 1.6 2010/11/12 17:20:47 mauritsl Exp $

/**
 * @file
 * Rate widget theme
 *
 * This is the default template for rate widgets. See section 3 of the README
 * file for information on theming widgets.
 */

$rating = $results['rating'];
$class = 'neutral';
if ($rating > 0) {
  $rating = '+'. $rating;
  $class = 'positive';
}
elseif ($rating < 0) {
  $class = 'negative';
}?>

<div class="bbcom-entry-rating bbcom-entry-rating-<?php print $class ?>">
<div class="bbcom-entry-rating-count"><?php print $rating; ?></div>
<!-- <div class="bbcom-entry-rating-label"><?php print t('Rating') ?></div> -->
</div>

<ul class="bbcom-entry-rating-list">
<?php
foreach ($links as $link) {
  if (!empty($results['user_vote']) && $link['value'] == $results['user_vote']) {
    print '<li><strong>'. $link['text'] .'</strong></li>';
  }
  else {
    print '<li>'. theme('rate_button', $link['text'], $link['href'], NULL) .'</li>';
  }
}
?>
</ul>

