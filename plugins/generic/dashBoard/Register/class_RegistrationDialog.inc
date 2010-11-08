<?php



/* Missing functions, validate registration
 *
 */

class RegistrationDialog extends plugin
{
    protected $selectedRegistrationType = "registered";
    protected $isRegistered = FALSE; 
    protected $step = 0;

    public $attrs = array(
            
            // User table
            'uid','password','password_repeated','restoreQuestion','restoreAnswer',
            'mailAddress','surname','givenName','newsletter',

            // Contact table
            'phone','contact','contact_mail','company','street',
            'postal_code','city','trade','employees','description');


    protected $values = array();

    function __construct(&$config)
    {
        $this->config = $config;    

        foreach($this->attrs as $attr) $this->values[$attr] = "";

        // Try to establish a rpc connection.
        $this->rpcConfigured = FALSE;
        if ($this->config->get_cfg_value("core","gosaRpcServer") != ""){
            $this->rpcConfigured = TRUE;
            $this->rpcHandle = $this->config->getRpcHandle(
                    "http://10.3.64.59:4000",
                    "65717fe6-9e3e-11df-b010-5452005f1250",
                    "WyukwauWoid2",
                    TRUE);
        }
    }


    function registerNow()
    {
        if($this->selectedRegistrationType == "registered"){

            // Register user 
            print_a($this->rpcHandle->registerUser('cajus', $this->values));

            // Register Instance with User.
        }

        #$this->step = 200; // Awaiting verification
    }


    function check()
    {
        $message  = array();
        if($this->step == 1){

            // Checking for a valid 'uid'.
            if(empty($this->values['uid'])) {
                $message[] = msgPool::required(_("Account-ID"));
            }elseif(!preg_match("/^[a-z0-9_-]+$/", $this->values['uid'])){
                $message[] = msgPool::invalid(_("Account-ID"), $this->values['uid'],'/[a-z0-9_-]/');
            }
        }
        
        return($message);
    }

   
    function execute()
    {
        // Registration request.
        if(isset($_POST['registerNow'])){
            $msgs = $this->check();
            if(count($msgs)){
                msg_dialog::displayChecks($msgs); 
            }else{
                $this->registerNow();
            }
        }


        $smarty = get_smarty();
        $smarty->assign("default", $this->selectedRegistrationType);
        $smarty->assign("step", $this->step);

        foreach($this->attrs as $attr) $smarty->assign($attr, set_post($this->values[$attr]));


        return($smarty->fetch(get_template_path("Register/register.tpl", TRUE))); 
    } 


    function save_object()
    {
        if(isset($_POST['registrationType']))  $this->selectedRegistrationType = get_post('registrationType');
        if(isset($_POST['startRegistration'])) $this->step = 1;
        if(isset($_POST['stepBack'])) $this->step -= 1;

        foreach($this->attrs as $attr){
            if(isset($_POST[$attr])) $this->values[$attr] = get_post($attr);
        }
    }
}
?>