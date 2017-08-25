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
    '#title' => t('Senha'),
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
  // URL and data used for authentication
  $url = 'http://ava.ufrpe.br/login/token.php';
  $data = 'username='.$form['auth_ufrpe_username'].'&password='.$form['auth_ufrpe_password'];

  // Building server request
  $curl = curl_init($url);
  curl_setopt($curl, CURLOPT_POST, 1);
  curl_setopt($curl, CURLOPT_POSTFIELDS, $data);
  curl_setopt($curl, CURLOPT_FOLLOWLOCATION, 1);
  curl_setopt($curl, CURLOPT_HEADER, 0);
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

  $response = curl_exec($curl);
  var_dump($response);
  return $response;
}
