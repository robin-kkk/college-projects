<?php
extract($_GET);
if (!$_GET['path']) die("Could not created main thumbnails.");
$path = $_GET['path'];
$type = explode(".", $path)[1];

list($origin_w, $origin_h) = getimagesize("img/original/".$path);

if ($type == "jpg" || $type == "jpeg") $image = imagecreatefromjpeg("img/original/".$path);
else if ($type == "png") $image = imagecreatefrompng("img/original/".$path);
else if ($type == "gif") $image = imagecreatefromgif("img/original/".$path);

$resized_img = imagecreatetruecolor($cur_w, $cur_h);
imagecopyresized($resized_img, $image, 0, 0, 0, 0, $cur_w, $cur_h, $origin_w, $origin_h);

$main_thumb = imagecreatetruecolor(700, 300);
imagecopyresampled($main_thumb, $resized_img, 0, 0, $left, $top, 700, 300, 700, 300);

if ($type == "jpg" || $type == "jpeg") imagejpeg($main_thumb, "img/main_thumb/".$path, 100);
else if ($type == "png") imagepng($main_thumb, "img/main_thumb/".$path, 100);
else if ($type == "gif") imagegif($main_thumb, "img/main_thumb/".$path, 100);
?>

<script>
    document.location.href="../../index.php?wr=t";
</script>
