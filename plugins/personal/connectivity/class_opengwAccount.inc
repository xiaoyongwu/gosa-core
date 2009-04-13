<?php

class opengwAccount extends plugin
{
	/* Definitions */
	var $plHeadline       = "Opengroupware account";
	var $plDescription    = "This does something";

	/* attribute list for save action */
	var $attributes       = array();
	var $objectclasses    = array();

  var $SetPassword   = false;

	/* Attribute mapping opengroupware->ldap  */
	var $attrsToUse    = array( "salutation"  =>"vocation",
			"name"        =>"sn",
			"firstname"   =>"givenName",
			"login"       =>"uid",
			"degree"      =>"academicTitle",
			"birthday"    =>"dateOfBirth",
			"sex"         =>"gender",
			"street"      =>"street",
			"zip"         =>"postalCode",
			"value_string"=>"mail",
			"number"      =>"telephoneNumber"
			);

	var $serverCon            =  false;
	var $handle               = NULL;
	var $is_account           = false;
	var $initialy_was_account = false;
  var $OGWPassword          = "";
  var $uid ="";

	function opengwAccount ($config, $dn= NULL, $parent= NULL)
	{
		plugin::plugin ($config, $dn, $parent);

    /* Setting uid to default */
    if(isset($this->attrs['uid'][0])){
      $this->uid = $this->attrs['uid'][0];
    }

		$this->info=array();

		/* is no account and was no account */
		$this->initialy_was_account = false;
		$this->is_account           = false;

		/* check if datebase funktions are available, and if database configurations are available */ 
		if(is_callable("pg_connect")){

			/* Get configurations */
			if(search_config($this->config->data,"opengwAccount","USERNAME")){
				$atr = array();
				$atr['LOGIN']	=	search_config($this->config->data,"opengwAccount","USERNAME");
				$atr['PASSWORD']=	search_config($this->config->data,"opengwAccount","PASSWORD");
				$atr['SERVER']	=	search_config($this->config->data,"opengwAccount","DATAHOST");
				$atr['DB']		=	search_config($this->config->data,"opengwAccount","DATABASE");
				$this->serverCon = $atr;
			}else{
				$this->serverCon  = $this->config->data['SERVERS']['OPENGROUPWARE'];
			}
			$this->handle     = new ogw($this->serverCon['LOGIN'],$this->serverCon['PASSWORD'],$this->serverCon['SERVER'],$this->serverCon['DB']);

			/* Check if current configuration allows database connection */
			if($this->handle->connected){

				/* Set login name, to check if this is_account */
				if(isset($this->attrs['uid'][0])){
					$this->handle->info['login'] = $this->attrs['uid'][0];
				}else{
					$this->handle->info['login'] = ""; 
				}

				/* If this is account get data from database first */
				if($this->handle->CheckExistence()){
					$this->info = $this->handle->GetInfos($this->attrs['uid'][0]);

					/* This is an account */
					$this->initialy_was_account = true;
					$this->is_account= true;
				}else{
					/* this is no account */ 
					$this->initialy_was_account = false;
					$this->is_account= false;

					/* Selectable in GOsa */
					$this->info['template_user_id']   = 0;
					$this->info['is_locked']          = 0;
					$this->info['LocationTeamID']     = 0;
					$this->info['TeamIDis']           = array();
				}

				/* Set settings from ldap */
				foreach($this->attrsToUse as $name=>$attr){
					if(isset($this->attrs[$attr][0])){
						$this->info[$name] = $this->attrs[$attr][0];
					}else{
						$this->info[$name] = false;
					}
				} 

				/* Description is displayed as 'Nickname' */
				$this->info['description']          = $this->info['login'];

				if($this->info['sex'] == "F"){
					$this->info['sex']                = "female";
				}else{
					$this->info['sex']                = "male";
				}

			}

			/* Transmit data back to ogw handle */
			$this->handle->SetInfos($this->info);
		}
	}

