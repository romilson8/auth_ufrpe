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
      'callback' => 'return_info',
      'method' => 'POST',

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
  // URL and data used for authentication
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
  //curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
  $response = curl_exec($curl);
  curl_close($curl);
  echo "<script>console.log(".$response.");</script>";
  return $response;
}
