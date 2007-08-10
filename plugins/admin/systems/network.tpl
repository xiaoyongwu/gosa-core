<h2><img	class="center"	alt=""	align="middle"	src="images/network.png">	{t}Network	settings{/t}</h2>


<table	summary=""	width="100%" cellspacing=0 cellpadding=0>
	<tr>
		<td	style="width:50%; vertical-align: top;">
			<table	summary="">
				<tr>
					<td><LABEL	for="ipHostNumber">{t}IP-address{/t}{if $IPisMust}{$staticAddress}{/if}</LABEL></td>
					<td><input	id="ipHostNumber"	name="ipHostNumber"	size=25	maxlength=80	value="{$ipHostNumber}" {$ipHostNumberACL}></td>
				</tr>
				<tr>
					<td><LABEL	for="macAddress">{t}MAC-address{/t}</LABEL>{$staticAddress}</td>
					<td>
            <input	name="macAddress"	id="macAddress"	size=25	maxlength=80	value="{$macAddress}" {$macAddressACL}>
            {if $autonet==true}
					    <input type="submit" name="autonet" value="{t}Autodetect{/t}" 
                {if ($ipHostNumberACL !="" || $macAddressACL!= "")} disabled {/if}>
            {/if}
          </td>
				</tr>
        {if $dhcpEnabled}
        <tr>
          <td colspan=2 style='padding-top:12px;'>
            <table>
              <tr>
                <td>
                  <input onClick='document.mainform.submit();'
                    {if $dhcp_is_Account} checked {/if} type='checkbox' name='dhcp_is_Account' class='center'>
                </td>
                <td>
                  {t}Enable DHCP for this device{/t}
                  <input type='image' src='images/list_reload.png' class='center'>
                </td>
              </tr>
              {if $dhcp_is_Account}
              <tr>
                <td>&nbsp;</td>
                <td>
                  {t}Parent node{/t}
                  <select name='dhcpParentNode'>      
                    {html_options options=$dhcpParentNodes selected=$dhcpParentNode}
                  </select>
                  &nbsp;
                  <input type='submit' name='dhcpEditOptions' value='{t}Edit settings{/t}'>
                </td>
              </tr>
              {/if}
            </table>
          </td>
        </tr>
        {/if}
      </table> 
    </td>
    {if $DNS_is_account==true}
		<td	style="width:50%;vertical-align:top;border-left:1px solid #b0b0b0;" valign="top">
      <input type="checkbox" name="enableDNS" value="1" {$DNSaccountACL} 
        {if $DNSAccount == true}checked="checked"
        onclick="toggle('test2');
          changeState('zoneName');
          changeState('dNSTTL');
          changeState('AddNewRecord');
          {$changeStateForRecords}
      "/>
        {else}
        onclick="toggle('test2');
          changeState('zoneName');
          changeState('dNSTTL');
          changeState('AddNewRecord');
      "/>
        {/if}
      {t}Enable DNS for this device{/t}
      <input type='image' src='images/list_reload.png' class='center' name="reloadThisDNSStuff" {$DNSaccountACL}/>
      {if $DNSAccount == true}
      <div style="padding-left:20px;visibility:visible;" id="test2">
      {else}
      <div style="padding-left:20px;visibility:hidden;" id="test2">
      {/if}
			<table summary="">
				<tr>
					<td><LABEL	for="zoneName">{t}Zone{/t}</LABEL></td>
					<td>
              <select name="zoneName" id="zoneName" {if $DNSAccount == false} disabled='disabled' {/if}  {$zoneNameACL}>
                {html_options values=$ZoneKeys output=$Zones selected=$zoneName}
              </select>
          </td>
				</tr>
				<tr>
          <td>{t}TTL{/t}</td>
          <td><input type="text" name="dNSTTL" value="{$dNSTTL}" id="dNSTTL" {if $DNSAccount == false} disabled='disabled' {/if} {$dNSTTLACL}>
          </td> 
				</tr>
				<tr>
          <td valign="top">{t}Dns records{/t}</td>
          <td>
            {if $DNSAccount == true}{$records}{/if}
            </td>
				</tr>
			</table>
      </div>
		</td>
    {/if}
    
	</tr>
</table>
<input type="hidden" name="network_tpl_posted" value="1">

<!--
vim:tabstop=2:expandtab:shiftwidth=2:filetype=php:syntax:ruler:
-->
