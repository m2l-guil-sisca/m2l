<?php 
namespace MRBS;

use \phpCAS;
use MRBS\Form\Form;

require_once MRBS_ROOT . '/session/functions_session.inc';


function authGet()
{
  // Useless Method
}


function getUserName()
{
  return (phpCAS::isAuthenticated()) ? phpCAS::getUser() : null;
}


// Print the logon entry on the top banner.
function print_logon_box()
{
  global $QUERY_STRING;
  
  $target_url = this_page();

  if (isset($QUERY_STRING))
  {
    $target_url = $target_url . "?" . $QUERY_STRING;
  }
  
  $user = getUserName();
  
  if (isset($user))
  {
    print_report_link($user);
    print_logoff_button($target_url);
  }
  else
  {
    print_unknown_user();
    print_logon_button($target_url);
  } 
  
  print_userlist_link();
}


function logoff_user()
{
  phpCAS::logout();
}


global $HTTP_REFERER;

// Get non-standard form variables
$action = get_form_var('action', 'string', null, INPUT_POST);
$target_url = get_form_var('target_url', 'string', null, INPUT_POST);
$returl = get_form_var('returl', 'string', null, INPUT_POST);

if (isset($action))
{
  // Target of the form with sets the URL argument "action=QueryName".
  if ($action == "QueryName")
  {
    phpCAS::forceAuthentication();
  }
  
  
  // Target of the form with sets the URL argument "action=SetName".
  // Will eventually return to URL argument "target_url=whatever".
  if ($action == "SetName")
  {
    // If we're going to do something then check the CSRF token first
    Form::checkToken();
    
    // You should only get here using CAS authentication after clicking the logoff
    // link, no matter what the value of the form parameters.
    logoff_user();
    
    header ("Location: $target_url"); /* Redirect browser to initial page */
    exit;
  }
}
