<?php
/**
 * @file
 * Administration page callbacks for the auth_ufrpe module.
 *
 * @author Matheus Campos da Silva, 2017
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

  $form['auth_ufrpe_cpa_presential_urls'] = array(
    '#type' => 'fieldset',
    '#title' => t('Links para webforms CPA Presencial'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => 'Links dos webforms para a modality presencial.',
  );

  $form['auth_ufrpe_cpa_presential_urls']['docente-presencial'] = array(
    '#type' => 'textfield',
    '#title' => t('Docente presencial:'),
    '#required' => TRUE,
  );

  $form['auth_ufrpe_cpa_presential_urls']['discente-presencial'] = array(
    '#type' => 'textfield',
    '#title' => t('Discente presencial:'),
    '#required' => TRUE,
  );

  $form['auth_ufrpe_cpa_presential_urls']['tecnico-presencial'] = array(
    '#type' => 'textfield',
    '#title' => t('Técnico presencial:'),
    '#required' => TRUE,
  );

  $form['auth_ufrpe_cpa_e-learning_urls'] = array(
    '#type' => 'fieldset',
    '#title' => t('Links para webforms CPA EAD'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => 'Links dos webforms para a modality EAD.'
  );

  $form['auth_ufrpe_cpa_e-learning_urls']['docente-ead'] = array(
    '#type' => 'textfield',
    '#title' => t('Docente EAD:'),
    '#required' => TRUE,
  );

  $form['auth_ufrpe_cpa_e-learning_urls']['discente-ead'] = array(
    '#type' => 'textfield',
    '#title' => t('Discente EAD:'),
    '#required' => TRUE,
  );

  $form['auth_ufrpe_cpa_e-learning_urls']['tecnico-ead'] = array(
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
  /**
   * Analyze if the given urls are valid
   */

  // Set modalities and profiles
  $modalities = array('-presencial' => '_presential_', '-ead' => '_e-learning_');
  $profiles = array('docente', 'discente', 'tecnico');

  // Process urls given
  foreach ($modalities as $modality_suffix => $modality_fieldset) {
    foreach ($profiles as $profile) {
      $currentURL = $form['auth_ufrpe_cpa'.$modality_fieldset.'urls'][$profile.$modality_suffix]['#value'];
      if (url_is_external($currentURL)) {
        // URL must be internal
        drupal_set_message(t('Todos os links devem ser internos'), 'error');
        return;
      }
    }
  }

  // Get current settings and insert on database
  $record = array(
    'doc_pre' => $form['auth_ufrpe_cpa_presential_urls']['docente-presencial']['#value'],
    'dis_pre' => $form['auth_ufrpe_cpa_presential_urls']['discente-presencial']['#value'],
    'tec_pre' => $form['auth_ufrpe_cpa_presential_urls']['tecnico-presencial']['#value'],
    'doc_ead' => $form['auth_ufrpe_cpa_e-learning_urls']['docente-ead']['#value'],
    'dis_ead' => $form['auth_ufrpe_cpa_e-learning_urls']['discente-ead']['#value'],
    'tec_ead' => $form['auth_ufrpe_cpa_e-learning_urls']['tecnico-ead']['#value'],
    'ano' => date('Y'),
    'semestre' => getSemester((int) date('m')),
  );

  try {
    drupal_write_record('auth_ufrpe_settings', $record);
  } catch (Exception $e) {
    // Administer must not update settings more than
    // one time per semester
    drupal_set_message(t('Os links já foram atualizados este período. Por favor entrar em contato com o suporte do NTI-UFRPE.'), 'error');
    drupal_set_message(t('Contato: suporte.nti@ufrpe.br'), 'error');
  }
}
