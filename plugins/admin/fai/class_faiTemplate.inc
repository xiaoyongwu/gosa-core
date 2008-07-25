<?php

class faiTemplate extends plugin
{
  /* attribute list for save action */
  var $ignore_account   = TRUE;

  /* Attributes for this Object */
  var $attributes       = array("cn","description");

  /* ObjectClasses for this Object*/
  var $objectclasses    = array("top","FAIclass","FAItemplate");

  /* Class name of the Ldap ObjectClass for the Sub Object */
  var $subClass         = "FAItemplateEntry";
  var $subClasses       = array("top","FAIclass","FAItemplateEntry");

  /* Class name of the php class which allows us to edit a Sub Object */
  var $subClassName     = "faiTemplateEntry";      

  /* Attributes to initialise for each subObject */
  var $subAttributes    = array("cn","description","FAItemplatePath","FAIowner","FAImode"); 
  var $sub64coded       = array();
  var $sub_Load_Later   = array("FAItemplateFile");
  var $subBinary        = array("FAItemplateFile");

  /* Specific attributes */
  var $cn               = "";       // The class name for this object
  var $description      = "";       // The description for this set of partitions
  var $is_dialog        = false;    // specifies which buttons will be shown to save or abort
  var $dialog           = NULL;     // a dialog, e.g. new disk dialog
  var $SubObjects       = array();  // All leafobjects of this object

  var $FAIstate         = "";
  var $base             = "";
  var $release          = "";
  var $copy_paste_mode  = false;
  var $cut_paste_mode   = false;

  var $CopyPasteVars  = array("SubObjects");

  function faiTemplate ($config, $dn= NULL)
  {
    /* Load Attributes */
    plugin::plugin ($config, $dn);

    $this->acl="#all#";

    /* If "dn==new" we try to create a new entry
     * Else we must read all objects from ldap which belong to this entry.
     * First read SubObjects from ldap ... and then the partition definitions for the SubObjects.
     */
     if($dn != "new"){
      $this->dn =$dn;

      /* Set acls
       */
      $ui   = get_userinfo();
      $acl  = get_permissions ($this->dn, $ui->subtreeACL);
      $acli = get_module_permission($acl, "FAIclass", $this->dn);
      $this->acl=$acli;

      /* Get FAIstate
       */
      if(isset($this->attrs['FAIstate'][0])){
        $this->FAIstate = $this->attrs['FAIstate'][0];
      }

      /* Read all leaf objects of this object (For FAIscript this would be FAIscriptEntry)
       */
      $ldap     = $this->config->get_ldap_link();
      $ldap->cd ($this->dn);
      $ldap->search("(&(objectClass=FAIclass)(objectClass=".$this->subClass."))",array("dn"));

      $data = array();
      while($object = $ldap->fetch()){
        $data[] = $object;
      }
      foreach($data as $object){

        /* Set status for save management */
        $objects = array();
        $objects['status']      = "FreshLoaded";
        $objects['dn']          = $object['dn'];
        $objects                = $this->get_object_attributes($objects,$this->subAttributes);
        $this->SubObjects[$objects['cn']] = $objects;
      }
    }
  }


  /* Reload some attributes */
  function get_object_attributes($object,$attributes)
  {
    $ldap = $this->config->get_ldap_link();
    $ldap->cd($this->config->current['BASE']);
    $ldap->cat($object['dn'],$attributes);
    $tmp  = $ldap->fetch();

    foreach($attributes as $attrs){
      if(isset($tmp[$attrs][0])){
        $var = $tmp[$attrs][0];

        /* Check if we must decode some attributes */
        if(in_array_ics($attrs,$this->sub64coded)){
          $var = base64_decode($var);
        }

        /*  check if this is a binary entry */
        if(in_array_ics($attrs,$this->subBinary)){
          $var = $ldap->get_attribute($object['dn'], $attrs,$r_array=0);
        }

        /* Fix slashes */
        $var = addslashes($var);

        $object[$attrs] = $var;
      }
    }
    return($object);
  }


