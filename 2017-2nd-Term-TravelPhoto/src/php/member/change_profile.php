<?php
include '../lib/functions.php';
$db = dbconn();
$member = member();
$id = $member['id'];
if (!$id) Error("Available after login.");
if (!$_FILES['photo']) Error("Pictures must be attached.");
$photo = $_FILES['photo'];
extract($_POST);
$typeArr = array("jpg", "jpeg", "png", "gif");

if ($photo_name = strtolower($photo['name'])) {
    $type = explode(".", $photo_name)[1];
    if (array_search($type, $typeArr) === false) Error("It is not image file.");
    if ($photo['size'] >= 2097152) Error("The file size can be up to 2MB.");
    
    if ($type == "jpg" || $type == "jpeg") $source = imagecreatefromjpeg($photo['tmp_name']);
    else if ($type == "png") $source = imagecreatefrompng($photo['tmp_name']);
    else if ($type == "gif") $source = imagecreatefromgif($photo['tmp_name']);
    $file_name = $id.".".$type;
    $thumb_h = 80;
    $thumb_w = 80;
    list($width, $height) = getimagesize($photo['tmp_name']);
    $thumb = imagecreatetruecolor($thumb_w, $thumb_h);
    
    imagecopyresized($thumb, $source, 0, 0, 0, 0, $thumb_w, $thumb_h, $width, $height);
    
    if ($type == "jpg" || $type == "jpeg") imagejpeg($thumb, "../../img/profile/".$file_name, 100);
    else if ($type == "png") $source = imagepng($thumb, "../../img/profile/".$file_name, 100);
    else if ($type == "gif") $source = imagegif($thumb, "../../img/profile/".$file_name, 100);
    
    $sql = "update member set profile='".$db->real_escape_string($file_name)
        ."' where id='".$db->real_escape_string($id)."'";
    $db->query($sql) or trigger_error($db->error."[$sql]");
    $sql = "update comment set profile='".$db->real_escape_string($file_name)
        ."' where writer='".$db->real_escape_string($id)."'";
    $db->query($sql) or trigger_error($db->error."[$sql]");
    echo $file_name;
}
?>