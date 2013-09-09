<?php

class AnkiServerException extends Exception { }

class AnkiCollection {
  public $server;
  public $name;

  function __construct($server, $name) {
    $this->server = $server;
    $this->name = $name;
  }

  function setup() {
    // NOTE: This is done automatically now, so this a non-OP!
  }

  function addNote($note) {
    return $this->server->request($this->name, 'add_note', $note);
  }

  function updateNote($note) {
    return $this->server->request($this->name, "note/{$note['id']}/update", $note);
  }

  function addNoteForNode($node) {
    $note = $this->findNoteForNode($node);
    $new = is_null($note);

    if ($new) {
      $note = array(
        'model' => 'External',
        'fields' => array(
          'External ID' => LingwoEntry::fromNode($node)->getLingwoId(),
        ),
      );
    }

    // Update / set the fields
    $note['fields'] = array_merge($note['fields'], array(
      'Front' => theme('bbcom_flashcards_flashcard_front', $node),
      'Back'  => theme('bbcom_flashcards_flashcard_back', $node),
    ));

    if ($new) {
      return $this->addNote($note);
    }
    else {
      return $this->updateNote($note);
    }
  }

  function findNoteForNode($node, $preload = TRUE) {
    $lingwo_id = LingwoEntry::fromNode($node)->getLingwoId();
    $query = '"External ID":"' . $lingwo_id . '"';
    $result = $this->server->request($this->name, 'find_notes', array(
      'query' => $query,
      'preload' => $preload,
    ));

    if (count($result) > 1) {
      watchdog('bbcom_flashcards', "Duplicate notes for $lingwo_id in collection {$this->name}");
    }

    if (count($result) == 0) {
      return NULL;
    }

    return $result[0];
  }

  function deleteNote($note) {
    return $this->server->request($this->name, "note/{$note['id']}/delete", array());
  }

  function deleteNoteForNode($node) {
    $note = $this->findNoteForNode($node);
    if ($note) {
      return $this->deleteNote($note);
    }
  }

  // TODO: We need to implement this!
  function resyncNotes($ids) {
    return $this->server->request($this->name, 'resync_notes', array(
      'external_ids' => $ids,
    ));
  }

  function resetScheduler($type = 'standard') {
    // TODO: How will we handle the review types?
    return $this->server->request($this->name, 'reset_scheduler', array(
      'deck' => 'bibliobird',
    ));
  }

  function getNextCard() {
    return $this->server->request($this->name, 'next_card', array(
      'load_note' => TRUE,
    ));
  }

  function answerCard($card_id, $ease, $timer_started = NULL) {
    return $this->server->request($this->name, 'answer_card', array(
      'id' => $card->id,
      'ease' => $ease,
      'timerStarted' => $timer_started,
    ));
  }
}

class AnkiServer {
  private $url;

  function __construct($url) {
    $this->url = $url;
  }

  // TODO: should we provide convenience functions we the names and normal arguments?
  // I'm not aware of an automatic way to do it, so my inclination is to let it be.
  function request($collection_name, $func, $args=array()) {
    $url = 'http://' . $this->url . '/' . $collection_name . '/' . $func;
    $headers = array('Content-Type', 'application/json');

    $resp = drupal_http_request($url, $headers, 'POST', json_encode((object)$args));

    if ($resp->code != 200) {
      throw new AnkiServerException($resp->data);
    }

    return json_decode($resp->data, TRUE);
  }

  function openCollection($name) {
    return new AnkiCollection($this, $name);
  }

}

