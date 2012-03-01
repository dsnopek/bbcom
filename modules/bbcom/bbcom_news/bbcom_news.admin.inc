<?php
// $Id$

/**
 * @file
 * Admin pages for news
 */

function bbcom_news_admin_settings() {
  $form['mailchimp'] = array(
    '#type' => 'fieldset',
    '#title' => t('MailChimp'),
    '#tree' => FALSE,
  );
  $form['mailchimp']['bbcom_news_mailchimp_integration'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable MailChimp integration'),
    '#default_value' => variable_get('bbcom_news_mailchimp_integration', FALSE),
  );
  $form['mailchimp']['bbcom_news_mailchimp_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('MailChimp API Key'),
    '#default_value' => variable_get('bbcom_news_mailchimp_api_key', ''),
  );
  $form['mailchimp']['bbcom_news_mailchimp_list_name'] = array(
    '#type' => 'textfield',
    '#title' => t('MailChimp List Name'),
    '#default_value' => variable_get('bbcom_news_mailchimp_list_name', 'BiblioBird.com Newsletter'),
  );
  $form['mailchimp']['bbcom_news_mailchimp_language_group'] = array(
    '#type' => 'textfield',
    '#title' => t('MailChimp Language Group'),
    '#default_value' => variable_get('bbcom_news_mailchimp_language_group', 'Language'),
  );

  return system_settings_form($form);
}

