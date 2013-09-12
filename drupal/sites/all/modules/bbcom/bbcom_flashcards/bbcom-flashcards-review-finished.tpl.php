<div id="bibliobird-flashcards-finished"><?php print t('Congratuations!') ?></div>
<p><?php print t('You have finished for now.'); ?></p>
<p><?php
  // TODO: Show this data again if we can get it!
  //print t('At this time tomorrow:<br />There will be <span class="bibliobird-flashcards-count">!reviews_count reviews</span>.<br />And <span class="bibliobird-flashcards-count">!new_count new</span> cards.', 
  //  array('!reviews_count' => $reviews_count, '!new_count' => $new_count));
  print t('<strong>Please return around this time tomorrow and review again!</strong> We only show you the cards that need to be reviewed each day. To learn more about how "spaced repetition" works, please <a href="!url">read this article</a> in the library.', array('!url' => url('embed-reader/l/www.linguatrek.com/blog/2011/01/what-is-spaced-repetition')));
?></p>
<p><?php
  print t('<strong>Or you can <a href="!url">start a "cram session"</a>,</strong> and review random words regardless of when they are due.', array('!url' => url('wial/review')));
?></p>

