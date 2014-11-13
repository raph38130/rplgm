<HTML>

<center> les eleves de la classe V0.0 </center>

<br>
<br>
<form>

nom du champ à visualiser <input type =text name=field value=*><br>
nom de la table à visualiser <input type =text name=table value=eleves><br>
<input type =submit>
</form>
<hr>
<br>

<?php
// Connecting, selecting database

$link = mysql_connect('172.16.104.13', 'remy', 'marion')
   or die('Could not connect: ' . mysql_error());
echo 'nom du champ <strong>'.$_GET['field'].'</strong>';
mysql_select_db('Michalet') or die('Could not select database');

// Performing SQL query
$query = 'SELECT '.$_GET['field'].' FROM '.$_GET['table'];
$result = mysql_query($query) or die('Query failed: ' . mysql_error());

// Printing results in HTML
echo "<table>\n";
while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
   echo "\t<tr>\n";
   foreach ($line as $col_value) {
       echo "\t\t<td>$col_value</td>\n";
   }
   echo "\t</tr>\n";
}
echo "</table>\n";

// Free resultset
mysql_free_result($result);

// Closing connection
mysql_close($link);

?>	

</HTML>
