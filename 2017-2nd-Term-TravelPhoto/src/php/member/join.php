<?php
header("charset=UTF-8");
include '../lib/functions.php';
$db = dbconn();
extract($_POST);

if (!$name) Error("You should input your name.");
if ((strlen($name) < 6)) Error("Wrong input : Name.");
if (!$nationality) Error("You should choice your nationality.");
if (!$user_id) Error("You should input your ID");
else {
    if (strlen($user_id) < 6) Error("ID must be at least 6 characters long.");
    else if (!preg_match("/(\w+)/", $user_id)) Error("Wrong input : ID.");
    
    $sql = "select mnum from member where id='".$db->real_escape_string($user_id)."'";
    $res = $db->query($sql) or trigger_error($db->error."[$sql]");
    if ($res->num_rows) Error("Sorry, Already existing ID.");
}

if (!$email) Error("You should input your email.");
else if (!preg_match("/\w+@\w+\.\w+/", $email)) Error("Wrong input : Email.");

if (!$user_pw) Error("You should input your password");
else if (strlen($user_pw) < 6) Error("Password must be at least 6 characters long.");

if (strcmp($user_pw, $check_pw) !== 0) 
    Error("The verification password and the actual password are different.");
    
if ($website) {
    if (!preg_match("
/^(((http(s?))\:\/\/)?)([0-9a-zA-Z\-]+\.)+[a-zA-Z]{2,6}(\:[0-9]+)?(\/\S*)?$/", $website))
        Error("Wrong input : Website.");
}

if (!$pw_hint) Error("You should input the hint in case you will forget your password.");
else if (strlen($pw_hint) > 100) Error("The hint must be the most 100 characters.");

$joindate = date("Y/m/d");
$ip = getenv("REMOTE_ADDR");

$query = "insert into member(id, pw, name, nation, email, website, pwhint, joindate) values ('"
    .$db->real_escape_string($user_id)."', '"
    .$db->real_escape_string(md5($user_pw))."', '"
    .$db->real_escape_string($name)."', '"
    .$db->real_escape_string($nationality)."', '"
    .$db->real_escape_string($email)."', '"
    .$db->real_escape_string($website)."', '"
    .$db->real_escape_string($pw_hint)."', '"
    .$db->real_escape_string($joindate)."')";
$db->query($query) or trigger_error($db->error."[$query]");
$db->close();
echo "We truly congratulate ".$name."'s sign up!!";
?>