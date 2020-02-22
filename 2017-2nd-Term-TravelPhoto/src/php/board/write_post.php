<?php
include "../lib/functions.php";
include "../lib/country.php";
$db = dbconn();
$member = member();
extract($_POST);
if (!$_FILES['photo']) Error("Pictures must be attached.");
$id = $member['id'];
if (!$id) Error("Available after login.");
if (!$nation) Error("Please select a country.");
$photo = $_FILES['photo'];
$typeArr = array("jpg", "jpeg", "png", "gif");
$dirArr = array("./img/original/", "./img/main_thumb/", "./img/thumb/");
$regdate = date("Ymdhis", time());

if (!$userplace) $userplace = $coun[$nation];
$addr = getLocationInfo($userplace);
$seekplace = $addr['formatted_address'];
setcookie("pid", $addr['place_id'], time()+60*60*24, "/");
setcookie("loc", $addr['lat'].",".$addr['lng'], time()+60*60*24, "/");
$detail = implode(",", $addr['detail']);
setcookie("detail_addr", $detail, time()+60*60*24, "/");

if ($photo_name = strtolower($photo['name'])) {
    // 확장자 비교
    $type = explode(".", $photo_name)[1];
    if (array_search($type, $typeArr) === false)
        Error("It is not image file.");
    
    // 1MB = 1048576 Byte
    if ($photo['size'] >= 2097152)
        Error("The file size can be up to 2MB.");
    
    list($width, $height) = getimagesize($photo['tmp_name']);
    if ($width < 700) Error("The picture width is at least 700px.");
    $file_name = $nation."_".$id."_".$regdate.chr(rand(97, 122)).chr(rand(97, 122)).rand(1, 9).rand(1, 9).".".$type;

    // 이미지 저장
    move_uploaded_file($photo['tmp_name'], $dirArr[0].$file_name);
    //chmod($dirArr[0].$file_name, 0777);
    
    if ($type == "jpg" || $type == "jpeg") $source = imagecreatefromjpeg($dirArr[0].$file_name);
    else if ($type == "png") $source = imagecreatefrompng($dirArr[0].$file_name);
    else if ($type == "gif") $source = imagecreatefromgif($dirArr[0].$file_name);
    
    // 게시판 썸네일용 
    $thumb_h = ceil(($height*200)/$width);
    $thumb_w = 200;
    
    $thumb = imagecreatetruecolor($thumb_w, $thumb_h);
    
    // 이미지 크기를 줄이기.
    imagecopyresized($thumb, $source, 0, 0, 0, 0, $thumb_w, $thumb_h, $width, $height);
    
    if ($type == "jpg" || $type == "jpeg") imagejpeg($thumb, $dirArr[2].$file_name, 100);
    else if ($type == "png") $source = imagepng($thumb, $dirArr[2].$file_name, 100);
    else if ($type == "gif") $source = imagegif($thumb, $dirArr[2].$file_name, 100);
}

$result = $db->query("select country from num_of_photos_by_country where country='{$nation}'");
if ($result->num_rows) { // 등록된 국가일 경우
    $query = "select pnum from post where imgpath like '{$nation}_{$id}%'";
    $result = $db->query($query);
    
    if ($result->num_rows) // 작성자가 그 국가가 처음이 아닐 경우
        $query = "update num_of_photos_by_country set imgcnt=imgcnt+1 where country='{$nation}'";
    else // 작성자가 그 국가가 처음일 경우
        $query = "update num_of_photos_by_country set imgcnt=imgcnt+1, peoplecnt=peoplecnt+1 where country='{$nation}'";
    
    $result->free();
} else { // 등록되지 않은 국가일 경우
    $query = "insert into num_of_photos_by_country values ('"
    .$db->real_escape_string($nation)."', 1, 1)";
}
$db->query($query) or trigger_error($db->error."[$query]");

$query = "insert into post(writer, regdate, feeling, country, userplace, seekplace, imgpath) values('"
    .$db->real_escape_string($id)."', '"
    .$db->real_escape_string(date("Y/m/d h:i:s", time()))."', '"
    .$db->real_escape_string($feeling)."', '"
    .$db->real_escape_string($nation)."', '"
    .$db->real_escape_string($userplace)."', '"
    .$db->real_escape_string($seekplace)."', '"
    .$db->real_escape_string($file_name)."')";
$db->query($query) or trigger_error($db->error."[$query]");

$db->close();
echo $file_name;
?>
