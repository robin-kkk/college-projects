<?php
include "../lib/functions.php";
extract($_POST);
$db = dbconn();
$member = member();
$likernum = $member['mnum'];
if (isset($pnum) && isset($liked)) {
    if ($liked == "true") {
        $q = "delete from liker_info where postnum={$pnum} and likernum={$likernum}";
        $db->query($q);
        $q = "update post set liker=liker-1 where pnum={$pnum}";
        $db->query($q);
    } else {
        $q = "insert into liker_info values ({$pnum}, {$likernum})";
        $db->query($q);
        $q = "update post set liker=liker+1 where pnum={$pnum}";
        $db->query($q);
    }
}
?>