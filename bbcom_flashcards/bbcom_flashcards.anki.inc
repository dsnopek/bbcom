<?php

require_once('HTTP/Request2.php');

class AnkiServerException extends Exception { }

class AnkiDeck {
  public $server;
  public $deck;

  function __construct($server, $deck) {
    $this->server = $server;
    $this->deck = $deck;
  }

  function addFact($fields) {
    $this->server->request('add_fact', array(
      'deck_id' => $this->deck->id,
      'fields' => $fields
    ));
  }

  function addFactForNode($node) {
    $this->addFact(array(
      'Front' => theme('bbcom_flashcards_flashcard_front', $node),
      'Back'  => theme('bbcom_flashcards_flashcard_back', $node),
      'BiblioBird ID' => lingwo_dictionary_entry_get_id($node),
    ));
  }

  function findFactForNode($node) {
    return $this->server->request('find_fact', array(
      'deck_id' => $this->deck->id,
      'bibliobird_id' => lingwo_dictionary_entry_get_id($node),
    ));
    if (empty($fact)) {
      throw new AnkiServerError("Unable to find fact for node");
    }
  }

  function deleteFact($fact) {
    $this->server->request('delete_fact', array(
      'deck_id' => $this->deck->id,
      'fact_id' => $fact->id,
    ));
  }

  function getCard() {
    return $this->server->request('get_card', array(
      'deck_id' => $this->deck->id,
    ));
  }

  function answerCard($card, $ease) {
    $this->server->request('answer_card', array(
      'deck_id' => $this->deck->id,
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
  function request($func, $args) {
    $req = new HTTP_Request2('http://'. $this->url .'/'. $func, HTTP_Request2::METHOD_POST);
    $req->setHeader('Content-Type', 'application/json');
    $req->setBody(json_encode($args));

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
    try {
      $deck = $this->request('open_deck', array('path' => $path));
    }
    catch (AnkiServerException $e) {
      $deck = $this->request('create_deck', array('path' => $path));
    }

    return new AnkiDeck($this, $deck);
  }

  function openWialDeck($uid) {
    return $this->openDeck("anki/$uid/wial.anki");
  }
}

