<?php
include "../lib/functions.php";
include "../lib/country.php";
$db = dbconn();
$member = member();
extract($_POST);

if (!$member['id'] || !$pnum || !$writer) Error("Wrong Access.", 0);
if ($member['id'] != $writer) Error("Wrong Access.", 0);

$sql = "select country, imgpath, comnum from post where pnum={$pnum}";
$res = $db->query($sql) or trigger_error($db->error."[$sql]");
$data = $res->fetch_assoc();
if ($data['imgpath']) {
    $path1 = "./img/original/".$data['imgpath'];
    $path2 = "./img/main_thumb/".$data['imgpath'];
    $path3 = "./img/thumb/".$data['imgpath'];
    if (file_exists($path1)) unlink($path1);
    if (file_exists($path2)) unlink($path2);
    if (file_exists($path3)) unlink($path3);
}
if ($data['comnum']) {
    $sql = "delete from comment where pnum={$pnum}";
    $db->query($sql) or trigger_error($db->error."[$sql]");
}

$sql = "delete from post where pnum={$pnum}";
$db->query($sql) or trigger_error($db->error."[$sql]");

$sql = "select pnum from post where writer='"
    .$db->real_escape_string($writer)."' and country='"
    .$db->real_escape_string($data['country'])."'";
$res = $db->query($sql) or trigger_error($db->error."[$sql]");

$sql = "update num_of_photos_by_country set imgcnt=imgcnt-1";
if (!$res->num_rows) $sql .= ", peoplecnt=peoplecnt-1 ";
$sql .= " where country='".$db->real_escape_string($data['country'])."'";
$db->query($sql) or trigger_error($db->error."[$sql]");

$sql = "delete from liker_info where postnum={$pnum}";
$db->query($sql) or trigger_error($db->error."[$sql]");
?>