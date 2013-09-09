<?php

function bbcom_flashcards_page_wial_review_form(&$form_state, $account) {
  $form['#user'] = $account;
  $card = $form_state['card'];
  if ($card) {
    $form['#card'] = $card;
  }

  if (!empty($card)) {
    if (!$card['finished']) {
      $form['card'] = array(
        '#prefix' => '<div id="bibliobird-flashcards-flashcard"><center>',
        '#suffix' => '</center></div>',
      );
      $form['card']['question'] = array(
        '#prefix' => '<div id="bibliobird-flashcards-flashcard-question">',
        '#suffix' => '</div>',
        '#value' => $card['question'],
      );
      $form['card']['answer'] = array(
        '#prefix' => '<div id="bibliobird-flashcards-flashcard-answer">',
        '#suffix' => '</div>',
        '#value' => $card['answer'],
      );

      $form['ease_buttons'] = array(
        '#prefix' => '<div id="bibliobird-flashcards-ease-buttons">',
        '#suffix' => '</div>',
      );
      foreach ($card['answer_buttons'] as $button) {
        $form['ease_buttons']['ease_'. $button['ease']] = array(
          '#type' => 'submit',
          '#value' => t($button['label'] . ' (@time)', array('@time' => $button['interval_string'])),
          '#ease' => $button['ease'],
        );
      }
    }
    else {
      # If we are out of questions, we tell the user and let them reset the quiz,
      # if they want.
      $form['message'] = array(
        // TODO: where do these come from?
        '#value' => theme('bbcom_flashcards_review_finished', $card->reviews_count, $card->new_count),
      );
      $form['learn_more'] = array(
        '#type' => 'submit',
        '#value' => t('Learn more'),
      );
      $form['review_early'] = array(
        '#type' => 'submit',
        '#value' => t('Review early'),
      );
      $form['finish'] = array(
        '#type' => 'submit',
        '#value' => t('Finish'),
      );
    }
  }
  else {
    try {
      $counts = bbcom_flashcards_anki_server_connect()
        ->openCollection($form['#user']->uid)
        ->resetScheduler();

      // TODO: reset and get counts
    }
    catch (AnkiServerException $e) {
      _bbcom_flashcards_error($e);
      return $form;
    }

    $form['message'] = array(
      '#value' => '<div class="bbcom-important-message">' . t('You can also review the "Words I Am Learning" as <strong>flashcards</strong> with <a href="http://www.ankisrs.net/" target="_blank">Anki</a> on your desktop!  <a href="http://www.youtube.com/watch?v=juqB3fPFcuM" target="_blank">Watch this video</a> to learn how to install Anki and configure it to communicate with BiblioBird.') . '</div>',
    );

    $form['study_options'] = array(
      '#tree' => TRUE,
    );

    $form['study_options']['type'] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#options' => array(
        // somehow explain that this is today's daily study regimin
        'normal' => t('Normal'),
        'cram'   => t('Cram'),
      ),
    );

    $form['study_options']['new_cards'] = array(
      '#type' => 'item',
      '#title' => t('New cards'),
      '#value' => 0,
    );

    $form['study_options']['learning_cards'] = array(
      '#type' => 'item',
      '#title' => t('Learning cards'),
      '#value' => 0,
    );

    $form['study_options']['review_cards'] = array(
      '#type' => 'item',
      '#title' => t('Review cards'),
      '#value' => 0,
    );


    $form['start_quiz'] = array(
      '#type' => 'submit',
      '#value' => t('Review'),
    );
  }

  drupal_add_css(drupal_get_path('module', 'bbcom_flashcards') .'/bbcom_flashcards.css');
  drupal_add_js(drupal_get_path('module', 'bbcom_flashcards') .'/bbcom_flashcards.js');

  return $form;
}

function _bbcom_flashcards_error($e) {
  watchdog('anki', $e->getMessage(), array(), WATCHDOG_ERROR);
  drupal_set_message(t('Unable to open flashcard collection.  Sorry for the inconvenience!  Please try again later.'), 'error');
}

function bbcom_flashcards_page_wial_review_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Finish')) {
    # we don't mark to rebuild which means the form will be regenerated from new
    return;
  }

  # we continue to rebuild the same form
  $form_state['rebuild'] = TRUE;

  # get the ease if the question was answered
  $ease = $form_state['clicked_button']['#ease'];

  # answer the current card and fetch the next one
  try {
    $collection = bbcom_flashcards_anki_server_connect()
      ->openCollection($form['#user']->uid);

    switch ($form_state['clicked_button']['#value']) {
      case t('Review'):
        $collection->resetScheduler('standard');
        break;

      case t('Learn more'):
        $collection->resetScheduler('learnMore');
        break;

      case t('Review early'):
        $collection->resetScheduler('reviewEarly');
        break;
    }

    // answer the last card
    if ($form['#card'] && $ease > 0) {
      if (!$collection->answerCard($form['#card']['id'], $ease, $form['#card']['timerStarted'])) {
        // return to the options screen and tell user their session has ended
        $form_state['rebuild'] = FALSE;
        drupal_set_message(t('Your session has ended.  Please start your review again.'), 'error');
        return;
      }
    }

    // get the next one
    $form_state['card'] = $collection->getNextCard();
  }
  catch (AnkiServerException $e) {
    _bbcom_flashcards_error($e);
  }
}

