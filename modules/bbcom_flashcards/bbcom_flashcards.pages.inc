<?php

function bbcom_flashcards_page_wial_view($account) {
  return 'view';
}

function bbcom_flashcards_page_wial_quiz_form(&$form_state, $account) {
  $form['#user'] = $account;
  $card = $form_state['card'];
  if ($card) {
    $form['#card'] = $card;
  }

  $in_quiz = $form_state['values']['in_quiz'] == 'yes' || FALSE;
  $form['in_quiz'] = array(
    '#type'  => 'value',
    # unless this is the last question, we always want to say we are in the quiz
    '#value' => 'yes'
  );

  if (isset($form_state['values']['quiz_options'])) {
    // TODO: set our configuration
  }

  if ($in_quiz) {
    if ($card) {
      $form['title'] = array(
        '#value' => '<h3 id="bibliobird-flashcards-title">'. t('Question') .'</h3>',
      );
      $form['card'] = array(
        '#prefix' => '<div id="bibliobird-flashcards-flashcard"><center>',
        '#suffix' => '</center></div>',
      );
      $form['card']['question'] = array(
        '#prefix' => '<div id="bibliobird-flashcards-flashcard-question">',
        '#suffix' => '</div>',
        '#value' => $card->question,
      );
      $form['card']['answer'] = array(
        '#prefix' => '<div id="bibliobird-flashcards-flashcard-answer">',
        '#suffix' => '</div>',
        '#value' => $card->answer,
      );

      $form['ease_buttons'] = array(
        '#prefix' => '<div id="bibliobird-flashcards-ease-buttons">',
        '#suffix' => '</div>',
      );
      $ease_list = array(
        1 => 'Again (@time)',
        2 => 'Hard (@time)',
        3 => 'Good (@time)',
        4 => 'Easy (@time)',
      );
      foreach ($ease_list as $id => $text) {
        $form['ease_buttons']['ease_'. $id] = array(
          '#type' => 'submit',
          '#value' => t($text, array('@time' => $card->intervals[$id-1])),
          '#ease' => $id,
        );
      }
    }
    else {
      # If we are out of questions, we tell the user and let them reset the quiz,
      # if they want.
      $form['in_quiz']['#value'] = 'no';
      $form['message'] = array(
        '#value' => '<div id="bibliobird-flashcards-finished">'. t('Quiz finished!') .'</div>',
      );
      $form['reset_quiz'] = array(
        '#type' => 'submit',
        '#value' => t('Reset Quiz'),
      );
    }
  }
  else {
    $form['quiz_options'] = array(
      '#type' => 'radios',
      '#title' => t('Quiz Type'),
      '#default_value' => 'normal',
      '#options' => array(
        'normal' => t('Quiz all the words that are ready'),
        'limited' => t('Quiz a maximum of XXX ready cards'),
        'all' => t('Quiz all cards'),
      )
    );
    $form['start_quiz'] = array(
      '#type' => 'submit',
      '#value' => t('Start Quiz'),
    );
  }

  drupal_add_css(drupal_get_path('module', 'bbcom_flashcards') .'/bbcom_flashcards.css');
  drupal_add_js(drupal_get_path('module', 'bbcom_flashcards') .'/bbcom_flashcards.js');

  return $form;
}

function bbcom_flashcards_page_wial_quiz_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Reset Quiz')) {
    # we don't mark to rebuild which means the form will be regenerated from new
  	return;
  }

  # we continue to rebuild the same form
  $form_state['rebuild'] = TRUE;

  # get the ease if the question was answered
  $ease = $form_state['clicked_button']['#ease'];

  # answer the current card and fetch the next one
  try {
    $deck = bbcom_flashcards_anki_server_connect()
      ->openWialDeck($form['#user']->uid);
    if ($form_state['clicked_button']['#value'] == t('Start Quiz')) {
      # TODO: set the settings we want
    }
    if ($form['#card'] && $ease > 0) {
      $deck->answerCard($form['#card'], $ease);
    }
    $form_state['card'] = $deck->getCard();
  }
  catch (AnkiServerError $e) {
    watchdog('anki', $e->getMessage(), array(), WATCHDOG_ERROR);
    drupal_set_message(t('Unable to open flashcard deck.  Sorry for the inconvenience!  Please try again later.', 'error'));
  }
}

