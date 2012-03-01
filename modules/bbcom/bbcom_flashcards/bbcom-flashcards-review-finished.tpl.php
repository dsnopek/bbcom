<div id="bibliobird-flashcards-finished"><?php print t('Congratuations!') ?></div>
<p><?php print t('You have finished for now.'); ?></p>
<p><?php
  print t('At this time tomorrow:<br />There will be <span class="bibliobird-flashcards-count">!reviews_count reviews</span>.<br />And <span class="bibliobird-flashcards-count">!new_count new</span> cards.', 
  array('!reviews_count' => $reviews_count, '!new_count' => $new_count)); ?></p>

