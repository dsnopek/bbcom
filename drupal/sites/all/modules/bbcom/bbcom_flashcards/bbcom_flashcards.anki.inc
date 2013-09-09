<?php

class AnkiServerException extends Exception { }

class AnkiCollection {
  public $server;
  public $deck;

  function __construct($server, $path) {
    $this->server = $server;
    $this->path = $path;
  }

  function setup() {
    // NOTE: This is done automatically now, so this a non-OP!
  }

  function addNote($note) {
    return $this->server->request($this->path, 'add_note', $note);
  }

  function updateNote($note) {
    return $this->server->request($this->path, "note/{$note['id']}/update", $note);
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
    $result = $this->server->request($this->path, 'find_notes', array(
      'query' => $query,
      'preload' => $preload,
    ));

    if (count($result) > 1) {
      watchdog('bbcom_flashcards', "Duplicate notes for $lingwo_id in collection $this->path");
    }

    if (count($result) == 0) {
      return NULL;
    }

    return $result[0];
  }

  function deleteNote($note) {
    return $this->server->request($this->path, "note/{$note['id']}/delete", array());
  }

  function deleteNoteForNode($node) {
    $note = $this->findNoteForNode($node);
    if ($note) {
      return $this->deleteNote($note);
    }
  }

  function resyncNotes($ids) {
    return $this->server->request($this->path, 'resync_notes', array(
      'external_ids' => $ids,
    ));
  }

  function getOptions() {
    return $this->server->request($this->path, 'get_options');
  }

  function setOptions($study_options) {
    return $this->server->request($this->path, 'set_options', array(
      'study_options' => $study_options,
    ));
  }

  function setupScheduler($name) {
    return $this->server->request($this->path, 'setup_scheduler', array(
      'name' => $name,
    ));
  }

  function getCard() {
    return $this->server->request($this->path, 'get_card');
  }

  function answerCard($card, $ease) {
    return $this->server->request($this->path, 'answer_card', array(
      'card_id' => $card->id,
      'ease' => $ease,
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
  function request($path, $func, $args=array()) {
    $url = 'http://' . $this->url . '/' . $path . '/' . $func;
    $headers = array('Content-Type', 'application/json');

    $resp = drupal_http_request($url, $headers, 'POST', json_encode((object)$args));

    if ($resp->code != 200) {
      throw new AnkiServerException($resp->data);
    }

    return json_decode($resp->data, TRUE);
  }

  function openCollection($path) {
    return new AnkiCollection($this, $path);
  }

}

