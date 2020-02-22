<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Share your Travel Photos</title>
    <link rel="shortcut icon" href="favicon.ico">
    <link href="https://fonts.googleapis.com/css?family=Courgette|Indie+Flower" rel="stylesheet">
    <link rel="stylesheet" type="text/css" href="fonts/font-awesome-4.3.0/css/font-awesome.min.css" />
    <link rel="stylesheet" type="text/css" href="css/main/basic.css" />
    <link rel="stylesheet" type="text/css" href="css/main/search.css" />
    <link rel="stylesheet" type="text/css" href="css/main/gallery.css" />
    <link rel="stylesheet" type="text/css" href="css/main/pullupmenu.css" />
    <link rel="stylesheet" type="text/css" href="css/main/modal/modal.css" />
    <link rel="stylesheet" type="text/css" href="css/main/modal/member.css" />
    <link rel="stylesheet" type="text/css" href="css/main/modal/upload.css" />
    <!--[if IE]>
		<script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script>
		<![endif]-->
    <script src="js/lib/classie.js"></script>
    <script src="js/lib/TweenMax.min.js"></script>
    <script src="js/lib/jquery.min.js"></script>
    <script src="js/lib/snap.svg-min.js"></script>
    <script src="js/lib/modernizr-custom.js"></script>
    <script src="js/lib/cookie.js"></script>
    <script>
        var pid;
        var default_lat;
        var default_lng;
        var cur_idx;
        var isRecovered = false;
    </script>
