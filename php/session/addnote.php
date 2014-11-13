<HTML>

<center> ajouter note V0.0 </center>

<br>
<br>
<form>
id <input type =text name=user><br>
math <input type =text name=math><br>
francais <input type =text name=fr>
<input type =submit>
</form>
<hr>
<br>

<?php

$link = mysql_connect('172.16.104.13', 'remy', 'marion')
   or die('Could not connect: ' . mysql_error());
mysql_select_db('Michalet') or die('Could not select database');

// Performing SQL query
$query = 'INSERT INTO note VALUES ('.$_GET['user'].','.$_GET['math'].','.$_GET['fr'].',0,Now())';
echo '<strong>'.$query.'</strong>';

$result = mysql_query($query) or die('Query failed: ' . mysql_error());

// Free resultset
mysql_free_result($result);

// Closing connection
mysql_close($link);
?>	

</HTML>
