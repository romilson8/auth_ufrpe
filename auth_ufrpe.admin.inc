<?php
/**
 * @file
 * Administration page callbacks for the auth_ufrpe module.
 */

// Importing functions from file
require_once 'custom_functions.php';

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
    'doc_pre' => $form['auth_ufrpe_cpa_urls']['docente-presencial']['#value'],
    'dis-pre' => $form['auth_ufrpe_cpa_urls']['discente-presencial']['#value'],
    'tec-pre' => $form['auth_ufrpe_cpa_urls']['tecnico-presencial']['#value'],
    'doc_ead' => $form['auth_ufrpe_cpa_urls']['docente-ead']['#value'],
    'dis-ead' => $form['auth_ufrpe_cpa_urls']['discente-ead']['#value'],
    'tec-ead' => $form['auth_ufrpe_cpa_urls']['tecnico-ead']['#value'],
    'ano' => date('Y'),
    'semestre' => getSemester((int) date('m')),
  );

  drupal_write_record('auth_ufrpe_settings', $record);
}
