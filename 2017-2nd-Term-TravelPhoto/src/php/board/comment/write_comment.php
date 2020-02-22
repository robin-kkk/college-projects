<?php
header("content-type:text/html; charset=utf-8");
include "../../lib/functions.php";
extract($_POST);
$db = dbconn();
$member = member();

if (!$pnum) Error("Wrong Access.");
if (!$contents) Error("Contents is empty.");
if (!$member) Error("Wrong Access.");

$regdate = date("Y/m/d h:i:s", time());

$sql = "insert into comment(pnum, writer, contents, regdate, refer) values ({$pnum}, '"
    .$db->real_escape_string($member['id'])."', '"
    .$db->real_escape_string($contents)."', '"
    .$db->real_escape_string($regdate)."', {$refer})";
$db->query($sql) or trigger_error($db->error."[$sql]");

$sql = "update post set comnum=comnum+1 where pnum={$pnum}";
$db->query($sql) or trigger_error($db->error."[$sql]");

$sql = "select cnum, writer, profile, contents, regdate, refer from comment where writer='"
    .$db->real_escape_string($member['id'])
    ."' order by regdate desc limit 1";
$res = $db->query($sql) or trigger_error($db->error."[$sql3]");
$cmt = $res->fetch_assoc();
if ($refer == 0) {
?>
<tr class="reply1" id="cnum<?=$cmt['cnum']?>">
    <td width="15%" align="center">
        <div>
            <img src="img/profile/<?=$cmt['profile']?>">
            <p class="reply-writer"><?=$cmt['writer']?></p>
        </div>
    </td>
    <td width="85%" align="left">
        <?=$cmt['contents']?>
        <p class="reply-option">
            <button class="re-reply" name="<?=$cmt['cnum']?>" value="<?=$cmt['writer']?>">Reply</button>
            <button class="option-btn reply-delete delete<?=$cmt['cnum']?>" data-cnum="<?=$cmt['cnum']?>" data-cwriter="<?=$cmt['writer']?>">Delete</button>
            <span class="date"><?=$cmt['regdate']?></span>
        </p>
    </td>
</tr>
<?php } else { ?>
<tr class="reply2 refer<?=$cmt['refer']?>">
    <td align="center">
        <div>
            <img src="img/profile/basic_profile.svg">
            <p class="reply-writer"><?=$cmt['writer']?></p>
        </div>
    </td>
    <td class="reply_content">
        <?=$cmt['contents']?>
        <p class="reply-option">
            <button class="option-btn reply-delete delete<?=$cmt['cnum']?>" data-cnum="<?=$cmt['cnum']?>" data-cwriter="<?=$cmt['writer']?>">Delete</button>
            <span class="date"><?=$cmt['regdate']?></span>
        </p>
    </td>
</tr>
<?php } ?>