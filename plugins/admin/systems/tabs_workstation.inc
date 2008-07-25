<?php

class worktabs extends tabs
{

  function worktabs($config, $data, $dn)
  {
    /* Save dn */
    $this->dn= $dn;
    $this->config= $config;

    foreach ($data as $tab){
      if($tab['CLASS'] == "printgeneric"){
        $this->by_name[$tab['CLASS']]= $tab['NAME'];
        $this->by_object[$tab['CLASS']]= new $tab['CLASS']($this->config, $this->dn,$this);
        $this->by_object[$tab['CLASS']]->parent= &$this;
      }else{
        $this->by_name[$tab['CLASS']]= $tab['NAME'];
        $this->by_object[$tab['CLASS']]= new $tab['CLASS']($this->config, $this->dn);
        $this->by_object[$tab['CLASS']]->parent= &$this;
      }
      /* Initialize current */
      if ($this->current == ""){
        $this->current= $tab['CLASS'];
      }
    }
  }

  function save_object($save_current= FALSE)
  {
    tabs::save_object($save_current);

    /* Assign current cn to all other plugins */
    $baseobject= $this->by_object['workgeneric'];
    foreach ($this->by_object as $key => $obj){
      $this->by_object[$key]->dn= $this->dn;
      $this->by_object[$key]->cn= $baseobject->cn;
    }
  }


  function save()
  {
    /* Check for new 'dn', in order to propagate the
       'dn' to all plugins */
    $baseobject= $this->by_object['workgeneric'];
    $this->dn= "cn=".$baseobject->cn.",ou=workstations,ou=systems,".$baseobject->base;
    $baseobject->dn= $this->dn;

    foreach ($this->by_object as $key => $obj){
      $this->by_object[$key]->dn= $this->dn;
      $this->by_object[$key]->cn= $baseobject->cn;
    }

    tabs::save(TRUE);

    if(isset($this->by_object['printgeneric'])){
      if(($this->by_object['printgeneric']->is_account==false)&&($this->by_object['printgeneric']->initially_was_account==true)){
        $this->by_object['printgeneric']->remove_from_parent();
      }
    }
  }

}

// vim:tabstop=2:expandtab:shiftwidth=2:filetype=php:syntax:ruler:
?>