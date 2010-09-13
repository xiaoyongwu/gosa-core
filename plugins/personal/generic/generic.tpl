<table summary="" style="width:100%; vertical-align:top; text-align:left;" cellpadding="4" border="0">

 <!-- Headline container -->
 <tr>
  <td colspan="4">
   <h2><img alt="" align="middle" src="plugins/generic/images/head.png" class="center"> {t}Personal information{/t}</h2>
  </td>
 </tr>

 <!-- Base 1 container -->
 <tr>

  <!-- Image container -->
	<!-- Image container -->
	<td style="vertical-align:top">
	  <table>
	   <tr>
        <td style='width:147px; height:200px; background-color:gray; vertical-align: middle; text-align: center;'>

		{if !$userPicture_is_readable}
			<img class='center' border="0" width="100%" src="plugins/users/images/default.jpg" alt="{t}Personal picture{/t}">
		{else}
            <img  src="getbin.php?rand={$rand}" alt='' style='max-width:147px; max-height: 200px; vertical-align: middle;'
              alt="{t}Personal picture{/t}" >
		{/if}
		</td>
	   </tr>
	   <tr>
		<td>
{render acl=$userPictureACL checkbox=$multiple_support checked=$use_edit_picture}
		 	<input style='vertical-align:top;' type="submit" name="edit_picture" value="{t}Change picture{/t}...">
{/render}
		</td>
	   </tr>
	  </table>
	</td>
	<!--
	Old image container, with incorrect image scaling.
  <td style="vertical-align:top">
   <img align="bottom" border="1" width="145" height="200" src="getbin.php?rand={$rand}" alt="{t}Personal picture{/t}">
   <br>
   <input type="submit" name="edit_picture" value="{t}Change picture{/t}...">
  </td>
	-->

  <!-- Name, ... -->
  <td style="vertical-align:top;">
   <table summary=""> 
    {if $is_template ne "true"}
    <tr>
     <td><label for="sn">{t}Last name{/t}{$must}</label></td>
     <td>
{if $multiple_support}
	 <input id="sn" name="dummy2" size=25 maxlength=60 value="{t}Multiple edit{/t}" disabled>
{else}
{render acl=$snACL}
	 <input id="sn" name="sn" size=25 maxlength=60 value="{$sn}">
{/render}
{/if}
	 </td>
    </tr>
    {else}
    <tr>
     <td><label for="sn">{t}Template name{/t}{$must}</label></td>
     <td>{render acl=$snACL}<input id="sn" name="sn" size=25 maxlength=60 value="{$sn}">{/render}</td>
    </tr>
    {/if}

    {if $is_template ne "true"}
    <tr>
     <td><label for="givenName">{t}First name{/t}{$must}</label></td>
     <td>

