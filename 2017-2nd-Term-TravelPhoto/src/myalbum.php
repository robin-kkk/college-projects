<!DOCTYPE html>
<html lang="en" class="no-js">
<head>
	<meta charset="UTF-8" />
	<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Share your Travel Photos</title>
	<!-- menu styles -->
	<link href="https://fonts.googleapis.com/css?family=Courgette|Indie+Flower|Dosis" rel="stylesheet">
	<link rel="stylesheet" type="text/css" href="css/myalbum/organicfoodicons.css" />
	<link rel="stylesheet" type="text/css" href="css/myalbum/basic.css" />
    <link rel="stylesheet" type="text/css" href="css/myalbum/nav.css" />
    <link rel="stylesheet" type="text/css" href="css/main/gallery.css" />
    <link rel="stylesheet" type="text/css" href="css/myalbum/grid.css" />
    <style>
    .option-btn {
        background-color: #000;
    }

    .option-btn:hover,
    .option-btn:focus {
        color: #fff;
    }
    </style>
    <script src="js/lib/modernizr-custom.js"></script>
    <script src="js/lib/jquery.min.js"></script>   
    <script src="js/lib/TweenMax.min.js"></script>
    <script src="js/lib/imagesloaded.pkgd.min.js"></script>
	<script src="js/lib/masonry.pkgd.min.js"></script>
	<script src="js/lib/cookie.js"></script>
	<script>
    var tmp = getCookie("COOKIES");
    if (tmp == "") {
        window.alert("Available after login.");
        history.back(1);
    }
    </script>
