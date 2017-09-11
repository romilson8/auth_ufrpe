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
    '#description' => 'Links dos webforms para a modalidade presencial.',
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
    '#description' => 'Links dos webforms para a modalidade EAD.'
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
  if (validate_settings_URLs($form)) {
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
      // Writing record on auth_ufrpe_settings database
      drupal_write_record('auth_ufrpe_settings', $record);
    } catch (Exception $e) {
      // Administer must not update settings more than
      // one time per semester
      drupal_set_message(t('Os links já foram atualizados este período. Por favor entrar em contato com o suporte do NTI-UFRPE.'), 'error');
      drupal_set_message(t('Contato: suporte.nti@ufrpe.br'), 'error');
    }
  }
}

/**
 * Analyze if the given urls are valid
 *
 * @return bool
 *  TRUE if the settings urls given are valid
 *  FALSE otherwise
 */

function validate_settings_URLs($form)
{
  // Set modalities and profiles
  $modalities = array('presencial' => '_presential_', 'ead' => '_e-learning_');
  $profiles = array('docente', 'discente', 'tecnico');

  // Process urls given
  foreach ($modalities as $modality_suffix => $modality_fieldset) {
    foreach ($profiles as $profile) {
      // URL that will be writed on database
      $currentURL = $form['auth_ufrpe_cpa'.$modality_fieldset.'urls'][$profile.'-'.$modality_suffix]['#value'];
      // Path of the URL
      $current_normal_path = drupal_get_normal_path($currentURL);

      if (!drupal_valid_path($current_normal_path)) {
        // URL must be internal and valid
        drupal_set_message(t('Link para '.$profile.' '.$modality_suffix.' inválido.'), 'error');
        drupal_set_message(t('O link deve ser interno e válido (webform já existente)'),'error');
        return FALSE;
      }

      // Getting node type
      $nid = (int) explode('/', $current_normal_path)[1];
      $node = db_select('node', 'n')->fields('n')
                                    ->condition('nid', $nid)
                                    ->execute()
                                    ->fetchAssoc();
      if ($node['type'] != 'webform') {
        // URL must lead to a webform content
        drupal_set_message(t('O conteúdo do link de '.$profile.' '.$modality_suffix.' deve ser do tipo webform.'),'error');
        return FALSE;
      }
    }
  }

  return TRUE;
}
