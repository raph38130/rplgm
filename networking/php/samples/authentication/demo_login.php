<?php
if (!session_is_registered('count')) {
    session_register('count');
    $count = 1;
}
else {
    $count++;
}
?><?php echo SID;
# the <?php echo SID?> (<?=SID?> can be used if short tag is enabled) 
# is necessary to preserve the session id
# in the case that the user has disabled cookies
?>
<!-- demo_login.php

   RP Thu Sep 18 10:14:53 CEST 2003
	- version initiale	
-->
<HTML>
<HEAD>
	<TITLE>Authentification V.0.0.0
	</TITLE>
</HEAD>
  



<FORM method="get">
    
    <INPUT type="text"   name="nom"><BR>
    <INPUT type="password" name="passwd" > <BR>
    <INPUT type="radio"  name="cmd" value="ON"> <BR>
    <INPUT type="radio"  name="cmd" value="OFF"> <BR>
    <INPUT type="submit" value="Send"> <INPUT type="reset">
   
</FORM>

<!-- dynamicaly generated HTML page start below-->
<?php 
	print "nom=".$nom." passwd=".$passwd." cmd=".$cmd;
?>


Hello visitor, you have seen this page <?php echo $count; ?> times.<p>;


To continue, <A HREF="nextpage.php?<?php echo SID?>">click here</A>


</HTML>
