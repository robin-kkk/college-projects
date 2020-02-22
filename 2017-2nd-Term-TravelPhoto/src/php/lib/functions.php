<?php
header("content-type:text/html; charset=utf-8");
function dbconn() {
    $db = new mysqli("localhost", "root", "zaq123", "travel_photos");
    if ($db->connect_errno) die("Database Connect Failed.");
    return $db;
}

function Error($msg, $errno=1) {
    if ($errno) {
        echo $msg;
    } else {
        echo "<script> window.alert(\"{$msg}\"); </script>";
        echo "<script> history.back(1); </script>";
    }
    exit;
}

function member() {
    global $db;
    if (!$_COOKIE) return null;
    $cookie = explode(",", $_COOKIE["COOKIES"]);
    // cookie[0] -> id
    // cookie[1] -> pw
    $query = "select mnum, id, pw, name, nation, email, website, profile from member where id='{$cookie[0]}'";
    $result = $db->query($query) or trigger_error($db->error."[$sql]");
    $member = $result->fetch_assoc();

    return $member;
}

function getLocationInfo($keyword) {
    $keyword = urlencode($keyword);
    $result = array();
    $url = "https://maps.googleapis.com/maps/api/geocode/json?address={$keyword}&language=en&key=AIzaSyCDsgWiX-MEMgqtibBrO-CLiBzEDSgXcBw";
    
    $json = file_get_contents($url);
    
    $obj = json_decode($json, true);
    
    switch(json_last_error()) {
        case JSON_ERROR_NONE: break;
        case JSON_ERROR_DEPTH:
        case JSON_ERROR_STATE_MISMATCH:
        case JSON_ERROR_CTRL_CHAR:
        case JSON_ERROR_SYNTAX:
        case JSON_ERROR_UTF8:
            return null;
    }

    $result['formatted_address'] = $obj['results'][0]['formatted_address'];
    $result['place_id'] = $obj['results'][0]['place_id'];
    $result['lat'] = $obj['results'][0]['geometry']['location']['lat'];
    $result['lng'] = $obj['results'][0]['geometry']['location']['lng'];
    $result['detail'] = array();
    
    foreach($obj['results'][0]['address_components'] as $component) {
        if ($component['types'])
            if ($component['types'][0] != 'postal_code')
                array_push($result['detail'], $component['short_name']);
    }
    return $result;
}

function getLocInfoFromCookie() {
    $info = array();
    if (!$_COOKIE) return null;
    $info['place_id'] = $_COOKIE['pid'];
    $tmp = explode(",", $_COOKIE['loc']);
    $info['lat'] = $tmp[0];
    $info['lng'] = $tmp[1];
    $info['detail'] = array();
    if ($_COOKIE['detail_addr']) {
        $detail = $_COOKIE['detail_addr'];
        if (strpos($detail, ",") !== false) $info['detail'] = explode(",", $_COOKIE['detail_addr']);
        else $info['detail'][0] = $detail;
    }
    return $info;
}
?>