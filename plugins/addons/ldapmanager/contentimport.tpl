{if $type == FALSE || $LDIFError != FALSE}
<p style="margin-top:5px;">
  {t}The LDIF import plugin provides methods to upload a set of entries to your running LDAP directory as ldif. You may use this to add new or modify existing entries. Remember that GOsa will not check your ldifs for GOsa conformance.{/t}
</p>

<p class="seperator">&nbsp;</p>
<table summary="">
<tr>
    <td width="30%">
		<LABEL for="userfile">{t}Import LDIF File{/t}</LABEL>
    </td>
    <td>
			<input type="hidden" name="ignore">
			<input type="hidden" name="MAX_FILE_SIZE" value="2097152">
			<input name="userfile" id="userfile" type="file" value="{t}Browse{/t}">
    </td>
</tr>
<tr>
	<td>
		&nbsp;
	</td>
	<td>
        <input type="checkbox" name="overwrite" value="1" id="overwrite">
		<LABEL for="overwrite">{t}Modify existing attributes{/t}</LABEL>
	</td>
</tr>
<tr>
   	<td>
		&nbsp;
   	</td>
   	<td>
        <input type="checkbox" name="cleanup" value="1" id="cleanup">
		<LABEL for="cleanup">{t}Overwrite existing entry{/t}</LABEL>
	</td>
</tr>
</table>
{else}

<br>
    <h1>{t}Import successful{/t}</h1>
<br>

<div align="right">
		<input type="submit" value="{t}Back{/t}" name='back'>
</div>
		
{/if}
<p class="plugbottom">
<input type="submit" value="{t}Import{/t}" name="fileup" >
</p>
<input type="hidden" name="ignore">