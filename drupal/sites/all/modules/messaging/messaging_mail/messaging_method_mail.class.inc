<?php
/**
 * @file
 * Drupal Messaging Framework - Send_Method class file
 */

/**
 * Base class for mail sending methods
 */
class Messaging_Method_Mail extends Messaging_Send_Method {
  // Default group and address type
  public $group = 'mail';
  public $address_type = 'mail';
  public $type = MESSAGING_TYPE_SEND;
  public $anonymous = TRUE;
  
  /**
   * Get address for user account
   */
  function get_user_address($account) {
    $account = messaging_user_object($account);
    return $account->uid && $account->mail ? $account->mail : NULL;
  }
  /**
   * Get uid for address
   */
  function get_address_uid($mail) {
    return db_result(db_query("SELECT uid FROM {users} WHERE mail = '%s'", $mail));
  }
  /**
   * Validate address
   */
  function address_validate($address) {
    return valid_email_address($address);
  }

  /**
   * Message default callback: prepare
   * 
   * Decide on queue, log, cron and send options, prepare parameters
   */
  function message_prepare($message) {
    // First, decide on queue, log, cron and send options
    parent::message_prepare($message);
    // Build specific mail parameters and store them into the message
    $params = $message->get_params('mail') + $this->default_params();
    $message->params[$this->method] = $this->mail_params($message, $params);
  }
  
  /**
   * Get default method parameters
   */
  static function default_params($message = NULL) {
    $default_mail = variable_get('site_mail', ini_get('sendmail_from'));
    return array(
      'default_from' => variable_get('messaging_mail_default_from', $default_mail),
      'returnpath' => variable_get('messaging_mail_returnpath', $default_mail),
    );
  }


  /**
   * Rebuild message in Drupal mail format, right before sending
   * 
   * @param $destination
   *   Email destination
   * @param $message
   *   Message object
   * @param $params
   *   Aditional parameters
   * @param $alter
   *   Whether to run the mail_alter hook
   */
  static function mail_build($destination, $message, $params = array(), $alter = TRUE) {
    // Build parameters and headers if not previously built
    if (!$params || empty($params['headers'])) {
      $params = self::mail_params($message, $params);
    }
    // Build the mail object, mimic drupal_mail() format
    $mail = array(
      'id'       => $params['id'],
      'to'       => $destination,
      'from'     => $params['from'],
      'language' => $message->get_language(),
      'subject'  => $message->get_subject(),
      'body'     => $message->get_body(),
      'headers'  => $params['headers'],
      'attachments' => $message->get_files(),
      'params'   => $params,
    );
    // Invoke hook_mail_alter() to allow all modules to alter the resulting e-mail.
    if ($alter) {
      drupal_alter('mail', $mail);
    }
    return $mail;
  }
  
  /**
   * Format from name and address
   */
  static function format_from($name, $mail) {
    $tokens = array(
      '[name]' => $name,
      '[mail]' => $mail,
    );
    return strtr(variable_get('messaging_mail_sender_format', '[name] <[mail]>'), $tokens);
  }

  /**
   * Prepare from address and mail headers
   */
  static function mail_params($message, $params = array()) {
    // The message 'from' will depend on message sender if present, otherwise default to site mail    
    if (empty($params['from'])) {
      $sender_name = $message->get_sender_name();
      $sender_account = $message->get_sender();
      if ($sender_name && $sender_account && !empty($sender_account->mail)) {
        $from = self::format_from($sender_name, $sender_account->mail);
      } elseif ($sender_name) {
        $from = self::format_from($sender_name, $params['default_from']);
      } else {
        $from = $params['default_from'];
      }
      $params['from'] = $from;
    } else {
      $from = $params['from'];
    }
  
    // Set headers, or add to existing ones. Pre-existing ones should not be overridden.
    $headers = self::mail_headers($params);

    $params += array(
      'id'       => 'messaging_'. (!empty($message->type) ? 'message-'.$message->type : 'message'),
      'from'     => $from,
      'headers'  => array(),
    );
    $params['headers'] += $headers;  
  
    return $params;
  }
  
  /**
   * Get mail headers. Helper function for mail methods
   * 
   * @param $params
   *   Array of parameters with the following elements
   *   - 'from', Mail from address
   *   - 'default_from', Default from address
   *   - 'headers', Predefined headers to be added to this one
   */
  static function mail_headers($params) {
    $headers = !empty($params['headers']) ? $params['headers'] : array();
    
    // Add some default headers
    $headers += array(
      'MIME-Version'              => '1.0',
      'Content-Type'              => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
      'Content-Transfer-Encoding' => '8Bit',
      'X-Mailer'                  => 'Drupal'
    );
    
    $default_from = $params['default_from'];
    $from = !empty($params['from']) ? $params['from'] : $default_from;
  
    // Set default headers depending on data
    $headers += array(
      'From' => $from,
      'Reply-To' => $from,
    );
    
    if ($params['returnpath']) {
      // To prevent e-mail from looking like spam, the addresses in the Sender and
      // Return-Path headers should have a domain authorized to use the originating
      // SMTP server. Errors-To is redundant, but shouldn't hurt.
      $more_headers['Sender'] = $more_headers['Return-Path'] = $more_headers['Errors-To'] = $params['returnpath'];
      $headers += $more_headers;
    }  
  
    return $headers;
  }
  
}