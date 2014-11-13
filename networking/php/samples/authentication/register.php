<?php session_start(); ?>
<HTML>
<?session_register("count");?>
Hello visitors, you have seen this page <?php echo $count++; ?> times.<p>

<?php 
$sess_id=session_id(); 
session_register($sess_id);
?>
<br>
To continue, <A HREF="continue.php?ID=<?php echo session_id()?>">click here</A>
</HTML>