{if $multiple_support}
	 <input id="givenName" name="dummy3" size=25 maxlength=60 value="{t}Multiple edit{/t}" disabled>
{else}
{render acl=$givenNameACL}
	 <input id="givenName" name="givenName" size=25 maxlength=60 value="{$givenName}">
{/render}
{/if}
	 </td>
    </tr>
    <tr>
     <td><label for="uid">{t}Login{/t}{$must}</label></td>
     <td>
		{if !$multiple_support}
{render acl=$uidACL}
			<input id="uid" name="uid" size=25 maxlength=60  value="{$uid}">
{/render}
		{else}
			<input id="uid" name="dummy1" size=25 maxlength=60  value="{t}Multiple edit{/t}" disabled>
		{/if}
	 </td>
    </tr>
    {/if}
      
    <tr>
     <td>
      <div style="height:10px;"></div>
      <label for="personalTitle">{t}Personal title{/t}</label>
     </td>
     <td>
      <div style="height:10px;"></div>
{render acl=$personalTitleACL checkbox=$multiple_support checked=$use_personalTitle}
		<input id="personalTitle" name="personalTitle" size=25 maxlength=60 value="{$personalTitle}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="academicTitle">{t}Academic title{/t}</label></td>
     <td>
{render acl=$academicTitleACL checkbox=$multiple_support checked=$use_academicTitle}
	      <input id="academicTitle" name="academicTitle" size=25 maxlength=60 value="{$academicTitle}">
{/render}
     </td>
    </tr>
{if !$multiple_support}
    <tr>
     <td>
      <div style="height:10px;"></div>
      <label for="day">{t}Date of birth{/t}</label>
     </td>
     <td>
      <div style="height:10px;"></div>
{render acl=$dateOfBirthACL}
	      {if $use_dob eq 1}
	      <select id="day" name=day onChange="createResult(this.form,this.form.dateOfBirth);"> 
	       {html_options values=$days output=$days selected=$day}
	      </select>
	      <select name=month onChange="populate(this.form,this.form.dateOfBirth);" >
	       {html_options options=$months selected=$month}
	      </select>
	      <select name=year onChange="populate(this.form,this.form.dateOfBirth);" >
	       {html_options values=$years output=$years selected=$year}
	      </select>
	      <input type="hidden" name="dateOfBirth" value="{$dateOfBirth}">
	      <input type="submit" name="set_dob" value="-" >
	      {else}
	      <input type="submit" name="set_dob" value="{t}Set{/t}" >
	      {/if}
{/render}
     </td>
    </tr>
    <tr>
     <td>
      <label for="gender">{t}Sex{/t}</label>
     </td>
     <td>
{render acl=$genderACL}
	      <select size="1" id="gender" name="gender">
	       {html_options options=$gender_list selected=$gender}
	      </select>
{/render}
     </td>
    </tr>
{/if}
    <tr>
     <td>
      <label for="preferredLanguage">{t}Preferred langage{/t}</label>
     </td>
     <td>
{render acl=$preferredLanguageACL checkbox=$multiple_support checked=$use_preferredLanguage}
	      <select size="1" id="preferredLanguage" name="preferredLanguage">
	       {html_options options=$preferredLanguage_list selected=$preferredLanguage}
	      </select>
{/render}
     </td>
    </tr>
    <tr>
     <td>
      <div style="height:10px;"></div>
      <label for="base">{t}Base{/t}</label>
     </td>
     <td>
      <div style="height:10px;"></div>
{render acl=$baseACL checkbox=$multiple_support checked=$use_base}
		<select id="base" size="1" name="base" title="{t}Choose subtree to place user in{/t}">
			{html_options options=$bases selected=$base_select}
		</select>
{/render}
{render acl=$baseACL disable_picture='images/lists/folder_grey.png'}
		<input type="image" name="chooseBase" src="images/lists/folder.png" class="center" title="{t}Select a base{/t}">
{/render}
     </td>
    </tr>
   </table>
  </td>
  <td style="border-left:1px solid #A0A0A0">
   &nbsp;
  </td>
  <td style="vertical-align:top;">

   <!-- Address, ... -->
   <table summary="" style="width:100%"> 
    <tr>
     <td style="vertical-align:top;"><label for="homePostalAddress">{t}Address{/t}</label></td>
      <td>
{render acl=$homePostalAddressACL checkbox=$multiple_support checked=$use_homePostalAddress}
	     <textarea id="homePostalAddress" name="homePostalAddress" rows="3" style="width:100%">{$homePostalAddress}</textarea>
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="homePhone">{t}Private phone{/t}</label>
     </td><td>
{render acl=$homePhoneACL checkbox=$multiple_support checked=$use_homePhone}
	     <input id="homePhone" name="homePhone" size=25 maxlength=60 value="{$homePhone}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="labeledURI">{t}Homepage{/t}</label></td>
     <td>
{render acl=$labeledURIACL checkbox=$multiple_support checked=$use_labeledURI}
	 	<input id="labeledURI" name="labeledURI" size=25 maxlength=60 value="{$labeledURI}">
{/render}
     </td>
    </tr>
    <tr>
     <td colspan=2>
      <div style="height:20px;"></div>
     </td>
    </tr>
    <tr>
     <td><label for="pw_storage">{t}Password storage{/t}</label></td>
     <td>
{render acl=$passwordStorageACL checkbox=$multiple_support checked=$use_pw_storage}
	      <select size="1" id="pw_storage" name="pw_storage" onchange='document.mainform.submit()'>
			{foreach from=$pwmode key=value item=desc}
				<option value="{$value}" {if $pwmode_select == $value}selected{/if} >{$desc}</option>
			{/foreach}
	      </select>
	{if $pw_configurable eq "true"}
	      &nbsp; <input id="edit_pw_method" type="submit" name="edit_pw_method" value="{t}Configure{/t}...">
	{/if}
{/render}
     </td>
    </tr>

    {if $is_template ne "true" && !$multiple_support}
    <tr>
     <td><label for="edit_cert">{t}Certificates{/t}</label></td>
     <td>
{render acl=$CertificatesACL mode=read_active}
      		<input id="edit_cert" type="submit" name="edit_cert" value="{t}Edit certificates{/t}...">
{/render}
     </td>
    </tr>
    {/if}

	{if $gotoLastSystemLogin}
		<tr>
			<td>{t}Last logon{/t}</td>
			<td>{$gotoLastSystemLogin}</td>
		</tr>		
	{/if}
   </table>
  </td>
 </tr>
