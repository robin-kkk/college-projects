<?php
header("content-type:text/html; charset=UTF-8");
include '../lib/functions.php';
$db = dbconn();
extract($_POST);

$query = "select id, pw from member where id='{$id}'";
$result = $db->query($query);
$member = $result->fetch_array(MYSQLI_ASSOC);

if (!$id) Error("You should input your ID", 0);
else if ($member['id'] != $id) Error("It does not exist.", 0);
else {
    if (!$pw) Error("You should input your password", 0);
    else if ($member['pw'] != md5($pw)) Error("Your password is incorrect.", 0);
}

if (($id == $member['id']) && ($member['pw'] == md5($pw))) {
    $tmp = $member['id'].",".$member['pw'];
    setcookie("COOKIES", $tmp, time()+60*60*24, "/");
}

$result->free();
$db->close();
?>

<script>
    location.href="../../index.php";
</script>