</head>
<body>
    <div id="map"></div>
    <?php
    include "./php/lib/functions.php";
    include "./php/lib/country.php";
    extract($_GET);
    $db = dbconn();
    $member = member();
    $addr = array();
    if (isset($search)) $addr = getLocationInfo($search);
    else if (isset($wr)) { $addr = getLocInfoFromCookie(); }
    if (isset($addr['detail'])) {
        echo "<script>
            pid = '{$addr['place_id']}';
            default_lat = {$addr['lat']};
            default_lng = {$addr['lng']};
            </script>";
        $range = $addr['detail'];
        for ($i = 0; $i < count($range); $i++) {
            if ($i == (count($range)-1)) {
                $sql = "select pnum, writer, regdate, feeling, userplace, seekplace, imgpath, liker, comnum from post where country='{$range[$i]}' order by regdate desc";
                $sql2 = "select pnum, imgpath from post where country='{$range[$i]}' order by regdate desc";
            } else {
                $sql = "select pnum, writer, regdate, feeling, userplace, seekplace, imgpath, liker, comnum from post where seekplace like '%{$range[$i]}%' order by regdate desc";
                $sql2 = "select pnum, imgpath from post where seekplace like '%{$range[$i]}%' order by regdate desc";
            }
            $result = $db->query($sql) or trigger_error($db->error."[$sql]");
            if ($result->num_rows) break;
        }
    } else {
        $sql = "select country from num_of_photos_by_country
                order by imgcnt desc limit 1";
        $result = $db->query($sql) or trigger_error($db->error."[$sql]");
        $temp = $result->fetch_assoc();
        $addr = getLocationInfo($temp['country']);
        echo "<script>
            pid = '{$addr['place_id']}';
            default_lat = {$addr['lat']};
            default_lng = {$addr['lng']};
            </script>";
        $result->free();
        $sql = "select pnum, writer, regdate, feeling, userplace, seekplace, country, imgpath, liker, comnum from post where country='{$temp['country']}' order by regdate desc";
        $sql2 = "select pnum, imgpath from post where country='{$temp['country']}' order by regdate desc";
        $result = $db->query($sql) or trigger_error($db->error."[$sql]");
    }
    ?>
    <!-- MENU -->
    <nav id="menu" class="menu">
        <button class="menu-handle"><span>Menu</span></button>
        <div class="menu-inner">
            <ul>
                <li>
                    <?php if ($member['id']) { ?>
                    <a href="./php/member/logout.php">Logout</a>
                    <?php } else { ?>
                    <a class="login" href="" onclick="return false;">Login</a>
                    <?php } ?>
                </li>
                <li><a href="myalbum.php">My Album</a></li>
                <li><a href="" onclick="return false;">Best Photos</a></li>
                <li><a class="upload" href="" onclick="return false;">Upload</a></li>
            </ul>
        </div>
        <div class="morph-shape" data-morph-open="M-10,100c0,0,44-95,290-95c232,0,290,95,290,95" data-morph-close="M-10,100c0,0,44,95,290,95c232,0,290-95,290-95">
            <svg width="100%" height="100%" viewBox="0 0 560 200" preserveAspectRatio="none">
                <path fill="none" d="M-10,100c0,0,44,0,290,0c232,0,290,0,290,0"/>
            </svg>
        </div>
    </nav>
    <!-- MOTION BLUR -->
    <svg xmlns="http://www.w3.org/2000/svg" version="1.1" class="filters hidden">
        <defs>
          <filter id="blur"> <!-- Tag attribute supported in svg -->
              <feGaussianBlur in="SourceGraphic" stdDeviation="0,0" />
              <!-- "stdDeviation" : attribute setting the directinal blur's intensity -->
          </filter>
        </defs>
    </svg>
    <!-- MAIN -->
    <div class="container">
        <!-- SEARCH SECTION -->
        <div id="sb-search" class="sb-search">
            <form action="index.php" id="search-form" method="get">
                <input class="sb-search-input" placeholder="Enter any place" type="text" value="" name="search" id="search">
                <button class="sb-search-submit" type="submit"><img src="img/basic/search.svg" ></button>
            </form>
        </div>
        <!-- POST SECTION -->
        <?php
        if ($member['id']) {
            if ($result->num_rows) {
        ?>
        <div id="read-btn">
            <button id="read_more">READ MORE</button>
        </div>
        <div class="post-background"></div>
        <?php
                $result->free();
                $result = $db->query($sql) or trigger_error($db->error."[$sql]");
                while ($post = $result->fetch_assoc()) {
                    if (!$post['seekplace']) $post['seekplace'] = $post['country'];
        ?>
        <div class="post-wrapper" id="<?=$post['pnum']?>">
            <button class="post-close">Done</button>
            <div class="img-wrapper">
                <img class="post-img" src="php/board/img/original/<?=$post['imgpath']?>" >
            </div>
            <div class="desc-wrapper set_zero_to_bottom">
                <div class="description">
                       <table width="100%" class="desc-table">
                           <tr>
                               <td colspan="2"><h1 style="margin-bottom: 0;"><?=$post['seekplace']?></h1></td>
                           </tr>
                           <tr>
                               <td align="left">
                               <h3>Photo by <a href="myalbum.php?id=<?=$post['writer']?>" class="writer"><?=$post['writer']?></a>
                               <?php 
                                    if ($member['id'] != $post['writer']) {
                                        $query = "select postnum from `liker_info` where postnum={$post['pnum']} and likernum={$member['mnum']}";
                                        $res = $db->query($query) or trigger_error($db->error."[$query]");
                                        if ($res->num_rows) {
                                ?>
                                        <button class="post-option like_after like" value="<?=$post['pnum']?>"><img src="img/basic/like_after.svg" ></button>
                                <?php } else { ?>
                                        <button class="post-option like_before like" value="<?=$post['pnum']?>"><img src="img/basic/like.svg" ></button>
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
                                   <?php 
                                   if ($member['id'] == $post['writer'] || $member['mnum'] == 1) {
                                   ?>
                                   <button class="post-option post-delete" data-pnum="<?=$post['pnum']?>" data-writer="<?=$post['writer']?>"><img src="img/basic/trash_can.svg" ></button>
                                   <?php } ?>
                                </td>
                           </tr>
                       </table>
                    <p class="date"><?=$post['regdate']?>&nbsp;&nbsp;</p>
                    <hr>
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
                                            <button class="re-reply" name="<?=$comment['cnum']?>" value="<?=$comment['writer']?>">Reply</button>
                                            <?php if ($comment['writer'] == $member['id']) { ?>
                                            <button class="reply-delete" 
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
                                            <button class="reply-delete" data-cnum="<?=$replys['cnum']?>" data-cwriter="<?=$replys['writer']?>">Delete</button>
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
        <?php } } } ?>
        <div class="gallery">
            <ul class="gallery-pictures">
                <?php
                $result->free();
                $result = $db->query($sql2) or trigger_error($db->error."[$sql2]");
                while($data = $result->fetch_assoc()) {
                ?>
                <li class="gallery-picture">
                    <img src="./php/board/img/main_thumb/<?=$data['imgpath']?>" alt="<?=$data['pnum']?>">
                </li>
                <?php } ?>
            </ul>
        </div>
        <!-- MEMBER SECTION -->
        <div class="modal-overlay"></div>
        <div class="modal-wrapper">
            <div class="modal login-modal">
                <!-- LOGIN -->
                <form action="./php/member/login.php" method="post">
                    <button class="close-modal" id="close-login" type="reset">×</button>
                    <section>
                        <span class="input login-input">
                            <input class="input-field login-input-field" type="text" id="id" name="id" />
                            <label class="input-label login-input-label" for="id">
                                <span class="input-label-content login-input-label-content">ID</span>
                            </label>
                        </span>
                        <br>
                        <span class="input login-input">
                            <input class="input-field login-input-field" type="password" id="pw" name="pw"/>
                            <label class="input-label login-input-label" for="pw">
                                <span class="input-label-content login-input-label-content">PASSWORD</span>
                            </label>
                        </span>
                    </section>
                    <div class="login-div">
                        <button class="button" type="submit">LOGIN</button>
                        <a href="" onclick="return false;">ID Search</a>
                        <a href="" onclick="return false;">PW Search</a>
                        <span class="join-btn">Sign Up</span>
                    </div>
                </form>
                <!-- JOIN -->
                <div class="member">
                    <form method="post">
                        <button class="close-modal" id="close-join" type="reset">×</button>
                        <span class="input join-input">
                        <input class="input-field join-input-field" type="text" name="name" id="name" maxlength="12"/>
                        <label class="input-label join-input-label" for="name">
                            <span class="input-label-content join-input-label-content" data-content="Name">Name</span>
                        </label>
                        <svg class="join-graphic" width="300%" height="100%" viewBox="0 0 1200 60" preserveAspectRatio="none">
                        <path d="M1200,9c0,0-305.005,0-401.001,0C733,9,675.327,4.969,598,4.969C514.994,4.969,449.336,9,400.333,9C299.666,9,0,9,0,9v43c0,0,299.666,0,400.333,0c49.002,0,114.66,3.484,197.667,3.484c77.327,0,135-3.484,200.999-3.484C894.995,52,1200,52,1200,52V9z"/>
                        </svg>
                        </span>

                        <div class="nationality">
                            <label for="nationality">Where are you from?</label>
                            <select name="nationality" id="nationality">
                            <option value="0" selected>Any country</option>
                            <?php
                            foreach($coun as $code=>$name) {
                            ?>
                            <option value="<?=$code?>"><?=$name?></option>
                            <?php } ?>
                        </select>
                        </div>

                        <span class="input join-input">
                        <input class="input-field join-input-field" type="text" id="user_id" name="user_id" maxlength="14"/>
                        <label class="input-label join-input-label" for="user_id">
                            <span class="input-label-content join-input-label-content" data-content="ID">ID</span>
                        </label>
                        <svg class="join-graphic" width="300%" height="100%" viewBox="0 0 1200 60" preserveAspectRatio="none">
                            <path d="M1200,9c0,0-305.005,0-401.001,0C733,9,675.327,4.969,598,4.969C514.994,4.969,449.336,9,400.333,9C299.666,9,0,9,0,9v43c0,0,299.666,0,400.333,0c49.002,0,114.66,3.484,197.667,3.484c77.327,0,135-3.484,200.999-3.484C894.995,52,1200,52,1200,52V9z"/>
                        </svg>
                        </span>

                        <span class="input join-input">
                        <input class="input-field join-input-field" type="email" id="email" name="email"/>
                        <label class="input-label join-input-label" for="email">
                            <span class="input-label-content join-input-label-content" data-content="Email">Email</span>
                        </label>
                        <svg class="join-graphic" width="300%" height="100%" viewBox="0 0 1200 60" preserveAspectRatio="none">
                            <path d="M1200,9c0,0-305.005,0-401.001,0C733,9,675.327,4.969,598,4.969C514.994,4.969,449.336,9,400.333,9C299.666,9,0,9,0,9v43c0,0,299.666,0,400.333,0c49.002,0,114.66,3.484,197.667,3.484c77.327,0,135-3.484,200.999-3.484C894.995,52,1200,52,1200,52V9z"/>
                        </svg>
                        </span>

                        <span class="input join-input">
                        <input class="input-field join-input-field" type="password" id="user_pw" name="user_pw" maxlength="18"/>
                        <label class="input-label join-input-label" for="user_pw">
                            <span class="input-label-content join-input-label-content" data-content="Password">Password</span>
                        </label>
                        <svg class="join-graphic" width="300%" height="100%" viewBox="0 0 1200 60" preserveAspectRatio="none">
                            <path d="M1200,9c0,0-305.005,0-401.001,0C733,9,675.327,4.969,598,4.969C514.994,4.969,449.336,9,400.333,9C299.666,9,0,9,0,9v43c0,0,299.666,0,400.333,0c49.002,0,114.66,3.484,197.667,3.484c77.327,0,135-3.484,200.999-3.484C894.995,52,1200,52,1200,52V9z"/>
                        </svg>
                        </span>

                        <span class="input join-input">
                        <input class="input-field join-input-field" type="password" id="check_pw" name="check_pw" maxlength="18"/>
                        <label class="input-label join-input-label" for="check_pw">
                            <span class="input-label-content join-input-label-content" data-content="Check your password">Check your password</span>
                        </label>
                        <svg class="join-graphic" width="300%" height="100%" viewBox="0 0 1200 60" preserveAspectRatio="none">
                            <path d="M1200,9c0,0-305.005,0-401.001,0C733,9,675.327,4.969,598,4.969C514.994,4.969,449.336,9,400.333,9C299.666,9,0,9,0,9v43c0,0,299.666,0,400.333,0c49.002,0,114.66,3.484,197.667,3.484c77.327,0,135-3.484,200.999-3.484C894.995,52,1200,52,1200,52V9z"/>
                        </svg>
                        </span>

                        <span class="input join-input" style="max-width: 350px;">
                        <input class="input-field join-input-field" type="text" id="website" name="website"/>
                        <label class="input-label join-input-label" for="website">
                            <span class="input-label-content join-input-label-content" data-content="Website">Website</span>
                        </label>
                        <svg class="join-graphic" width="300%" height="100%" viewBox="0 0 1200 60" preserveAspectRatio="none">
                            <path d="M1200,9c0,0-305.005,0-401.001,0C733,9,675.327,4.969,598,4.969C514.994,4.969,449.336,9,400.333,9C299.666,9,0,9,0,9v43c0,0,299.666,0,400.333,0c49.002,0,114.66,3.484,197.667,3.484c77.327,0,135-3.484,200.999-3.484C894.995,52,1200,52,1200,52V9z"/>
                        </svg>
                        </span>

                        <span class="input join-input" style="max-width: 350px;">
                        <input class="input-field join-input-field" type="text" id="pw_hint" name="pw_hint"/>
                        <label class="input-label join-input-label" for="pw_hint">
                            <span class="input-label-content join-input-label-content" data-content="" >Password Hint</span>
                        </label>
                        <svg class="join-graphic" width="300%" height="100%" viewBox="0 0 1200 60" preserveAspectRatio="none">
                            <path d="M1200,9c0,0-305.005,0-401.001,0C733,9,675.327,4.969,598,4.969C514.994,4.969,449.336,9,400.333,9C299.666,9,0,9,0,9v43c0,0,299.666,0,400.333,0c49.002,0,114.66,3.484,197.667,3.484c77.327,0,135-3.484,200.999-3.484C894.995,52,1200,52,1200,52V9z"/>
                        </svg>
                        <span><font size="2em" color="darked">&nbsp;&nbsp;If you forget PW, you can find with this</font></span>
                        </span>
                        <div class="signup-div">
                            <button class="button" id="signup">SIGN UP</button>
                        </div>
                    </form>
                </div>
            </div>
        </div>
        <!-- /MEMBER -->
        <!-- UPLOAD -->
        <div class="modal-wrapper">
            <div class="modal upload-modal">
                <button class="close-modal" id="close-upload" type="reset">×</button>
                <form id="nl-form" class="nl-form" method="post" enctype="multipart/form-data">
                    Welcome,&nbsp;&nbsp;<span class="writer"><?=$member['id']?></span>&nbsp;&nbsp;!
                    <hr> Please Share Your Travel Photo. <br> Many people wonder your experience!! <br>
                    <div class="file-div">
                        <input type="file" name="photo" id="file" class="inputfile" data-multiple-caption="{count} files selected" multiple />
                        <label for="file"><span></span> <strong><svg xmlns="http://www.w3.org/2000/svg" width="20" height="17" viewBox="0 0 20 17"><path d="M10 0l-5.2 4.9h3.3v5.1h3.8v-5.1h3.3l-5.2-4.9zm9.3 11.5l-3.2-2.1h-2l3.4 2.6h-3.5c-.1 0-.2.1-.2.1l-.8 2.3h-6l-.8-2.2c-.1-.1-.1-.2-.2-.2h-3.6l3.4-2.6h-2l-3.2 2.1c-.4.3-.7 1-.6 1.5l.6 3.1c.1.5.7.9 1.2.9h16.3c.6 0 1.1-.4 1.3-.9l.6-3.1c.1-.5-.2-1.2-.7-1.5z"/></svg> Choose a file&hellip;</strong></label>
                    </div>
                    How was your feeling at that time?&nbsp;&nbsp;
                    <select name="feeling">
                        <option value="Not feeling" selected>Not feeling</option>
                        <option value="Power Up!!">Power Up!!</option>
                        <option value="Unforgettable">Unforgettable</option>
                        <option value="In heaven..?">In heaven..?</option>
                        <option value="Spirituous">Spirituous</option>
                        <option value="Stranger">Stranger</option>
                        <option value="Pretty Good~">Pretty Good~</option>
                        <option value="be touched!">be touched!</option>
                        <option value="Tired -_-..">Tired -_-..</option>
                        <option value="Excited!!!">Excited!!!</option>
                    </select>
                    <br> Where did you take a picture?&nbsp;&nbsp;
                    <select name="nation" id="nation">
                        <option value="0" selected>Any country</option>
                        <?php
                        foreach($coun as $code=>$name) {
                        ?>
                        <option value="<?=$code?>"><?=$name?></option>
                        <?php } ?>
                    </select><br>
                    <input type="text" name="userplace" id="detail-addr" placeholder="Explain detail address here" /><hr>
                    <div class="nl-submit-wrap">
                        <button class="button" id="nl-submit" type="submit">Complete</button>
                    </div>
                    <div class="nl-overlay"></div>
                </form>
            </div>
        </div>
    </div>
    <!-- /container -->
    <script src="js/main/search.js"></script>
    <script src="js/lib/google_map.js"></script>
    <script src="js/main/gallery.js"></script>
    <script src="js/main/post.js"></script>
    <script src="js/main/menu.js"></script>
    <script src="js/main/motionblur.js"></script>
    <script src="js/main/modal.js"></script>
    <script src="js/main/upload.js"></script>
    <script>
        var nlform = new NLForm(document.getElementById('nl-form'));
        new UISearch(document.getElementById('sb-search'));
    </script>
    <!-- 지정된 URL 에서 API를 로드 -->
    <script type="text/javascript" src="https://maps.googleapis.com/maps/api/js?key=AIzaSyCDsgWiX-MEMgqtibBrO-CLiBzEDSgXcBw&libraries=places&callback=initMap" async defer></script>
</body>

</html>