	function execute()
	{
		/* Show tab dialog headers */
		$display= "";
		$smarty= get_smarty();

		/* set default values */
		foreach(array("validLocationTeam","validTemplateUser","validLocationTeams","validTemplateUsers") as $ar){
			$smarty->assign($ar,array());
		}
		$smarty->assign("OGWstate"," disabled ");
		foreach(array("LocationTeam","TemplateUser","is_locked","validTeams","is_account"/*,"OGWPassword"*/) as $ar){
			$smarty->assign($ar,"");
			$smarty->assign($ar."CHK","");
			$smarty->assign($ar."ACL"," disabled ");
		}

    /* Allow account status toogle?  */
    $smarty->assign("opengwAccountACL", "disabled");
    if(!$this->is_account && chkacl($this->acl,"create") == ""){
      $smarty->assign("opengwAccountACL", "");
    }elseif($this->is_account && $this->acl == "#all#" ){
      $smarty->assign("opengwAccountACL", "");
    }

		/* Check database extension */    
		if(!is_callable("pg_connect")){
			print_red(_("OpenGroupware: Your configuration is missing a postgresql extension. Can't perform any database queries."));
		}else

			/* Check if config exists */
			if(search_config($this->config->data,"opengwAccount","DATABASE")==""){
				print_red(_("OpenGroupware: Missing database configuration for opengroupware. Can't get or set any informations."));
			}else{

				/* Create handle */
				$this->handle     = new ogw($this->serverCon['LOGIN'],$this->serverCon['PASSWORD'],$this->serverCon['SERVER'],$this->serverCon['DB']);

				/* Check if we are successfully connected to db */
				if(!$this->handle->connected){
					print_red(_("OpenGroupware: Can't connect to specified database. Please check given configuration twice."));
				}else{

					/* Show main page */
					$smarty->assign("OGWstate"," disabled ");
					foreach(array("LocationTeam","TemplateUser","is_locked","validTeams","opengwAccount"/*,"OGWPassword"*/) as $ar){
						$smarty->assign($ar."ACL",chkacl($this->acl,$ar));
					}

					/* Assign LocationTeams */
					$tmp = array(""=>"none");
					foreach($this->handle->validLocationTeam as $id){
						$tmp[$id['company_id']] = $id['description']; 
					}
					$smarty->assign("validLocationTeam",$tmp);
					$smarty->assign("validLocationTeams",array_flip($tmp));
					$smarty->assign("LocationTeam",$this->info['LocationTeamID']);

					/* Assign TemplateUsers*/
					$tmp = array();
					foreach($this->handle->validTemplateUser as $id){
						$tmp[$id['company_id']] = $id['name']; 
					}
					$smarty->assign("validTemplateUser",$tmp);
					$smarty->assign("validTemplateUsers",array_flip($tmp));
					$smarty->assign("TemplateUser",$this->info['template_user_id']);

					/* Create Team membership */ 
					$str = "";
					if(($this->is_account)&&(chkacl($this->acl,"TeamIDs")=="")){
						$dis = "";
					}else{
						$dis = " disabled ";
					}

					foreach($this->handle->validTeams as $id){
						if(in_array($id['company_id'],$this->info['TeamIDis'])){
							$str .= "<input ".$dis." type='checkbox' value='".$id['company_id']."' 
								checked name='team_".base64_encode($id['description'])."'>".$id['description']."<br>";
						}else{
							$str .= "<input ".$dis." type='checkbox' value='".$id['company_id']."' 
								name='team_".base64_encode($id['description'])."'>".$id['description']."<br>";
						}
					}
					$smarty->assign("validTeams",$str);

					/* Assign ogw checkbox */
					if($this->is_account){
						$smarty->assign("OGWstate","");
						$smarty->assign("is_account", " checked ");
					}else{
						$smarty->assign("is_account", " ");
						$smarty->assign("OGWstate"," disabled ");
					}

					/* Assign acls */
					foreach(array("LocationTeam","TemplateUser","Teams","is_locked") as $atr){
						$smarty->assign($atr."ACL",chkacl($this->acl,$atr));
					}

					/* Assign lock status */
					if($this->info['is_locked']){
						$smarty->assign("is_lockedCHK", " checked ");
					}else{
						$smarty->assign("is_lockedCHK", "  ");
					}
					$smarty->assign("is_locked", $this->info['is_locked']);

				} 
			}
      /*
      if($this->SetPassword == false){
        if($_SESSION['js'] == 1){
          $smarty->assign("OGWPasswordACL"," disabled ");
        }else{
          $smarty->assign("OGWPasswordACL","");
        }
        $smarty->assign("OGWPassword","");
        $smarty->assign("OGWPasswordCHK"," ");
      }else{
        $smarty->assign("OGWPasswordACL","");
        $smarty->assign("OGWPassword",$this->OGWPassword);
        $smarty->assign("OGWPasswordCHK"," checked ");
      }
      */
		$display.= $smarty->fetch (get_template_path('opengw.tpl', TRUE, dirname(__FILE__)));
		return ($display);
	}

