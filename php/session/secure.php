<?
session_start();
?>
<HTML>

<em>cette page est � acc�s reserv�</em><br>
 
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
