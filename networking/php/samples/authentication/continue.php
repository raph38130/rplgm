<?
session_start($ID);
?>
<HTML>
<? echo $ID; ?>accessible à tous <? echo $count; ?>

<?
if ($ID==$sess_id) echo "on continue";
else echo "forbidden";
?>
</HTML>
