<?php

/*
  This code is part of GOsa (https://gosa.gonicus.de)
  Copyright (C) 2005 Guillaume Delecourt
  Copyright (C) 2005 Benoit Mortier

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

class pptpAccount extends plugin
{
  /* Definitions */
  var $plHeadline= "PPTP";
  var $plDescription= "This does something";

  /* attribute list for save action */
  var $attributes= array();
  var $objectclasses= array("pptpServerAccount");

  var $uid ="";

  function pptpAccount ($config, $dn= NULL, $parent= NULL)
  {
    plugin::plugin ($config, $dn, $parent);
    
    /* Setting uid to default */
    if(isset($this->attrs['uid'][0])){
      $this->uid = $this->attrs['uid'][0];
    }

  }

  function execute()
  {
	/* Call parent execute */
//	plugin::execute();

    /* Show tab dialog headers */
    $display= "";

    /* Show main page */
    $smarty= get_smarty();

    if ($this->is_account){
      $smarty->assign("pptpState", "checked");
    } else {
      $smarty->assign("pptpState", "");
      $smarty->assign("wstate", "disabled");
    }

    /* Allow account status toogle?  */
    $smarty->assign("gosapptpACL", "disabled");
    if(!$this->is_account && chkacl($this->acl,"create") == ""){
      $smarty->assign("gosapptpACL", "");
    }elseif($this->is_account && $this->acl == "#all#" ){
      $smarty->assign("gosapptpACL", "");
    }

    $display.= $smarty->fetch (get_template_path('pptp.tpl', TRUE, dirname(__FILE__)));
    return ($display);
  }

  function remove_from_parent()
  {
    if(chkacl($this->acl, 'gosapptp') == ""){
      /* Cancel if there's nothing to do here */
      if (!$this->initially_was_account){
        return;
      }

      plugin::remove_from_parent();
      $ldap= $this->config->get_ldap_link();

      $ldap->cd($this->dn);
      @DEBUG (DEBUG_LDAP, __LINE__, __FUNCTION__, __FILE__,
          $this->attributes, "Save");
      $this->cleanup();
      $ldap->modify ($this->attrs); 

      show_ldap_error($ldap->get_error(), _("Removing PPTP account failed"));

      /* Optionally execute a command after we're done */
      $this->handle_post_events('remove',array("uid" => $this->uid));
    }
  }


  /* Save data to object */
  function save_object()
  {
    /* Do we need to flip is_account state? */
    if (isset($_POST['connectivityTab'])){

      /* Change state if needed */
      if(!$this->is_account && isset($_POST["pptp"]) && chkacl($this->acl,"create") == ""){
        $this->is_account = true;
      }elseif($this->is_account && !isset($_POST["pptp"]) && $this->acl == "#all#"){
        $this->is_account = false;
      }
    }

    plugin::save_object();
    
    #FIXME seams to be unused code !
    if (isset($_POST["pptpStatus"])){
      $this->pptpStatus = "disabled";
    } else {
      $this->pptpStatus = "enabled";
    }
  }


  /* Save to LDAP */
  function save()
  {
    if(chkacl($this->acl, 'gosapptp') == ""){
      plugin::save();

      /* Write back to ldap */
      $ldap= $this->config->get_ldap_link();
      $ldap->cd($this->dn);
      $this->cleanup();
      $ldap->modify ($this->attrs); 

      show_ldap_error($ldap->get_error(), _("Saving PPTP account failed"));

      /* Optionally execute a command after we're done */
      if ($this->initially_was_account == $this->is_account){
        if ($this->is_modified){
          $this->handle_post_events("modify",array("uid" => $this->uid));
        }
      } else {
        $this->handle_post_events("add",array("uid" => $this->uid));
      }
    }
  }

}

// vim:tabstop=2:expandtab:shiftwidth=2:filetype=php:syntax:ruler:
?>