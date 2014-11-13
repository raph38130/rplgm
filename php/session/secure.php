<?
session_start();
?>
<HTML>

<em>cette page est à accès reservé</em><br>
 
<?
if ($_SESSION["accepted"]) {
	echo "OK !!!!";
	
	//add more code here
	
	
	}
else 
	{
	echo "<center><b>acces denied</center></b><br>";
	}

?>	

</HTML>
