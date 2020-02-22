<?php
include "../../lib/functions.php";
$db = dbconn();
$member = member();
extract($_POST);
if (!$member['id'] || !$pnum || !$cnum || !$c_writer) Error("Wrong Access.");
if ($member['id'] != $c_writer) Error("Wrong Access. Invalid Identifier.");

$sql = "delete from comment where cnum={$cnum} and pnum={$pnum}";
$db->query($sql) or trigger_error($db->error."[$sql]");

$sql = "select cnum from comment where refer={$cnum}";
$res = $db->query($sql) or trigger_error($db->error."[$sql]");

$count = 1;
if ($res->num_rows) $count += $res->num_rows;

$sql = "update post set comnum=comnum-{$count} where pnum={$pnum}";
$db->query($sql) or trigger_error($db->error."[$sql]");

$sql = "delete from comment where refer={$cnum}";
$db->query($sql) or trigger_error($db->error."[$sql]");
?>