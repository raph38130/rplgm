<!-- tango.php3
	Web interface for database TANGO
	(interface PHP à mySQL)

   RP Sun May 20 20:37:05 CEST 2001
	- transformation pour TP/EPS 2001	
	
   RP Tue Apr 24 20:54:07 CEST 2001
	- version initiale	
-->
<HTML>
<HEAD>
	<TITLE>TANGO Database Interface V1.0
	</TITLE>
</HEAD>

<!-- dynamicaly generated HTML page start below-->
<?php 

/* select database parameters */
$HOST="localhost";
$USER="somebody";
$TABLE="device";
$TABLE="device";
//$TABLE="property_attribute_device";
$DBASE="tango";
/*start connection*/
$c=mysql_connect($HOST,$USER);
if (!$c)
	echo "unable to connect";
$db=mysql_select_db($DBASE);
if (!$db )
	echo "unable to select db";

/* print header */
echo "<em>update time is ".date("d/m/y h:i:s")."</em><br><hr>\n"; 

/*this page is an HTML FORM*/
echo "<FORM action=tango.php3 method=POST>\n";

/* table fields description */
$res=mysql_query("DESCRIBE $TABLE;");
$ans=mysql_fetch_row($res);
echo "<font color=green>";
while ($ans=mysql_fetch_row($res)) {
	echo "<td>$ans[0] | ";
	}
echo "</font><br>";

/* dump table */
$res=mysql_query("SELECT * FROM $TABLE;");
echo "<table>";
while ($ans=mysql_fetch_row($res)) {
	//insert a radio button for each line
	echo "<tr><th><td><INPUT type=radio name=field1 value=$ans[0]>";
	echo "<td><font color=red>";
	for($i=0;$i<sizeof($ans);$i++) 
		echo "$ans[$i]</font><td>";
	}
echo "</table>";

?>
<!-- button form definition -->
<br><hr>
<em>Select with checkbox the line you want to edit...(not yet implemented !)</em><br>
<font color=green><INPUT type=submit value="edit ?"></font>
<font color=red><INPUT type=submit value="delete ?"></font>
<INPUT type=reset  value="reset">
</FORM>

<!-- print footer -->
<hr>
<em>warning : this is an automaticaly generated page.</em>
</HTML>