  function execute()
  {
	/* Call parent execute */
	plugin::execute();

    /* Fill templating stuff */
    $smarty= get_smarty();
    $display= "";

    /* New Listhandling
     */
    $once = true;
    foreach($_POST as $name => $value){
      if(preg_match("/^editscript_/",$name)&&($once)){
        $once = false;
        $entry = preg_replace("/^editscript_/","",$name);
        $entry = base64_decode(preg_replace("/_.*/","",$entry));

        $obj  = $this->SubObjects[$entry];
        if($obj['status'] == "FreshLoaded"){
          $obj  = $this->get_object_attributes($obj,$this->sub_Load_Later);
        }

        $this->dialog= new $this->subClassName($this->config,$this->dn,$obj);
        $this->dialog->acl = $this->acl;
        $_SESSION['objectinfo'] = $obj['dn'];
        $this->dialog->parent = &$this;
        $this->is_dialog=true;
      }
      if(preg_match("/^deletescript_/",$name)&&($once)){
        $once = false;
        $entry = preg_replace("/^deletescript_/","",$name);
        $entry = base64_decode(preg_replace("/_.*/","",$entry));

        $status = $this->SubObjects[$entry]['status'];
        if($status == "edited" || $status == "FreshLoaded"){
          $this->SubObjects[$entry]['status']= "delete";
        }else{
          unset($this->SubObjects[$entry]);
        }
      }
    }

    /* Edit entries via GET */
    if(isset($_GET['act']) && isset($_GET['id'])){
      if($_GET['act'] == "edit" && isset($this->SubObjects[$_GET['id']])){
        $obj = $this->SubObjects[$_GET['id']];
          if($obj['status'] == "FreshLoaded"){
          $obj  = $this->get_object_attributes($obj,$this->sub_Load_Later);
        }
        $this->dialog= new $this->subClassName($this->config,$this->dn,$obj);
        $this->dialog->acl = $this->acl;
        $_SESSION['objectinfo'] = $obj['dn'];
        $this->dialog->parent = &$this;
        $this->is_dialog=true;
      }
    }

    /* Add new sub object */
    if(isset($_POST['AddSubObject'])){
      $this->dialog= new $this->subClassName($this->config,"new");
      $this->dialog->acl = $this->acl;
      $this->dialog->parent = &$this;
      $this->is_dialog=true;
    }

    if($this->dn != "new"){
      $_SESSION['objectinfo']= $this->dn;
    }

    /* Save Dialog */
    if(isset($_POST['SaveSubObject'])){
      $this->dialog->save_object();
      $msgs = $this->dialog->check();
      if(count($msgs)>0){
        foreach($msgs as $msg){
          print_red($msg);
        }
      }else{

        /* Get return object */
        $obj = $this->dialog->save();
        if(isset($obj['remove'])){
          $old_stat = $this->SubObjects[$obj['remove']['from']]['status'];

          /* Depending on status, set new status */
          if($old_stat == "edited" || $old_stat == "FreshLoaded"){
            $this->SubObjects[$obj['remove']['from']]['status'] = "delete";
          }elseif($this->SubObjects[$obj['remove']['from']]['status']=="new"){
            unset($this->SubObjects[$obj['remove']['from']]);
          }
          $obj['status'] = "new";
          $this->SubObjects[$obj['remove']['to']] = $obj;
          unset($this->SubObjects[$obj['remove']['to']]['remove']);
        }else{
          if($obj['status'] == "FreshLoaded"){
            $obj['status'] = "edited";
          }
          $this->SubObjects[$obj['cn']]=$obj;
        }

        $this->is_dialog=false;
        unset($this->dialog);
        $this->dialog=NULL;
      }
    }

    /* Sort entries */
    $tmp = $keys = array();
    foreach($this->SubObjects as $key => $entry){
      $keys[$key]=$key;
    }
    natcasesort($keys);
    foreach($keys as $key){
      $tmp[$key]=$this->SubObjects[$key];
    }
    $this->SubObjects = $tmp;

    /* Cancel Dialog */
    if(isset($_POST['CancelSubObject'])){
      $this->is_dialog=false; 
      unset($this->dialog);
      $this->dialog=NULL;
    }

    /* Print dialog if $this->dialog is set */
    if($this->dialog){
      $this->dialog->save_object();
      $display = $this->dialog->execute();
      return($display);
    }

     /* Divlist Containing FAItemplates */
    $divlist = new divSelectBox("FAItemplates");
    $divlist->setHeight(400);
    if((chkacl($this->acl,"cn")!="") || ($this->FAIstate == "freeze")){
      $img_edit = "<input type='image' src='images/edit.png'      name='editscript_%s'    title='"._("edit")."' alt='"._("edit")."'>";
      $img_remo = "";
    }else{
      $img_edit = "<input type='image' src='images/edit.png'      name='editscript_%s'    title='"._("edit")."' alt='"._("edit")."'>";
      $img_remo = "<input type='image' src='images/edittrash.png' name='deletescript_%s'  title='"._("delete")."' alt='"._("delete")."'>";
    }

    foreach($this->getList(true) as $key => $name){

      if(($this->SubObjects[$key]['status'] == "new") || ($this->SubObjects[$key]['dn'] == "new")){
        $down = "";
      }else{
  
        $dn = $this->SubObjects[$key]['dn'];       
 
        $down = "<a href='getFAIscript.php?is_template&id=".base64_encode($dn)."'>
          <img src='images/save.png' alt='"._("Download")."' title='"._("Download")."' border=0>
          </a>";
      }

      $edit_link = "<a href='?plug=".$_GET['plug']."&act=edit&id=".$key."'>".$name."</a>";

      $divlist->AddEntry(array( array("string"=> $edit_link),
            array("string"=>$down , "attach" => "style='width:20px;'"),
            array("string"=>str_replace("%s",base64_encode($key),$img_edit.$img_remo),
              "attach"=>"style='border-right: 0px;width:50px;text-align:right;'")));
    }
    $smarty->assign("Entry_divlist",$divlist->DrawList());
    /* Divlist creation complete
     */

    $smarty->assign("SubObjects",$this->getList());

     /* Magic quotes GPC, escapes every ' " \, to solve some security risks
     * If we post the escaped strings they will be escaped again
     */
    foreach($this->attributes as $attrs){
      if(get_magic_quotes_gpc()){
        $smarty->assign($attrs,stripslashes($this->$attrs));
      }else{
        $smarty->assign($attrs,($this->$attrs));
      }
    }

    foreach($this->attributes as $attr){
      $smarty->assign($attr."ACL",chkacl($this->acl,$attr));
    }

    $display.= $smarty->fetch(get_template_path('faiTemplate.tpl', TRUE));
    return($display);
  }

