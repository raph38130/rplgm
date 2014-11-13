<?php
if (!session_is_registered('count')) {
    session_register('count');
    $count = 1;
    }
else {
    $count++;
     }
?>

<HTML>
Hello visitor, you have seen this page <?php echo $count; ?> times.<p>

<?php

# the <?php echo SID?> (<?=SID?> can be used if short tag is enabled) 
 # is necessary to preserve the session id
# in the case that the user has disabled cookies

?>

To continue, <A HREF="nextpage.php?<?php echo SID?>">click here</A></HTML>