</head>
<body>
    <?php
    include "php/lib/country.php";
    include "php/lib/saying.php";
    include "php/lib/functions.php";
    $db = dbconn();
    $login_user = member();
    if (isset($_GET['id'])) {
        $sql = "select mnum, id, profile from member where id='"
            .$db->real_escape_string($_GET['id'])."'";
        $res = $db->query($sql) or trigger_error($db->error."[$sql]");
        $member = $res->fetch_assoc();
    } else {
        $member = member();
    }
    ?>
	<!-- CONTAINER -->
	<div class="container">
	    <!-- HEADER -->
		<header class="bp-header cf">
            <a href="index.php" id="back"><img src="img/basic/back_before.svg" alt=""></a>
			<div id="profile">
				<div id="profile-photo">
				    <img src="img/profile/<?=$member['profile']?>" alt="<?=$member['mnum']?>">
				</div>
                <?php 
                if ($login_user['id'] == $member['id']) {
                ?>
				<a id="ch-photo" href="#">CHANGE</a>
				<?php } else { ?>
                <span><?=$member['id']?></span>
				<?php } ?> 
			</div>
			<div class="bp-header__main">
				<h2 style="font-family: 'Dosis';"><?=$saying[mt_rand(0, count($saying)-1)]?></h3>
				
			</div>    
		</header>
		<button class="action action--open" aria-label="Open Menu">
		    <span class="icon icon--menu"></span>
        </button>
        <!-- NAVIGATION -->
		<nav id="ml-menu" class="menu">
			<button class="action action--close" aria-label="Close Menu">
			    <span class="icon icon--cross"></span>
            </button>
			<div class="menu__wrap">
                <ul data-menu="main" class="menu__level" tabindex="-1" role="menu" aria-label="All">
                <?php
                $country = getAlphabetically();
                $i = 1;
                foreach($country as $code=>$name) {
                ?>
					<li class="menu__item" role="menuitem">
					    <a class="menu__link" data-submenu="submenu-<?=$i?>" aria-owns="submenu-<?=$i?>" href="#"><?=$code?></a>
				    </li>
				<?php
                    $i++;
                } 
                ?>
                </ul>
				<!-- Submenu -->
				<?php
                $tmp = array_keys($country);
                for ($i = 1; $i < count($tmp); $i++) {
                    $idx = $tmp[$i-1];
                ?>
                <ul data-menu="submenu-<?=$i?>" id="submenu-<?=$i?>" class="menu__level" tabindex="-1" role="menu" aria-label="<?=$idx?>">
                <?php
                    foreach($country[$idx] as $name) {
                ?>
					<li class="menu__item" role="menuitem"><a class="menu__link" href="#"><?=$name?></a></li>
					<?php } ?>
				</ul>
				<?php } ?>
			</div>
		</nav>
		<div id="overlay"></div>
		<div class="change-box">
            <div id="holder"><img src="img/basic/back_after.svg" alt=""></div>
            <form id="change-form" method="post">
            <label>Attach
		        <input id="upload" type="file" name="photo" onchange="javascript:">
            </label>
            <input type="text" readonly="readonly" title="File Route" id="file_route" name="file_route">
		    <div id="btn-box">
		    <button>Yes, it is</button>
		    <button>No, thanks</button>
            </div>
            </form>
		</div>
		<!-- CONTENT -->
		<div class="content">
		</div>
	</div>
	<!-- /view -->
    <script src="js/lib/classie.js"></script>
    <script src="js/myalbum/imgGrid.js"></script>
    <script src="js/myalbum/nav.js"></script>
    <script src="js/myalbum/profile-box.js"></script>
	<script>
	(function() {
		var menuEl = document.getElementById('ml-menu'),
			mlmenu = new MLMenu(menuEl, {
				// breadcrumbsCtrl : true, // show breadcrumbs
				// initialBreadcrumb : 'all', // initial breadcrumb text
				backCtrl : false, // show back button
				// itemsDelayInterval : 60, // delay between each menu item sliding animation
				onItemClick: loadDummyData // callback: item that doesn´t have a submenu gets clicked - onItemClick([event], [inner HTML of the clicked item])
			});

        
		// mobile menu toggle
		var openMenuCtrl = document.querySelector('.action--open'),
			closeMenuCtrl = document.querySelector('.action--close');

		openMenuCtrl.addEventListener('click', openMenu);
		closeMenuCtrl.addEventListener('click', closeMenu);

		function openMenu() {
			classie.add(menuEl, 'menu--open');
			closeMenuCtrl.focus();
		}

		function closeMenu() {
			classie.remove(menuEl, 'menu--open');
			openMenuCtrl.focus();
		}

		// simulate grid content loading
		var gridWrapper = document.querySelector('.content');
        
		function loadDummyData(ev, itemName) {
            if (ev != null) {
                ev.preventDefault();
                closeMenu();
            }
			gridWrapper.innerHTML = '';
			classie.add(gridWrapper, 'content--loading');
            if (document.location.href.search("id") != -1) {
                user_id = document.location.href.split("id=")[1];
            } else {
                var user_id = getCookie("COOKIES");
                user_id = user_id.split("%2C")[0];
            }
			setTimeout(function() {
				classie.remove(gridWrapper, 'content--loading');
                $(".content").hide();
                $.ajax({
                    url: "php/board/load_my_album.php",
                    type: "POST",
                    data: {
                        "user_id": user_id,
                        "country": itemName
                    },
                    success: function(data) {
                        gridWrapper.innerHTML = data;
                        $(".content").show();
                        if ($(".grid__item").length != 0) adjustGridEffect();
                        else $(".input-comment").hide();
                    }
                });
			}, 600);
		}
        loadDummyData(null, 'all');
        
//        $(".menu__breadcrumbs").children("a").click(function() {
//            loadDummyData(null, 'all');
//        });
        function adjustGridEffect() {
            var support = { transitions: Modernizr.csstransitions },
                // transition end event name
                transEndEventNames = { 'WebkitTransition': 'webkitTransitionEnd', 'MozTransition': 'transitionend', 'OTransition': 'oTransitionEnd', 'msTransition': 'MSTransitionEnd', 'transition': 'transitionend' },
                transEndEventName = transEndEventNames[ Modernizr.prefixed( 'transition' ) ],
                onEndTransition = function( el, callback ) {
                    var onEndCallbackFn = function( ev ) {
                        if( support.transitions ) {
                            if( ev.target != this ) return;
                            this.removeEventListener( transEndEventName, onEndCallbackFn );
                        }
                        if( callback && typeof callback === 'function' ) { callback.call(this); }
                    };
                    if( support.transitions ) {
                        el.addEventListener( transEndEventName, onEndCallbackFn );
                    }
                    else {
                        onEndCallbackFn();
                    }
                };

            new GridFx(document.querySelector('.grid'), {
                imgPosition : {
                    x : -0.5,
                    y : 1
                },
                onOpenItem : function(instance, item) {
                    instance.items.forEach(function(el) {
                        if(item != el) {
                            var delay = Math.floor(Math.random() * 50);
                            el.style.WebkitTransition = 'opacity .5s ' + delay + 'ms cubic-bezier(.7,0,.3,1), -webkit-transform .5s ' + delay + 'ms cubic-bezier(.7,0,.3,1)';
                            el.style.transition = 'opacity .5s ' + delay + 'ms cubic-bezier(.7,0,.3,1), transform .5s ' + delay + 'ms cubic-bezier(.7,0,.3,1)';
                            el.style.WebkitTransform = 'scale3d(0.1,0.1,1)';
                            el.style.transform = 'scale3d(0.1,0.1,1)';
                            el.style.opacity = 0;
                        }
                    });
                },
                onCloseItem : function(instance, item) {
                    instance.items.forEach(function(el) {
                        if(item != el) {
                            el.style.WebkitTransition = 'opacity .4s, -webkit-transform .4s';
                            el.style.transition = 'opacity .4s, transform .4s';
                            el.style.WebkitTransform = 'scale3d(1,1,1)';
                            el.style.transform = 'scale3d(1,1,1)';
                            el.style.opacity = 1;

                            onEndTransition(el, function() {
                                el.style.transition = 'none';
                                el.style.WebkitTransform = 'none';
                            });
                        }
                    });
                }
            });
        }
	})();
	</script>
</body>

</html>