  /* Generate listbox friendly SubObject list
  */
  function getList(){
    $a_return=array();
    foreach($this->SubObjects as $obj){
      if($obj['status'] != "delete"){
      
        if((isset($obj['description']))&&(!empty($obj['description']))){
          if(strlen($obj['description']) > 40){
            $obj['description'] = substr($obj['description'],0,40)."...";
          }
          $a_return[$obj['cn']]= $obj['cn']." [".stripslashes( $obj['description'] )."]";
        }else{
          $a_return[$obj['cn']]= $obj['cn'];
        }
      }
    }
    return($a_return);
  }

  /* Delete me, and all my subtrees
   */
  function remove_from_parent()
  {
    $ldap = $this->config->get_ldap_link();
    $ldap->cd ($this->dn);
    $ldap->rmdir_recursive($this->dn);
    $this->handle_post_events("remove");    
  }


  /* Save data to object 
   */
  function save_object()
  {
    if((isset($_POST['FAItemplate_posted'])) && ($this->FAIstate != "freeze") ){
      plugin::save_object();
      foreach($this->attributes as $attrs){
        if(isset($_POST[$attrs])){
          $this->$attrs = $_POST[$attrs];
        } 
      }
    }
  }


  /* Check supplied data */
  function check()
  {
    /* Call common method to give check the hook */
    $message= plugin::check();

    /* If this is a new script, check if a script with this name already exists */
    if(!empty($this->release) && ($this->copy_paste_mode || $this->cut_paste_mode) ){

      /* Check if current name is already used for fai scripts in selected release */
      $dn = 'cn='.$this->cn.",ou=templates,".$this->release;
      $ldap = $this->config->get_ldap_link();
      $ldap->cat($dn);
      if($ldap->count()){

        $r =convert_department_dn($this->release);;
        $message[] = sprintf(_("Can't insert a fai template named '%s' in '%s' there is already a template with the given name."),$this->cn,$r);
      }
    }
    return ($message);
  }