</table>

<p class="seperator">&nbsp;</p>

<table summary="" class="line" style="width:100%; vertical-align:top; text-align:left;" cellpadding="4" border="0">

 <!-- Headline container -->
 <tr>
  <td colspan="3">
   <h2><img alt="" align="middle" src="plugins/generic/images/house.png" class="center">&nbsp;{t}Organizational information{/t}</h2>
  </td>
 </tr>

 <!-- Base 2 container -->
 <tr>

  {if $governmentmode ne "true"}
  <!-- Company, ... -->
  <td style="vertical-align:top;">
   <table summary=""> 
    <tr>
     <td><label for="o">{t}Organization{/t}</label></td>
     <td>	
{render acl=$oACL checkbox=$multiple_support checked=$use_o}
		<input id="o" name="o" size=22 maxlength=60 value="{$o}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="ou">{t}Department{/t}</label></td>
     <td>
{render acl=$ouACL checkbox=$multiple_support checked=$use_ou}
		<input id="ou" name="ou" size=22 maxlength=60 value="{$ou}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="departmentNumber">{t}Department No.{/t}</label></td>
     <td>
{render acl=$departmentNumberACL checkbox=$multiple_support checked=$use_departmentNumber}
	      <input id="departmentNumber" name="departmentNumber" size=22 maxlength=60 value="{$departmentNumber}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="employeeNumber">{t}Employee No.{/t}</label></td>
     <td>
{render acl=$employeeNumberACL checkbox=$multiple_support checked=$use_employeeNumber}
      		<input id="employeeNumber" name="employeeNumber" size=22 maxlength=60 value="{$employeeNumber}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="employeeType">{t}Employee type{/t}</label></td>
     <td>
{render acl=$employeeTypeACL checkbox=$multiple_support checked=$use_employeeType}
      		<input id="employeeType" name="employeeType" size=22 maxlength=60 value="{$employeeType}">
{/render}
     </td>
    </tr>
   </table>
  </td>
   
  <td style="border-left:1px solid #A0A0A0">
   &nbsp;
  </td>

  <!-- Phone, ... -->
  <td style="vertical-align:top;">
   <table summary=""> 
    <tr>
     <td><label for="roomNumber">{t}Room No.{/t}</label></td>
     <td>
{render acl=$roomNumberACL checkbox=$multiple_support checked=$use_roomNumber}
		<input id="roomNumber" name="roomNumber" size=22 maxlength=60 value="{$roomNumber}">
{/render}
     </td>
    </tr>
    {if $has_phoneaccount ne "true"}
    <tr>
     <td><label for="telephoneNumber">{t}Phone{/t}</label></td>
     <td>
{render acl=$telephoneNumberACL checkbox=$multiple_support checked=$use_telephoneNumber}
      		<input id="telephoneNumber" name="telephoneNumber" size=22 maxlength=60 value="{$telephoneNumber}">
{/render}
     </td>
    </tr>
    {/if}
    <tr>
     <td><label for="mobile">{t}Mobile{/t}</label></td>
     <td>
{render acl=$mobileACL checkbox=$multiple_support checked=$use_mobile}
		<input id="mobile" name="mobile" size=22 maxlength=60 value="{$mobile}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="pager">{t}Pager{/t}</label></td>
     <td>
{render acl=$pagerACL checkbox=$multiple_support checked=$use_pager}
      		<input id="pager" name="pager" size=22 maxlength=60 value="{$pager}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="facsimileTelephoneNumber">{t}Fax{/t}</label></td>
     <td>
{render acl=$facsimileTelephoneNumberACL checkbox=$multiple_support checked=$use_facsimileTelephoneNumber}
		<input id="facsimileTelephoneNumber" name="facsimileTelephoneNumber" size=22 maxlength=60 value="{$facsimileTelephoneNumber}">
{/render}
     </td>
    </tr>
   </table>
  </td>

  <td style="border-left:1px solid #A0A0A0">
   &nbsp;
  </td>

  <!-- Location, ... -->
  <td style="vertical-align:top;">
   <table summary="" style="width:100%"> 
    <tr>
     <td><label for="l">{t}Location{/t}</label></td>
     <td>
{render acl=$lACL checkbox=$multiple_support checked=$use_l}
		<input id="l" name="l" size=22 maxlength=60 value="{$l}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="st">{t}State{/t}</label></td>
     <td>
{render acl=$stACL checkbox=$multiple_support checked=$use_st}
		<input id="st" name="st" size=22 maxlength=60 value="{$st}">
{/render}
     </td>
    </tr>
    <tr>
     <td style="vertical-align:top;"><label for="postalAddress">{t}Address{/t}</label></td>
     <td>
{render acl=$postalAddressACL checkbox=$multiple_support checked=$use_postalAddress}
      		<textarea id="postalAddress" name="postalAddress" rows=3 style="width:100%">{$postalAddress}</textarea>
{/render}
     </td>
    </tr>
   </table>
  </td>

  {else} 

  <!-- Beschreibung, ... -->
  <td style="vertical-align:top;border-right:1px solid #b0b0b0">
   <table summary=""> 
    <tr>
     <td><label for="vocation">{t}Vocation{/t}</label></td>
     <td>
{render acl=$vocationACL checkbox=$multiple_support checked=$use_vocation}
		<input id="vocation" name="vocation" size=22 maxlength=60 value="{$vocation}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="gouvernmentOrganizationalUnitDescription">{t}Unit description{/t}</label></td>
     <td>
{render acl=$gouvernmentOrganizationalUnitDescriptionACL checkbox=$multiple_support checked=$use_gouvernmentOrganizationalUnitDescription}
      		<input id="gouvernmentOrganizationalUnitDescription" name="gouvernmentOrganizationalUnitDescription" size=22 maxlength=60
		value="{$gouvernmentOrganizationalUnitDescription}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="gouvernmentOrganizationalUnitSubjectArea">{t}Subject area{/t}</label></td>
     <td>
{render acl=$gouvernmentOrganizationalUnitSubjectAreaACL checkbox=$multiple_support checked=$use_gouvernmentOrganizationalUnitSubjectArea}
	      	<input id="gouvernmentOrganizationalUnitSubjectArea" name="gouvernmentOrganizationalUnitSubjectArea" size=22 maxlength=60
           	value="{$gouvernmentOrganizationalUnitSubjectArea}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="functionalTitle">{t}Functional title{/t}</label></td>
     <td>
{render acl=$functionalTitleACL checkbox=$multiple_support checked=$use_functionalTitle}
      		<input name="functionalTitle" id="functionalTitle"  size=22 maxlength=60
             	value="{$functionalTitle}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="role">{t}Role{/t}</label></td>
     <td>
{render acl=$roleACL checkbox=$multiple_support checked=$use_role}
      		<input id="role" name="role" size=22 maxlength=60 value="{$role}">
{/render}
     </td>
    </tr>
   </table>
  </td>
  
  <!-- Phone, ... -->
  <td style="vertical-align:top;border-right:1px solid #b0b0b0">
   <table summary=""> 
    <tr>
     <td><label for="gouvernmentOrganizationalPersonLocality">{t}Person locality{/t}</label></td>
     <td>
{render acl=$gouvernmentOrganizationalPersonLocalityACL checkbox=$multiple_support checked=$use_gouvernmentOrganizationalPersonLocality}
      		<input id="gouvernmentOrganizationalPersonLocality" name="gouvernmentOrganizationalPersonLocality" size=22 maxlength=60
                value="{$gouvernmentOrganizationalPersonLocality}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="gouvernmentOrganizationalUnit">{t}Unit{/t}</label></td>
     <td>
{render acl=$gouvernmentOrganizationalUnitACL checkbox=$multiple_support checked=$use_gouvernmentOrganizationalUnit}
		<input id="gouvernmentOrganizationalUnit" name="gouvernmentOrganizationalUnit" size=22 maxlength=60 
            	value="{$gouvernmentOrganizationalUnit}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="street">{t}Street{/t}</label></td>
     <td>
{render acl=$streetACL checkbox=$multiple_support checked=$use_street}
      		<input name="street" id="street" size=22 maxlength=60 value="{$street}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="postalCode">{t}Postal code{/t}</label></td>
     <td> 
{render acl=$postalCodeACL checkbox=$multiple_support checked=$use_postalCode}
      		<input name="postalCode" id="postalCode" size=22 maxlength=60 value="{$postalCode}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="houseIdentifier">{t}House identifier{/t}</label></td>
     <td>
{render acl=$houseIdentifierACL checkbox=$multiple_support checked=$use_houseIdentifier}
      		<input id="houseIdentifier" name="houseIdentifier" size=22 maxlength=60
             	value="{$houseIdentifier}">
{/render}
     </td>
    </tr>
   </table>
  </td>

  <!-- Location, ... -->
  <td style="vertical-align:top;">
   <table summary=""> 
    <tr>
     <td><label for="roomNumber">{t}Room No.{/t}</label></td>
     <td>
{render acl=$roomNumberACL checkbox=$multiple_support checked=$use_roomNumber}
		<input id="roomNumber" name="roomNumber" size=22 maxlength=60 value="{$roomNumber}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="telephoneNumber">{t}Phone{/t}</label></td>
     <td>
{render acl=$telephoneNumberACL checkbox=$multiple_support checked=$use_telephoneNumber}
	      {if $has_phoneaccount ne "true"}
			<input id="telephoneNumber" name="telephoneNumber" size=22 maxlength=60 value="{$telephoneNumber}">
	      {else}
			<i>{t}Please use the phone tab{/t}</i>
	      {/if}
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="facsimileTelephoneNumber">{t}Fax{/t}</label></td>
     <td>
{render acl=$facsimileTelephoneNumberACL checkbox=$multiple_support checked=$use_facsimileTelephoneNumber}
      		<input id="facsimileTelephoneNumber" name="facsimileTelephoneNumber" size=22 maxlength=60 
                value="{$facsimileTelephoneNumber}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="ivbbLastDeliveryCollective">{t}Last delivery{/t}</label></td>
     <td>
{render acl=$ivbbLastDeliveryCollectiveACL checkbox=$multiple_support checked=$use_ivbbLastDeliveryCollective}
      		<input name="ivbbLastDeliveryCollective" size=22 maxlength=60 id="ivbbLastDeliveryCollective" 
             	value="{$ivbbLastDeliveryCollective}">
{/render}
     </td>
    </tr>
    <tr>
     <td><label for="publicVisible">{t}Public visible{/t}</label></td>
     <td>
{render acl=$publicVisibleACL checkbox=$multiple_support checked=$use_publicVisible}
	      <select size="1" id="publicVisible" name="publicVisible">
	       {html_options values=$ivbbmodes output=$ivbbmodes selected=$publicVisible}
	      </select>
{/render}
     </td>
    </tr>
   </table>
  </td>
 {/if} 
 </tr>
</table>

{if $multiple_support}
	<input type="hidden" name="user_mulitple_edit" value="1">
{/if}
<input type=hidden name="generic">

<!-- Place cursor -->
<script language="JavaScript" type="text/javascript">
  <!-- // First input field on page
	focus_field('sn');
  	if (document.mainform.yearSel != "")
				populate(document.mainform,document.mainform.dateOfBirth);
  -->
</script>
