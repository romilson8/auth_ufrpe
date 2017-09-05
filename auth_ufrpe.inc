<?php

/**
 * Implementation of authentication form
 */

function auth_ufrpe_form($form, &$form_state)
{
  $form['auth_ufrpe_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Login'),
    '#required' => TRUE,
  );

  $form['auth_ufrpe_password'] = array(
    '#type' => 'password',
    '#title' => t('Senha de Serviços Integrados'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Autenticar'),
  );

  $form['#submit'][] = 'auth_ufrpe_submit';
  return $form;
}

/**
 * Implementation of server request
 */

function auth_ufrpe_submit($form, &$form_state)
{
  // URL and data used to get token
  $url = 'http://ava.ufrpe.br/login/token.php';
  $data = array(
    'username' => $form['auth_ufrpe_username']['#value'],
    'password' => $form['auth_ufrpe_password']['#value'],
    'service' => 'moodle_mobile_app',
  );
  $options = array(
    'data' => drupal_http_build_query($data),
    'method' => 'POST',
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
  );

  // Building server request
  /*$curl = curl_init($url);
  curl_setopt($curl, CURLOPT_POST, TRUE);
  curl_setopt($curl, CURLOPT_POSTFIELDS, http_build_query($data));
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
  $response = json_decode(curl_exec($curl));
  curl_close($curl);*/

  $response_body_encoded = drupal_http_request($url, $options)->data;
  $response_body_decoded = drupal_json_decode($response_body_encoded);
  if (!isset($response_body_decoded['token'])) {
    drupal_set_message(t('Login ou Senha incorretos, tente novamente.'), 'error');
    return;
  }
  get_user_info($response_body_decoded);
}

/**
 * Get user basic information
 *
 * @param $response
 *   The response from the function above (JSON)
 */

function get_user_info($response)
{
  // URL and data used to get user info
  $url = 'http://ava.ufrpe.br/webservice/rest/server.php?moodlewsrestformat=json';
  $data = array(
    'wsfunction' => 'core_webservice_get_site_info',
    'wstoken' => $response['token'],
  );
  $options = array(
    'data' => drupal_http_build_query($data),
    'method' => 'POST',
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
  );

  /*$curl = curl_init($url);
  curl_setopt($curl, CURLOPT_POST, TRUE);
  curl_setopt($curl, CURLOPT_POSTFIELDS, http_build_query($data));
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
  $subresponse = json_decode(curl_exec($curl));
  curl_close($curl);*/

  $subresponse_body_encoded = drupal_http_request($url, $options)->data;
  $subresponse_body_decoded = drupal_json_decode($subresponse_body_encoded);
  get_advanced_user_info($subresponse_body_decoded, $response);
}

/**
 * Get user detailed information
 *
 * @param $response
 *  The response of the first request (contains token)
 *
 * @param $subresponse
 *  The response of the second request (contains user id)
 */

function get_advanced_user_info($subresponse, $response)
{
  // URL and data used to get detailed information about the user
  $url = 'http://ava.ufrpe.br/webservice/rest/server.php?moodlewsrestformat=json';
  $data = array(
    'wsfunction' => 'core_user_get_users_by_id',
    'wstoken' => $response['token'],
    'userids[0]' => $subresponse['userid'],
  );
  $options = array(
    'data' => drupal_http_build_query($data),
    'method' => 'POST',
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
  );

  /*$curl = curl_init($url);
  curl_setopt($curl, CURLOPT_POST, TRUE);
  curl_setopt($curl, CURLOPT_POSTFIELDS, http_build_query($data));
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
  $fullresponse = json_decode(curl_exec($curl));
  curl_close($curl);*/

  $fullresponse_body_encoded = drupal_http_request($url, $options)->data;
  $fullresponse_body_decoded = drupal_json_decode($fullresponse_body_encoded);

  // drupal_set_message(t(($fullresponse_body_decoded[0]['fullname'])),'warning');

  // Recording authentication on database
  $record = array(
    'cpf' => $fullresponse_body_decoded[0]['idnumber'],
    'nome' => $fullresponse_body_decoded[0]['fullname'],
    'ano' => date('Y'),
    'semestre' => getSemester((int) date('m')),
    'orgao' => $fullresponse_body_decoded[0]['department'],
    'perfil' => 'DISCENTE',
    'modalidade' => 'PRESENCIAL',
  );

  try {
    drupal_write_record('auth_ufrpe', $record);
  } catch (Exception $e) {
    // In case of double authentication
    // at same period
    drupal_set_message(t('Usuário já autenticado neste período'),'error');
  }
}

/**
 * Get current semester
 *
 * @param $month
 *  The current month
 */

function getSemester($month)
{
  $semester = null;
  if ($month <= 6) {
    $semester = 1;
  } else {
    $semester = 2;
  }
  return $semester;
}
