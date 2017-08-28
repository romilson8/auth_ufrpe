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
    '#ajax' => array(
      'callback' => 'auth_ufrpe_submit',
    ),
  );

  $form['#submit'][] = 'auth_ufrpe_submit';
  return $form;
}

/**
 * Implementation of server request
 */

function auth_ufrpe_submit($form, &$form_state)
{
  if ($form['auth_ufrpe_username']['#value'] == '' || $form[auth_ufrpe_password]['#value'] == '') {
    echo "<script>alert('Campos Login e Senha requeridos para autenticação.');</script>";
    return;
  }
  // URL and data used to get token
  $url = 'http://ava.ufrpe.br/login/token.php';
  $data = array(
    'username' => $form['auth_ufrpe_username']['#value'],
    'password' => $form['auth_ufrpe_password']['#value'],
    'service' => 'moodle_mobile_app',
  );

  // Building server request
  $curl = curl_init($url);
  curl_setopt($curl, CURLOPT_POST, TRUE);
  curl_setopt($curl, CURLOPT_POSTFIELDS, http_build_query($data));
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
  $response = json_decode(curl_exec($curl));
  curl_close($curl);
  if (isset($response->token)) {
    get_user_info($response);
  } else {
    echo "<script>alert('Login ou Senha incorretos. Tente novamente.');</script>";
    return;
  }
}

function get_user_info($response)
{
  // URL and data used to get user info
  $url = 'http://ava.ufrpe.br/webservice/rest/server.php?moodlewsrestformat=json';
  $data = array(
    'wsfunction' => 'core_webservice_get_site_info',
    'wstoken' => $response->token,
  );

  $curl = curl_init($url);
  curl_setopt($curl, CURLOPT_POST, TRUE);
  curl_setopt($curl, CURLOPT_POSTFIELDS, http_build_query($data));
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
  $subresponse = json_decode(curl_exec($curl));
  curl_close($curl);
  get_advanced_user_info($subresponse, $response);
}

function get_advanced_user_info($subresponse, $response)
{
  // URL and data used to get detailed information about the user
  $url = 'http://ava.ufrpe.br/webservice/rest/server.php?moodlewsrestformat=json';
  $data = array(
    'wsfunction' => 'core_user_get_users_by_id',
    'wstoken' => $response->token,
    'userids[0]' => $subresponse->userid,
  );

  $curl = curl_init($url);
  curl_setopt($curl, CURLOPT_POST, TRUE);
  curl_setopt($curl, CURLOPT_POSTFIELDS, http_build_query($data));
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
  $fullresponse = json_decode(curl_exec($curl));
  echo "<script>alert(".var_dump($fullresponse[0]).");</script>";
  curl_close($curl);
}
