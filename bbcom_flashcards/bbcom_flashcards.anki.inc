<?php

require_once('HTTP/Request2.php');

class AnkiServerException extends Exception { }

class AnkiDeck {
  public $server;
  public $deck;

  function __construct($server, $path) {
    $this->server = $server;
    $this->path = $path;
  }

  function setup() {
    $this->server->request($this->path, 'setup', array());
  }

  function addFact($fields) {
    $this->server->request($this->path, 'add_fact', array(
      'fields' => $fields
    ));
  }

  function addFactForNode($node) {
    $this->addFact(array(
      'Front' => theme('bbcom_flashcards_flashcard_front', $node),
      'Back'  => theme('bbcom_flashcards_flashcard_back', $node),
      'External ID' => lingwo_dictionary_entry_get_id($node),
    ));
  }

  function findFactForNode($node) {
    return $this->server->request($this->path, 'find_fact', array(
      'external_id' => lingwo_dictionary_entry_get_id($node),
    ));
    if (empty($fact)) {
      throw new AnkiServerError("Unable to find fact for node");
    }
  }

  function deleteFact($fact) {
    $this->server->request($this->path, 'delete_fact', array(
      'fact_id' => $fact->id,
    ));
  }

  function deleteFactForNode($node) {
    $this->server->request($this->path, 'delete_fact', array(
      'external_id' => lingwo_dictionary_entry_get_id($node),
    ));
  }

  function resyncFacts($ids) {
    return $this->server->request($this->path, 'resync_facts', array(
      'external_ids' => $ids,
    ));
  }

  function getCard() {
    return $this->server->request($this->path, 'get_card');
  }

  function answerCard($card, $ease) {
    $this->server->request($this->path, 'answer_card', array(
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
    $req = new HTTP_Request2('http://'. $this->url .'/'. $path .'/'. $func, HTTP_Request2::METHOD_POST);
    $req->setHeader('Content-Type', 'application/json');
    $req->setBody(json_encode((object)$args));

    try {
      $res = $req->send();
    }
    catch (HTTP_Request2_Exception $e) {
      throw new AnkiServerException($e->getMessage());
    }

    if ($res->getStatus() >= 300) {
      throw new AnkiServerException($res->getBody());
    }

    if ($res->getHeader('Content-Type') == 'application/json') {
      return json_decode($res->getBody());
    }

    return $res->getBody();
  }

  function openDeck($path) {
    return new AnkiDeck($this, $path);
  }

  function openWialDeck($uid) {
    return $this->openDeck("$uid/bibliobird.anki");
  }
}

