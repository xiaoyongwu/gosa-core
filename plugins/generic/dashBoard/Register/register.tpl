<h3>{t}GOsa registration{/t}</h3>


{if $step == 0}

    {t}Do you want to register GOsa and benefit from the features it brings?{/t}
    <p>
     <input type='radio' name='registrationType' value='' id="registrationType_"
         {if $default == ""} checked {/if}><b><LABEL for="registrationType_">{t}I do not want to register{/t}</LABEL></b>
        <p style='padding-left:20px;'>
            <!-- Add a descritive text later -->
        </p>
        <input type='radio' name='registrationType' value='annonym' id="registrationType_annonym"
            {if $default == "annonym"} checked {/if}><b><LABEL for="registrationType_annonym">{t}Annonym access{/t}</LABEL></b>
        <p style='padding-left:20px;'>
        <ul>
            <li>{t}Access to the 'Open-Channels' to easily install or remove plugins within the GOsa user interface{/t}</li>
            <li>{t}Use the inbuild bugtracker and keep track of their status on the dash-board.{/t}</li>
        </ul>
        </p>

        <p style='padding-left:20px;'>
        {t}What information will be transmitted to the backend and maybe stored:{/t}
        <ul>
            <li>{t}Information about the installed plugins and their version.{/t}</li>
            <li>{t}The GOsa-UUID (will be generated during the registration) and a password, to authenticate.{/t}</li>
            <li>{t}The bugs you will report and the corresponding trace. You can select what information you want to send in.{/t}</li>
        </ul>
        </p>
        
        <input type='radio' name='registrationType' value='registered' id="registrationType_registered"
            {if $default == "registered"} checked {/if}><b><LABEL for="registrationType_registered">{t}Register{/t}</LABEL></b>
        <p style='padding-left:20px;'>
        {t}Additionally to the 'Annonomous' account you can:{/t}
        <ul>
            <li>{t}Access to 'Premium-Channels'.{/t}</li>
            <li>{t}Watch the status of current plugin updates/patches and the availability of new plugins.{/t}</li>
            <li>{t}Receive newsletter, if wanted.{/t}</li>
            <li>{t}View several usefull statistics about your GOsa installation{/t}.</li>
        </ul>
        </p>
           
        <p style='padding-left:20px;'>
        {t}What information will be transmitted to the backend and maybe stored:{/t}
        <ul>
            <li>{t}All personal information filled in the registration form.{/t}</li>
            <li>{t}Information about the installed plugins and their version.{/t}</li>
            <li>{t}The GOsa-UUID (will be generated during the registration) and a password, to authenticate.{/t}</li>
            <li>{t}The bugs you will report and the corresponding trace. You can select what information you want to send in.{/t}</li>
            <li>{t}When the statistics extension is used. GOsa will transmit information about plugins, their usage and the amount of objects present in your ldap database. No sensitive data is transmitted here, just the object type, the action performed, cpu usage, memory usage, elapsed time...{/t}</li>
        </ul>
        </p>
    </p>
    <hr>
    <div class="plugin-actions">
        <button name='startRegistration'>{msgPool type=okButton}</button>
    </div>

{/if}

{if $step == 1}

    <hr>
    <div class="plugin-actions">
        <button name='stepBack'>{msgPool type=backButton}</button>
    </div>
{/if}

{if $step == 2}

    <hr>
    <div class="plugin-actions">
        <button name='stepBack'>{msgPool type=backButton}</button>
    </div>
{/if}