  /* Save to LDAP */
  function save()
  {
    plugin::save();

    $ldap = $this->config->get_ldap_link();

    /* Copy & Paste : Ensure that FAIstate is copied too */
    if($this->copy_paste_mode && preg_match("/freeze/",$this->FAIstate)){
      $this->attrs['FAIstate'] = $this->FAIstate;
    }

    $ldap->cat($this->dn,array("objectClass"));
    if($ldap->count()!=0){
      /* Write FAIscript to ldap*/
      $ldap->cd($this->dn);
      $this->cleanup();
      $ldap->modify ($this->attrs); 

    }else{
      /* Write FAIscript to ldap*/
      $ldap->cd($this->config->current['BASE']);
      $ldap->create_missing_trees(preg_replace('/^[^,]+,/', '', $this->dn));
      $ldap->cd($this->dn);
      $ldap->add($this->attrs);
    }
    show_ldap_error($ldap->get_error(), _("Saving FAI template base failed"));

    /* Prepare FAIscriptEntry to write it to ldap
     * First sort array.
     *  Because we must delete old entries first.
     * After deletion, we perform add and modify 
     */
    $Objects = array();

    /* We do not need to save untouched objects */
    foreach($this->SubObjects as $name => $obj){
      if($obj['status'] == "FreshLoaded"){
        if($this->copy_paste_mode){
          $this->SubObjects[$name] = $this->get_object_attributes($obj,$this->sub_Load_Later);
          $this->SubObjects[$name]['status'] = "new";
        }else{
          unset($this->SubObjects[$name]);
        }
      }
    }

    foreach($this->SubObjects as $name => $obj){
      if($obj['status'] == "delete"){
        $Objects[$name] = $obj; 
      }
    }

    foreach($this->SubObjects as $name => $obj){
      if($obj['status'] != "delete"){
        $Objects[$name] = $obj; 
      }
    }

    foreach($Objects as $name => $obj){

      foreach($this->sub64coded as $codeIt){
        $obj[$codeIt]=base64_encode($obj[$codeIt]);
      }
      $tmp = array();
      $attributes = array_merge($this->sub_Load_Later,$this->subAttributes);
      foreach($attributes as $attrs){
        if(empty($obj[$attrs])){
          $obj[$attrs] = array();
          $tmp[$attrs] = $obj[$attrs];
        }else{
          $tmp[$attrs] = stripslashes ($obj[$attrs]);
        }
      }    
        
      $tmp['objectClass'] = $this->subClasses;

      $sub_dn = "cn=".$obj['cn'].",".$this->dn;

      if($obj['status']=="new"){
        $ldap->cat($sub_dn,array("objectClass"));
        if($ldap->count()){
          $obj['status']="edited";
        }
      }

      /* Tag object */
      $this->tag_attrs(&$tmp, $sub_dn, $this->gosaUnitTag);

      if($obj['status'] == "delete"){
        $ldap->cd($sub_dn);
        $ldap->rmdir_recursive($sub_dn);
        $this->handle_post_events("remove");
        show_ldap_error($ldap->get_error(), _("Removing FAI template entry failed")); 
      }elseif($obj['status'] == "edited"){
        $ldap->cd($sub_dn);
        $this->cleanup();
        $ldap->modify ($tmp); 
        show_ldap_error($ldap->get_error(), _("Modifying FAI template entry failed")); 

        $this->handle_post_events("modify");
      }elseif($obj['status']=="new"){

        if($tmp['description']==array()){
          unset($tmp['description']);
        }
        $ldap->cd($this->config->current['BASE']);
        $ldap->create_missing_trees(preg_replace('/^[^,]+,/', '', $sub_dn));
        $ldap->cd($sub_dn);
        $ldap->add($tmp); 
        $this->handle_post_events("add");
        show_ldap_error($ldap->get_error(), _("Creating FAI template entry failed")); 
      }
    }
  }
  
  /* return copy & paste dialog
   */
  function getCopyDialog()
  {
    /* Ask for cn */
    $smarty = get_smarty();
    $smarty->assign("cn" ,$this->cn);
    $str = $smarty->fetch(get_template_path("paste_fai_object.tpl",TRUE));
    $ret = array();
    $ret['string'] = $str;
    $ret['status'] = "";
    return($ret);
  }

  /* Get posted cn */
  function saveCopyDialog()
  {
    if(isset($_POST['cn'])){
      $this->cn = $_POST['cn'];
    }
  }
}

// vim:tabstop=2:expandtab:shiftwidth=2:filetype=php:syntax:ruler:
?>