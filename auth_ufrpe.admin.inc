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
  // Define what will be the paths of the CPA webforms

  $form['auth_ufrpe_cpa_presential_paths'] = array(
    '#type' => 'fieldset',
    '#title' => t('Caminhos para webforms CPA Presencial'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => 'Caminhos dos webforms para a modalidade presencial.',
  );

  $form['auth_ufrpe_cpa_presential_paths']['docente-presencial'] = array(
    '#type' => 'textfield',
    '#title' => t('Docente presencial:'),
    '#required' => TRUE,
  );

  $form['auth_ufrpe_cpa_presential_paths']['discente-presencial'] = array(
    '#type' => 'textfield',
    '#title' => t('Discente presencial:'),
    '#required' => TRUE,
  );

  $form['auth_ufrpe_cpa_presential_paths']['tecnico-presencial'] = array(
    '#type' => 'textfield',
    '#title' => t('Técnico presencial:'),
    '#required' => TRUE,
  );

  $form['auth_ufrpe_cpa_e-learning_paths'] = array(
    '#type' => 'fieldset',
    '#title' => t('Caminhos para webforms CPA EAD'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => 'Caminhos dos webforms para a modalidade EAD.'
  );

  $form['auth_ufrpe_cpa_e-learning_paths']['docente-ead'] = array(
    '#type' => 'textfield',
    '#title' => t('Docente EAD:'),
    '#required' => TRUE,
  );

  $form['auth_ufrpe_cpa_e-learning_paths']['discente-ead'] = array(
    '#type' => 'textfield',
    '#title' => t('Discente EAD:'),
    '#required' => TRUE,
  );

  $form['auth_ufrpe_cpa_e-learning_paths']['tecnico-ead'] = array(
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
  if (validate_settings_paths($form)) {
    // Get current settings and insert on database
    $record = array(
      'doc_pre' => $form['auth_ufrpe_cpa_presential_paths']['docente-presencial']['#value'],
      'dis_pre' => $form['auth_ufrpe_cpa_presential_paths']['discente-presencial']['#value'],
      'tec_pre' => $form['auth_ufrpe_cpa_presential_paths']['tecnico-presencial']['#value'],
      'doc_ead' => $form['auth_ufrpe_cpa_e-learning_paths']['docente-ead']['#value'],
      'dis_ead' => $form['auth_ufrpe_cpa_e-learning_paths']['discente-ead']['#value'],
      'tec_ead' => $form['auth_ufrpe_cpa_e-learning_paths']['tecnico-ead']['#value'],
      'ano' => date('Y'),
      'semestre' => getSemester((int) date('m')),
    );

    try {
      // Writing record on auth_ufrpe_settings database
      drupal_write_record('auth_ufrpe_settings', $record);
    } catch (Exception $e) {
      // Administer must not update settings more than
      // one time per semester
      drupal_set_message(t('Os caminhos já foram atualizados este período. Por favor entrar em contato com o suporte do NTI-UFRPE.'), 'error');
      drupal_set_message(t('Contato: suporte.nti@ufrpe.br'), 'error');
    }
  }
}

/**
 * Analyze if the given paths are valid
 *
 * @return bool
 *  TRUE if the settings paths given are valid
 *  FALSE otherwise
 */

function validate_settings_paths($form)
{
  // Set modalities and profiles
  $modalities = array('presencial' => '_presential_', 'ead' => '_e-learning_');
  $profiles = array('docente', 'discente', 'tecnico');

  // Process paths given
  foreach ($modalities as $modality_suffix => $modality_fieldset) {
    foreach ($profiles as $profile) {
      // Path that will be writed on database (possible Alias)
      $current_path = $form['auth_ufrpe_cpa'.$modality_fieldset.'paths'][$profile.'-'.$modality_suffix]['#value'];
      // Path registered in Drupal system (node/*)
      $current_normal_path = drupal_get_normal_path($current_path);

      if (!drupal_valid_path($current_normal_path)) {
        // Path must be internal and valid
        drupal_set_message(t('Caminho para '.$profile.' '.$modality_suffix.' inválido ou incorreto.'), 'error');
        drupal_set_message(t('O caminho deve dar em um conteúdo do tipo Webform.'),'error');
        return FALSE;
      }

      // Getting node type
      $nid = (int) explode('/', $current_normal_path)[1];
      $node = db_select('node', 'n')->fields('n')
                                    ->condition('nid', $nid)
                                    ->execute()
                                    ->fetchAssoc();
      if ($node['type'] != 'webform') {
        // Path must lead to a webform content
        drupal_set_message(t('O conteúdo do caminho de '.$profile.' '.$modality_suffix.' deve ser do tipo webform.'),'error');
        return FALSE;
      }
    }
  }

  return TRUE;
}
