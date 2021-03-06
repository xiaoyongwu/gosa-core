<?php
/*
 * This code is part of GOsa (http://www.gosa-project.org)
 * Copyright (C) 2003-2008 GONICUS GmbH
 *
 * ID: $$Id$$
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

require_once("class_setupStep.inc");


class setup 
{
    var $i_steps  = 9;  // Number of setup steps 
    var $i_current= 1;  // Current step
    var $i_last   = 1;  // Last setup step;
    var $o_steps  = array(); 
    var $captured_values = array();

    function setup()
    {
        $i = 1; 
        $this->o_steps[$i++] = new Step_Welcome();
#    $this->o_steps[$i++] = new Step_Language();
        $this->o_steps[$i++] = new Step_Checks();
        $this->o_steps[$i++] = new Step_License();
        $this->o_steps[$i++] = new Step_Ldap();
    $this->o_steps[$i++] = new Step_Schema();
        $this->o_steps[$i++] = new Step_Migrate();
        $this->o_steps[$i++] = new Step_Feedback();
        $this->o_steps[$i++] = new Step_Finish();
        $this->i_steps = $i-1;

        /* Ensure that setup is not reachable if gosa.conf (CONFIG_FILE) */
        if(file_exists(CONFIG_DIR."/".CONFIG_FILE)){
            session::destroy();
            header("Location: index.php")    ;
            exit();
        }

        foreach($this->o_steps as $key => $step){
            $this->o_steps[$key]->parent = &$this;
        }
    }

    function execute()
    {
        /* Display phpinfo() dialog when $_GET['info'] is set,
         *  but only do this, if user is allowed to use the setup.
         * If setupStep_Welcome is_completed, we are allowed to view those infos-
         */
        if(isset($_GET['info']) &&  preg_match("/Step_Welcome/i",get_class($this->o_steps[1])) && $this->o_steps[1]->is_completed()){
            phpinfo();
            exit();
        }

        /* display step error msgs */
        $msgs = $this->o_steps[$this->i_current]->check();
        foreach($msgs as $msg){
            msg_dialog::display(_("Setup error"), $msg, ERROR_DIALOG);
        }

        $this->o_steps[$this->i_last]->set_active(FALSE);
        $this->o_steps[$this->i_current]->set_active();
        $content = $this->o_steps[$this->i_current]->execute();
        return($content);
    }


    /* Save posted attributes  */
    function save_object()
    {
        /* Call save_object for current setup step */
        $this->o_steps[$this->i_current] -> save_object();

        /* Get attributes from setup step */
        $tmp = $this->o_steps[$this->i_current]->get_attributes();
        foreach($tmp as $name => $value){
            $this->captured_values[$name] = $value;
        }

        /* Set parent */
        foreach($this->o_steps as $key => $value){
            $this->o_steps[$key]->parent = $this;
        }

        /* Check if image button requests next page */
        foreach($_POST as $name => $value){
            if(preg_match("/^next_(x|y)/",$name)){
                $_POST['next'] = TRUE;
            }
            if(preg_match("/^last_(x|y)/",$name)){
                $_POST['last'] = TRUE;
            }
        }

        /* Check if step was selected */
        if(isset($_GET['step']) || isset($_POST['next']) || isset($_POST['last'])){

            /* check if current setup step is completed now 
               and activate the next step if possible */
            for($i = 1 ; $i <= $this->i_steps ; $i ++ ){
                if($this->o_steps[$i]->is_completed()){
                    if(isset($this->o_steps[($i+1)])){
                        $this->o_steps[($i+1)]->set_enabled();
                    }
                }else{
                    $this->disable_steps_from($i+1);
                }
            }
        }

        /* Disable all following steps, if one step isn't compelted right now .*/
        for($i = 1 ; $i <= $this->i_steps ; $i ++ ){
            if($this->o_steps[$i]->is_completed()){
            }else{
                $this->disable_steps_from($i+1);
            }
        }

        $step = -1;

        if(isset($_POST['setup_goto_step'])){
            $step= $_POST['setup_goto_step'];
        }

        if(isset($_GET['step'])){
            $step = $_GET['step'];
        }elseif(isset($_POST['next'])){
            $step = $this->i_current + 1;
        }elseif(isset($_POST['last'])){
            $step = $this->i_current - 1;
        }

        $once = true;
        foreach($_POST as $name => $value){
            if(preg_match("/^step_[0-9]*$/",$name) && $once ){
                $step = preg_replace("/^step_/","",$name);
            }
        }

        if($this->selectable_step($step)){
            $this->i_last    = $this->i_current;
            $this->i_current = $step;
        }
    }


    function disable_steps_from($start)
    {
        $found = false;
        foreach($this->o_steps as $key => $step){
            if($key == $start){
                $found = true;
            }

            if($found){ 
                $this->o_steps[$key]->set_enabled(false);
                $this->o_steps[$key]->set_completed(false);
            }
        }
    }


    /* Create navigation menu */
    function get_navigation_html()
    {
        $str = "<ul>";
        $str .= "<li class='menu-header'>"._("Setup")."</li>";
        foreach($this->o_steps as $key => $step){

            $step -> update_strings();

            $s_title    = $step -> get_title();
            $s_info     = $step -> get_small_info();
            $b_active   = $step -> is_active();
            $b_enabled  = $step -> is_enabled();
            $b_completed= $step -> is_completed();

            if($b_completed){
                $s = "<img src='images/true.png' alt='"._("Completed")."' class='center'>&nbsp;"; 
            }else{
                $s = "<img src='images/empty.png' alt=' ' class='center'>&nbsp;";
            }

            if($b_enabled){
                if($b_active){
                    $str .= "<li class='active'>".$s.$s_title."</li>";
                }else{
                    $str .= "<li onClick='document.mainform.setup_goto_step.value=\"$key\";document.mainform.submit();'
                        class='enabled'>".$s.$s_title."</li>";
                }
            }else{
                $str .= "<li class='disabled'>".$s.$s_title."</li>";
            }
        }
        $str .="</ul>";
        $str .="<div class='v-spacer'></div>";
        return($str);
    }



    function get_bottom_html()
    {
        /* Skip adding forward/backward button,   
         *  if the currently opened step is a sub dialog 
         */
        if($this->o_steps[$this->i_current]->dialog){
            $str ="";
        }else{
            $str ="<div class='plugin-actions'>";

            if(isset($this->o_steps[$this->i_current -1]) && $this->o_steps[$this->i_current -1]->is_enabled()){
                $str .= "<button type='submit' name='last'>".msgPool::backButton()."</button>";
            }else{
                $str .= "<button disabled type='submit' name='last'>".msgPool::backButton()."</button>";
            }

            if($this->o_steps[$this->i_current]->b_displayCheckbutton){
                $str .= "&nbsp;<button type='submit' name='test'>"._("Check again")."</button>";
            }

            $str .= "&nbsp;<button type='submit' name='next'>"._("Next")."</button>";
            $str .="</div>";
        }
        return($str);
    }


    /* Create header entry */
    function get_header_html()
    {
        $title = $this->o_steps[$this->i_current]->getTitle();
        $image = $this->o_steps[$this->i_current]->getImage();
        $page = $this->i_current ." / ".count($this->o_steps);


        $str = "<div style='100%'>
                    <div style='float: left; '>
                        <img src='{$image}' alt=''>
                    </div>
                    <div style='float: left; margin-left:10px;margin-top:10px;font-size:24px;font-weight:bold;'>
                        {$title}
                    </div>
                    <div style='float: right; margin-top:10px;font-size:24px;font-weight:bold;'>
                        {$page}
                    </div>
                </div>
                <div class='clear'></div> ";
        return ($str);
    }


    /* Check if the given step id is valid and selectable */
    function selectable_step($id)
    {
        if(isset($this->o_steps[$id]) && $this->o_steps[$id]->is_enabled()){
            return(true);
        }
        return(false);
    }

    function step_name_to_id($name)
    {
        foreach($this->o_steps as $id => $class){
            if(get_class($class) == $name){
                return($id);
            }
        }
        return(0);
    }

}




// vim:tabstop=2:expandtab:shiftwidth=2:filetype=php:syntax:ruler:
?>
