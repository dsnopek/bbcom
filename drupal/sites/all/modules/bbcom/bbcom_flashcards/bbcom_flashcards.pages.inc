<?php

function bbcom_flashcards_page_wial_review_form(&$form_state, $account) {
  global $language;

  $form['#user'] = $account;
  $card = !empty($form_state['storage']['card']) ? $form_state['storage']['card'] : NULL;
  if ($card) {
    $form['#card'] = $card;
  }

  if (!empty($form_state['storage']['finished'])) {
    // clear out storage, so the next time we load this page, it'll start over
    unset($form_state['storage']);

    // If we are out of questions, we tell the user and let them reset the quiz,
    // if they want.
    $form['message'] = array(
      // TODO: we don't currently have a way to forecast tomorrows review
      '#value' => theme('bbcom_flashcards_review_finished', NULL, NULL),
    );
  }
  elseif (!empty($card)) {
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
        '#value' => $button['string_label'] . ' (' . $button['string_interval'] . ')',
        '#ease' => $button['ease'],
      );
    }
  }
  else {
    try {
      $collection = bbcom_flashcards_anki_server_connect()
        ->openCollection($form['#user']->uid);

      $collection->setLanguage($language->language);
      $counts = $collection->resetScheduler();
    }
    catch (AnkiServerException $e) {
      _bbcom_flashcards_error($e);
      return $form;
    }

    $form['#original_counts'] = $counts;
    $total_count = array_sum($counts);

    $form['message'] = array(
      '#value' => '<div class="bbcom-important-message">' . t('You can also review the "Words I Am Learning" as <strong>flashcards</strong> with <a href="http://www.ankisrs.net/" target="_blank">Anki</a> on your desktop!  <a href="https://en.bibliobird.com/node/16362" target="_blank">Read this article</a> to learn how to install Anki and configure it to communicate with BiblioBird.') . '</div>',
    );

    $form['study_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Study options'),
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
      '#description' => t("<em>Normal</em> will show you the cards due for today. <em>Cram</em> will randomly select cards regardless of when they are due."),
    );

    // if there are no cards for review today, we remove the option to review
    // for a 'normal' review
    if ($total_count == 0) {
      unset($form['study_options']['type']['#options']['normal']);
      $form['study_options']['type']['#description'] = t('<strong>There are no cards ready to review today!</strong> But you can still do a <em>"cram session"</em> which will randomly select cards regardless of when they are due.');
    }

    $form['study_options']['new_cards'] = array(
      '#prefix' => '<div class="bibliobird-flashcards-study-options-toggle bibliobird-flashcards-study-options-normal">',
      '#suffix' => '</div>',
      '#type' => 'item',
      '#title' => t('New cards'),
      '#value' => $counts['new_cards'],
    );

    $form['study_options']['review_cards'] = array(
      '#prefix' => '<div class="bibliobird-flashcards-study-options-toggle bibliobird-flashcards-study-options-normal">',
      '#suffix' => '</div>',
      '#type' => 'item',
      '#title' => t('Review cards'),
      '#value' => $counts['review_cards'],
    );

    $form['study_options']['dyn_cards'] = array(
      '#prefix' => '<div class="bibliobird-flashcards-study-options-toggle bibliobird-flashcards-study-options-cram">',
      '#suffix' => '</div>',
      '#type' => 'textfield',
      '#title' => t('Number of cards'),
      '#value' => 20,
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

    if ($form_state['clicked_button']['#value'] == t('Review')) {
      $study_options = $form_state['values']['study_options'];

      try {
        $collection->resetScheduler($study_options['type'], $study_options);
      }
      catch (AnkiServerException $e) {
        if (strpos($e->getMessage(), 'No cards matched the criteria you provided') !== FALSE) {
          drupal_set_message(t("You have no cards to review! Please add some words to your \"Words I am Learning\" list."), 'warning');
          return;
        }
        else {
          throw $e;
        }
      }

      // only set once we're sure that the $collection->resetScheduler(...)
      // worked.
      $form_state['storage']['study_options'] = $study_options;
    }

    // answer the last card
    if ($form['#card'] && $ease > 0) {
      $collection->answerCard($form['#card']['id'], $ease, $form['#card']['timerStarted']);
    }

    // get the next one
    $form_state['storage']['card'] = $collection->getNextCard();

    // if there is no card, then the session is over!
    if (empty($form_state['storage']['card'])) {
      $form_state['rebuild'] = FALSE;

      if ($form_state['storage']['study_options']['type'] == 'normal') {
        // show the 'finished' screen which explains that they have to come back
        // tomorrow
        $form_state['storage']['finished'] = TRUE;
      }
      else {
        // however, in cram mode, we simply return the user to the beginning.
        unset($form_state['storage']);
        drupal_set_message(t('Your session has ended.  Please start your review again.'));
      }
    }
  }
  catch (AnkiServerException $e) {
    _bbcom_flashcards_error($e);
  }
}

