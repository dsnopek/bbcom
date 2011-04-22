<?php
// $Id: rate-widget.tpl.php,v 1.6 2010/11/12 17:20:47 mauritsl Exp $

/**
 * @file
 * Rate widget theme
 *
 * This is the default template for rate widgets. See section 3 of the README
 * file for information on theming widgets.
 */

$num = 0;
foreach ($links as $link) {
  ++$num;
  print theme('rate_button', $link['text'], $link['href'], "rate-button-$num");
}

print t('Total votes: !count', array('!count' => $results['count'])); ?>