<?php

function bbcom_flashcards_page_wial_review_form(&$form_state, $account) {
  $form['#user'] = $account;
  $card = $form_state['card'];
  if ($card) {
    $form['#card'] = $card;
  }

  if (!empty($card)) {
    if (!$card->finished) {
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
      $form['message'] = array(
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
    $form['study_options'] = array(
      '#prefix' => '<h2>' . t('Study Options') . '</h2><div class="bbcom-flashcards-study-options"><ul></ul>',
      '#suffix' => '</div>',
    );

    // New cards
    $form['study_options']['new_cards'] = array(
      '#type' => 'fieldset',
      '#title' => t('New cards'),
    );
    $form['study_options']['new_cards']['cards_per_day'] = array(
      '#type' => 'textfield',
      '#title' => t('New cards per day'),
      '#default_value' => 20,
    );
    $form['study_options']['new_cards']['display_order_1'] = array(
      '#type' => 'select',
      '#title' => t('Display order'),
      '#options' => array(
        t('Show new cards in random order'),
        t('Show new cards in order added'),
        t('Show new cards in reverse order added'),
      ),
      '#default_value' => t('Show new cards in order added'),
    );
    $form['study_options']['new_cards']['display_order_2'] = array(
      '#type' => 'select',
      '#options' => array(
        t('Spread new cards out through reviews'),
        t('Show new cards after all other cards'),
        t('Show new cards before reviews'),
      ),
      '#default_value' => t('Spread new cards out through reviews'),
    );

    // Reviews
    $form['study_options']['reviews'] = array(
      '#type' => 'fieldset',
      '#title' => t('Reviews'),
    );
    $form['study_options']['reviews']['max_failed_cards'] = array(
      '#type' => 'textfield',
      '#title' => t('Max failed cards'),
      '#default_value' => 20,
    );
    $form['study_options']['reviews']['display_order_1'] = array(
      '#type' => 'select',
      '#title' => t('Display order'),
      '#options' => array(
        t('Show new cards in random order'),
        t('Show new cards in order added'),
        t('Show new cards in reverse order added'),
      ),
      '#default_value' => t('Show new cards in order added'),
    );
    $form['study_options']['reviews']['display_order_2'] = array(
      '#type' => 'select',
      '#options' => array(
        t('Review cards from largest interval'),
        t('Review cards from smallest interval'),
        t('Review cards in order due'),
        t('Review cards in random order'),
      ),
      '#default_value' => t('Review cards from largest interval'),
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
  drupal_set_message(t('Unable to open flashcard deck.  Sorry for the inconvenience!  Please try again later.'), 'error');
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
    $deck = bbcom_flashcards_anki_server_connect()
      ->openWialDeck($form['#user']->uid);

    switch ($form_state['clicked_button']['#value']) {
      case t('Review'):
        $deck->setupScheduler('standard');
        # TODO: set the settings we want
        break;

      case t('Learn more'):
        $deck->setupScheduler('learnMore');
        break;

      case t('Review early'):
        $deck->setupScheduler('reviewEarly');
        break;
    }

    // answer the last card
    if ($form['#card'] && $ease > 0) {
      $deck->answerCard($form['#card'], $ease);
    }

    // get the next one
    $form_state['card'] = $deck->getCard();
  }
  catch (AnkiServerException $e) {
    _bbcom_flashcards_error($e);
  }
}

