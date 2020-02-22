<?php
include "../lib/functions.php";
include "../lib/country.php";
$db = dbconn();
$member = member();
extract($_POST);

if (!$member['id']) Error("Available after login.", 0);
if (!$country) Error("Wrong Access.", 0);

$sql = "select pnum, writer, regdate, feeling, userplace, seekplace, imgpath, liker, comnum from post ";
if ($country == 'all') $sql .= "where writer='{$user_id}' order by regdate desc";
else {
    $country = array_search($country, $coun);
    $sql .= "where country='{$country}' and writer='{$user_id}' order by regdate desc";
}
$result = $db->query($sql) or trigger_error($db->error."[$sql]");
?>
<?php
if ($result->num_rows) { ?>
<div class="grid">
<?php
    while ($post = $result->fetch_assoc()) {
        list($w, $h) = @getimagesize("img/original/".$post['imgpath']);
        $tmp = $w."x".$h;
?>
    <div class="grid__item" data-size="<?=$tmp?>" id="post<?=$post['pnum']?>">
        <a href="php/board/img/original/<?=$post['imgpath']?>" class="img-wrap">
            <img src="php/board/img/thumb/<?=$post['imgpath']?>" alt="<?=$post['pnum']?>" id="img<?=$post['pnum']?>"/>
            <div class="description--grid desc">
                    <div class="wrapper" data-pnum="<?=$post['pnum']?>">
                    <table width="100%" class="desc-table">
                       <tr>
                           <td colspan="2"><h1 style="margin-bottom: 0;"><?=$post['seekplace']?></h1></td>
                       </tr>
                       <tr>
                           <td align="left">
                           <h3>Photo by <span style="color:darksalmon;"><?=$post['writer']?></span>
                               <?php 
                                    if ($member['id'] != $post['writer']) {
                                        $query = "select postnum from `liker_info` where postnum={$post['pnum']} and likernum={$member['mnum']}";
                                        $res = $db->query($query) or trigger_error($db->error."[$query]");
                                        if ($res->num_rows) {
                                ?>
                                        <button class="post-option like_after like" value="<?=$post['pnum']?>"><img src="img/basic/like_after.svg" ></button>
                                <?php } else { ?>
                                        <button class="post-option like_before like" value="<?=$post['pnum']?>"><img src="img/basic/like2.svg" ></button>
                                <?php }  ?>
                                   <label for="like" style="font-size:0.8em;" value="<?=$post['liker']?>"><?=$post['liker']?></label>
                               <?php } else { ?>
                                   <span style="font-size:0.8em; color:red;" value="<?=$post['liker']?>">&nbsp;&nbsp;(♥=<?=$post['liker']?>)</span>
                                <?php } ?>
                           </h3>
                           </td>
                           <td align="center"></td>
                       </tr>
                       <tr>
                           <td><h3><?=$post['feeling']?></h3></td>
                           <td align="center" valign="bottom">
                                <?php if ($member['id'] == $post['writer']) { ?>
                                <button class="post-option post-delete" data-pnum="<?=$post['pnum']?>" data-writer="<?=$post['writer']?>">
                                   <img src="img/basic/trash_can2.svg" >
                                </button>
                                <?php } ?>
                            </td>
                       </tr>
                   </table>
                    <p class="date"><?=$post['regdate']?>&nbsp;&nbsp;</p>
                    <hr style="color:#fff;">
                    <p style="text-align: right; margin: 0; padding: 0; font-size: 0.8em;">TOTAL COMMENT&nbsp;[<span class="cmtcount"><?=$post['comnum']?></span>]&nbsp;&nbsp;</p>
                    <div class="comment-wrapper">
                        <div class="comment">
                            <table>
                                <?php 
                                $sql3 = "select cnum, writer, profile, contents, regdate from comment 
                                where pnum={$post['pnum']} and refer=0 order by regdate";
                                $res = $db->query($sql3) or trigger_error($db->error."[$sql3]");
                                while ($comment = $res->fetch_assoc()) {
                                ?>
                                <tr class="reply1" id="cnum<?=$comment['cnum']?>">
                                    <td width="15%" align="center">
                                        <div>
                                            <img src="img/profile/<?=$comment['profile']?>">
                                            <p class="reply-writer"><?=$comment['writer']?></p>
                                        </div>
                                    </td>
                                    <td width="85%" align="left"><?=$comment['contents']?>
                                        <p class="reply-option">
                                            <button class="option-btn re-reply" name="<?=$comment['cnum']?>" value="<?=$comment['writer']?>">Reply</button>
                                            
                                            <?php if ($comment['writer'] == $member['id']) { ?>
                                            <button class="option-btn reply-delete delete<?=$comment['cnum']?>"
                                            data-cnum="<?=$comment['cnum']?>" data-cwriter="<?=$comment['writer']?>">Delete</button>
                                            <?php } ?>
                                            <span class="date"><?=$comment['regdate']?></span>
                                        </p>
                                    </td>
                                </tr>
                                <?php
                                $sql4 = "select cnum, writer, profile, contents, regdate, refer from comment
                                where pnum={$post['pnum']} and refer={$comment['cnum']} order by regdate";
                                $res2 = $db->query($sql4) or trigger_error($db->error."[$sql]");
                                while ($replys = $res2->fetch_assoc()) {
                                ?>
                                <tr class="reply2 refer<?=$replys['refer']?>">
                                    <td align="center">
                                        <div>
                                            <img src="img/profile/<?=$replys['profile']?>">
                                            <p class="reply-writer"><?=$replys['writer']?></p>
                                        </div>
                                    </td>
                                    <td class="reply_content"><?=$replys['contents']?>
                                        <p class="reply-option">
                                            <?php if ($replys['writer'] == $member['id']) { ?>
                                            <button class="option-btn reply-delete delete<?=$replys['cnum']?>" 
                                            data-cnum="<?=$replys['cnum']?>" data-cwriter="<?=$replys['writer']?>">Delete</button>
                                            <?php } ?>
                                            <span class="date"><?=$replys['regdate']?></span>
                                        </p>
                                    </td>
                                </tr>
                                <?php } } ?>
                            </table>
                        </div>
                    </div>
                </div>
            </div>
        </a>
    </div>
<?php } ?>
</div>
<div class="preview">
    <button class="preview--close">
            <span class="text-hidden">Done</span>
    </button>
    <div class="description--preview"></div>
    <div class="input-comment">
        <button class="comment-toggle">+</button>
        <form method="post">
            <input type="hidden" value="0" name="refer" class="refer">
            <div class="replyer">
            <button class="profile" onclick="return false;"><img src="img/profile/<?=$member['profile']?>"></button>me</div>
            <textarea name="contents" cols="40" rows="3"></textarea>
            <div><input type="submit" value="Submit"></div>
        </form>
    </div>
</div>
<?php 
} else { ?>
<div class="grid">
<?php if ($country == 'all') {  ?>
<p class="info">There's nothing took pictures.</p>
<?php } else { ?>
<p class="info">There's nothing took pictures in <?=$coun[$country]?>.</p>
<?php } ?>
</div>
<div class="preview">
    <button class="preview--close">
            <span class="text-hidden">Done</span>
    </button>
</div>
<?php } ?>

