<?php
// $Id: rate-widget.tpl.php,v 1.6 2010/11/12 17:20:47 mauritsl Exp $

/**
 * @file
 * Rate widget theme
 *
 * This is the default template for rate widgets. See section 3 of the README
 * file for information on theming widgets.
 */

drupal_add_css(drupal_get_path('module', 'bbcom_rate') . '/bbcom_rate.css');

$rating = $results['rating'];
$class = 'neutral';
if ($rating > 0) {
  $rating = '+'. $rating;
  $class = 'positive';
}
elseif ($rating < 0) {
  $class = 'negative';
}?>

<div class="bbcom-rate bbcom-rate-<?php print $class ?>">
<div class="bbcom-rate-count"><?php print $rating; ?></div>
<!-- <div class="bbcom-entry-rating-label"><?php print t('Rating') ?></div> -->
</div>

<ul class="bbcom-rate-list clear-block">
<?php
foreach ($links as $link) {
  if (!empty($results['user_vote']) && $link['value'] == $results['user_vote']) {
    print '<li><strong>'. $link['text'] .'</strong></li>';
  }
  else {
    print '<li>'. l($link['text'], $link['href']) .'</li>';
  }
}
?>
</ul>

<?php if ($rating >= variable_get('bbcom_dictionary_verified_threshold', 2)): ?>
<div class="bbcom-rate-verified"><?php print t('This entry is verified'); ?></div>
<?php endif; ?>

