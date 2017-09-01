<?php
/**
 * @file
 * Administration page callbacks for the auth_ufrpe module.
 */

/**
 * Form builder. Set auth_ufrpe settings.
 *
 * @ingroup forms
 * @see system_settings_form().
 */

function auth_ufrpe_settings_form($form, &$form_state)
{
  // Define what will be the urls of the CPA webforms

  $form['auth_ufrpe_cpa_urls'] = array(
    '#type' => 'fieldset',
    '#title' => t('Links para webforms CPA'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => 'Links para redirecionamento do usuário após a autenticação ser concluída.',
  );

  $form['auth_ufrpe_cpa_urls']['docente-presencial'] = array(
    '#type' => 'textfield',
    '#title' => t('Docente presencial:'),
    '#required' => TRUE,
  );

  $form['auth_ufrpe_cpa_urls']['discente-presencial'] = array(
    '#type' => 'textfield',
    '#title' => t('Discente presencial:'),
    '#required' => TRUE,
  );

  $form['auth_ufrpe_cpa_urls']['tecnico-presencial'] = array(
    '#type' => 'textfield',
    '#title' => t('Técnico presencial:'),
    '#required' => TRUE,
  );

  $form['auth_ufrpe_cpa_urls']['docente-ead'] = array(
    '#type' => 'textfield',
    '#title' => t('Docente EAD:'),
    '#required' => TRUE,
  );

  $form['auth_ufrpe_cpa_urls']['discente-ead'] = array(
    '#type' => 'textfield',
    '#title' => t('Discente EAD:'),
    '#required' => TRUE,
  );

  $form['auth_ufrpe_cpa_urls']['tecnico-ead'] = array(
    '#type' => 'textfield',
    '#title' => t('Técnico EAD:'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Salvar configurações'),
  );

  $form['#submit'][] = 'auth_ufrpe_settings_form_submit';
  return $form;
}

/**
 * Process auth_ufrpe settings form
 */

function auth_ufrpe_settings_form_submit($form, &$form_state)
{
  // Get current settings and insert on database
  $record = array(
    'doc_pre' => $form_state['values']['docente-presencial'],
    'dis-pre' => $form_state['values']['discente-presencial'],
    'tec-pre' => $form_state['values']['tecnico-presencial'],
    'doc_ead' => $form_state['values']['docente-ead'],
    'dis-ead' => $form_state['values']['discente-ead'],
    'tec-ead' => $form_state['values']['tecnico-ead'],
    'date' => date('Y-m-d H:m:s'),
  );

  drupal_write_record('auth_ufrpe_conf', $record);
}