	function remove_from_parent()
	{
		/* remove database entries */
		if($this->initialy_was_account){
			$this->handle = new ogw($this->serverCon['LOGIN'],$this->serverCon['PASSWORD'],$this->serverCon['SERVER'],$this->serverCon['DB']);
			$this->handle->SetInfos($this->info);
			$this->handle->Perform("REMOVE");
		}
	}


	/* Save data to object */
	function save_object()
	{
		/* get post data */
		if($this->is_account){

      /*
      if(isset($_POST['SetPassword'])){
        $this->SetPassword =true;
      }else{
        $this->SetPassword = false;
      }

      if((isset($_POST['OGWPassword']))&&($this->SetPassword)){
        $this->OGWPassword      = $_POST['OGWPassword'];
        $pwd = new passwordMethodCrypt($this->config);
        
        $this->info['password'] = preg_replace("/^{crypt}/","",$pwd->generate_hash($_POST['OGWPassword']));
      }else{
        if(isset($this->info['password'])){
          unset($this->info['password']);
        }
      }
      */

			/* Get selected team chkboxes */
			$this->info['TeamIDis'] = array();
			foreach($_POST as $name => $value ){
				if(preg_match("/team_/i",$name)){
					if(!in_array($value,$this->info['TeamIDis'])){
						$this->info['TeamIDis'][]=$value;
					}
				}
			}

			/* Get location Team*/
			if(isset($_POST['LocationTeam'])){
				$this->info['LocationTeamID'] = $_POST['LocationTeam'];
			}

			/* Get template user */
			if(isset($_POST['TemplateUser'])){
				$this->info['template_user_id'] = $_POST['TemplateUser'];
			}

			/* get lock status */
			if(isset($_POST['is_locked'])){
				$this->info['is_locked'] = $_POST['is_locked'];
			}else{
				$this->info['is_locked'] = 0;
			}
		}

    /* Change state if needed */
    if(!$this->is_account && isset($_POST["is_account"]) && chkacl($this->acl,"create") == ""){
      $this->is_account = true;
    }elseif($this->is_account && !isset($_POST["is_account"]) && $this->acl == "#all#"){
      $this->is_account = false;
    }
	}


	/* Save to LDAP */
	function save()
	{
		/* Save data */ 
		$this->handle = new ogw($this->serverCon['LOGIN'],$this->serverCon['PASSWORD'],$this->serverCon['SERVER'],$this->serverCon['DB']);
		$this->handle->SetInfos($this->info);
		$this->handle->checkInfos();
		if($this->initialy_was_account){
			$this->handle->Perform("EDIT");
		}else{
			$this->handle->Perform("ADD");
		}
	}

  
  function PrepareForCopyPaste($src){
    // Nothing to do, because this object can't be copied ... yet
  }

}
// vim:tabstop=2:expandtab:shiftwidth=2:filetype=php:syntax:ruler:
?>