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
    try {
      $opts = bbcom_flashcards_anki_server_connect()
        ->openWialDeck($form['#user']->uid)
        ->getOptions();
    }
    catch (AnkiServerException $e) {
      _bbcom_flashcards_error($e);
      return $form;
    }

    jquery_ui_add('ui.tabs');
    drupal_add_css(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/themes/default/ui.all.css');

    $form['study_options'] = array(
      '#tree' => TRUE,
    );

    // generate some stuff for each tab
    $tabs_list = array(
      'new-cards' => t('New cards'),
      'reviews' => t('Reviews'),
    );
    $tabs = '<ul>';
    foreach ($tabs_list as $tab_id => $tab) {
      $tabs .= '<li><a href="#tab-' . $tab_id . '">' . $tab . '</a></li>';
      $form['study_options'][str_replace('-', '_', $tab_id)] = array(
        '#type' => 'fieldset',
        '#title' => $tabs_list['new-cards'],
        '#prefix' => '<div id="tab-' . $tab_id . '">',
        '#suffix' => '</div>',
      );
    }
    $tabs .= '</ul>';

    // setup the tabs html and wrap the whole thing
    $form['study_options']['#prefix'] = '<h2>' . t('Study Options') . '</h2>' .
      '<div id="bibliobird-flashcards-study-options"><div id="bibliobird-flashcards-study-options-tabs">' . $tabs . '</div>';
    $form['study_options']['#suffix'] = '</div>';

    // New cards
    $form['study_options']['new_cards']['cards_per_day'] = array(
      '#type' => 'textfield',
      '#title' => t('New cards per day'),
      '#default_value' => $opts->new_cards->cards_per_day,
    );
    $form['study_options']['new_cards']['order'] = array(
      '#type' => 'select',
      '#title' => t('Display order'),
      '#options' => array(
        0 => t('Show new cards in random order'),
        1 => t('Show new cards in order added'),
        2 => t('Show new cards in reverse order added'),
      ),
      '#default_value' => $opts->new_cards->order,
    );
    $form['study_options']['new_cards']['spacing'] = array(
      '#type' => 'select',
      '#options' => array(
        0 => t('Spread new cards out through reviews'),
        1 => t('Show new cards after all other cards'),
        2 => t('Show new cards before reviews'),
      ),
      '#default_value' => $opts->new_cards->spacing,
    );

    // Reviews
    $form['study_options']['reviews']['failed_card_max'] = array(
      '#type' => 'textfield',
      '#title' => t('Max failed cards'),
      '#default_value' => $opts->reviews->failed_card_max,
    );
    $form['study_options']['reviews']['order'] = array(
      '#type' => 'select',
      '#title' => t('Display order'),
      '#options' => array(
        0 => t('Review cards from largest interval'),
        1 => t('Review cards from smallest interval'),
        2 => t('Review cards in order due'),
        3 => t('Review cards in random order'),
      ),
      '#default_value' => $opts->reviews->order,
    );
    $form['study_options']['reviews']['failed_policy'] = array(
      '#type' => 'select',
      '#options' => array(
        0 => t('Show failed cards soon'),
        1 => t('Show failed cards at end'),
        2 => t('Show failed cards in 10 minutes'),
        3 => t('Show failed cards in 8 hours'),
        4 => t('Show failed cards in 3 days'),
      ),
      '#default_value' => $opts->reviews->failed_policy,
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
        $deck->setOptions($form_state['values']['study_options']);
        $deck->setupScheduler('standard');
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

