<?php

 
function create_user ($name, $email) {
 
  $password = user_password(8);
 
  //set up the user fields
  $fields = array(
    'name' => $name,
    'mail' => $email,
    'pass' => $password,
    'status' => 1,
    'init' => 'email address',
   // 'picture' => $picture,
    'roles' => array(
      DRUPAL_AUTHENTICATED_RID => 'authenticated user',
    ),
  );
 
  //the first parameter is left blank so a new user is created
  $account = user_save('', $fields);
 
  // If you want to send the welcome email, use the following code
 
  // Manually set the password so it appears in the e-mail.
  $account->password = $fields['pass'];
 
  // Send the e-mail through the user module.
  drupal_mail('user', 'register_no_approval_required', $fields['mail'], NULL, array('account' => $account), variable_get('site_mail', 'noreply@example..com'));
  
  drupal_set_message("User ". $name . " created");
  
    //login
    $uid = user_authenticate($name, $password);
    global $user;
    $user = user_load($uid);
    
    //login finalize
    watchdog('remote_user', 'Session opened for %name.', array('%name' => $user->name));
    $user->login = REQUEST_TIME;
    
    db_update('users')
    ->fields(array('login' => $user->login))
    ->condition('uid', $user->uid)
    ->execute();
    
    drupal_session_regenerate();

}
 
/**
 * Tutorial donate 1.
 *
 * This first form function is from the
 * @link http://drupal.org/node/717722 Form Tutorial handbook page @endlink
 *
 * It just creates a very basic form with a textfield.
 *
 * This function is called the "form constructor function". It builds the form.
 * It takes a two arguments, $form and $form_state, but if drupal_get_form()
 * sends additional arguments, they will be provided after $form_state.
 *
 * @ingroup form_donate
 */



function alumni_donate_form($form, &$form_state) {
    global $user;
    $is_registered = false;
    $account = user_load($user->uid);
    $u_name = "";
    $u_mail ="";
    $u_picture = null;
    $u_ulogin_picture = null;
  
    if ($user->uid) {
        $u_name = $user->name;
        $u_mail = $user->mail;
        $u_picture = $user->picture;
        $is_registered = true;
        
        if (module_exists('ulogin')) {
            $identities = _ulogin_identity_load_by_uid($user->uid);
            if ($identities) {
                foreach ($identities as $identity) {
                    $data = unserialize($identity['data']);
                    dsm($data);
                }
                
                if ($data['first_name'] && $data['last_name']) {
                    $u_name = $data['first_name'] . " " . $data['last_name'];
                }
                
                if ($data['photo_big']) {
                    $u_ulogin_picture = $data['photo_big'];
                }
            }
        }

    } else {
        print_r("non");
    }
    

  

  

    
    $form['description'] = array(
        '#type' => 'item',
        '#title' => 'A simple form with a submit button:',
    );
    
    $form['is_new'] = array(
    '#type' => 'hidden',
    '#disabled' => true,
    '#default_value' => !$is_registered,
    );

    $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Surname & name (username)'),
    '#required' => TRUE,
    '#default_value' => $u_name,
    '#disabled' => $is_registered,
    );
  
    $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#required' => TRUE,
    '#default_value' => $u_mail,
    '#description' => "Please enter your email.",
    '#size' => 20,
    '#maxlength' => 20,
    '#disabled' => $is_registered,
    );
    
  //  if (module_exists('ulogin') && !user_is_logged_in()) 

  if ($u_picture) {
    $form['avatar'] = array(
        '#type' => 'markup',
        '#markup' => theme('user_picture', array('account' => $account)),
    );
  } 
  
  if ($u_ulogin_picture) {
      $form['avatar'] = array(
            '#type' => 'markup',
            '#markup' => '<img src="'. $u_ulogin_picture .'" />',
      );  
  } else {
    // Attach User Picture upload
   
    $form['profile'] = array(
        '#type' => 'file',
        '#title' => t('Upload picture'),
        '#size' => 48,
        '#description' => 'Some description',

    );
  }

  // Adds a simple submit button that refreshes the form and clears its
  // contents. This is the default behavior for forms.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}

//function form_donate_tutorial_2_email_element_validate($element, &$form_state, $form) {
//   if (!valid_email_address($element['#value'])) {
//     form_error($element, t('Please enter a valid email address.'));
//   }
//}

function alumni_donate_form_validate($form, &$form_state) {
    $email = $form_state['values']['email'];
    if (!valid_email_address($email)) {
        form_set_error('email', t('Please enter a valid email address.'));
    }
    
    $file = file_save_upload('profile', array(
      'file_validate_is_image' => array(),
      'file_validate_extensions' => array('png gif jpg jpeg'),
  ));
  
  if ($file) {
    if ($file = file_move($file, 'public://')) {
      $form_state['values']['profile'] = $file;
    }
    else {
      form_set_error('profile', t('Failed to write the uploaded file the site\'s file folder.'));
    }
  }
  else {
    form_set_error('profile', t('No file was uploaded.'));
  }
}

function alumni_donate_form_submit($form, &$form_state) {
    $name = $form_state['values']['name'];
    $email = $form_state['values']['email'];
    $is_new = $form_state['values']['is_new'];
  
    if ($is_new) {
        create_user($name, $email);

        global $user;
        $uid = $user->uid;
        $account = user_load($uid);

        $file = $form_state['values']['profile'];
        unset($form_state['values']['profile']);
        $file->status = FILE_STATUS_PERMANENT;
        file_save($file);

        $edit['picture'] = $file;
        user_save($account, $edit);
    }
  
  drupal_set_message($name);
}
