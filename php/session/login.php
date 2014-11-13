<?
session_start();
?>

<HTML>

<center> login V0.0 </center>

<br>
<br>
<form>
nom <input type =text name=user value=Cutrone><br>
passwd <input type =password name=pass><br>
<input type =submit name=cmd>
<input type =submit name=cmd value=logout>
</form>
<hr>
<br>


<?php
//comparer nom et prenom venant de la table eleve
$link = mysql_connect('172.16.104.13', 'remy', 'marion')
   or die('Could not connect: ' . mysql_error());
mysql_select_db('Michalet') or die('Could not select database');

// Performing SQL query
$query = 'SELECT prénom FROM eleves WHERE (nom='."'".$_GET['user']."')";
$result = mysql_query($query) or die('Query failed: ' . mysql_error());
$line = mysql_fetch_array($result,MYSQL_ASSOC);
foreach ($line as $col_value) ;
       
if ($col_value != null && $_GET['pass']==$col_value) {
	$_SESSION["accepted"]=true;
	echo "pass=OK<br> you can enter secure <A HREF=secure.php >page </A>";
	}
else
	echo "pass=Not OK !!!<br><br>";
mysql_free_result($result);

// Closing connection
mysql_close($link);

if ($_GET['cmd']=='logout') session_unregister("accepted");
echo "unique session id =". '<b>'.session_id().'</b>';
?>	

</HTML>
