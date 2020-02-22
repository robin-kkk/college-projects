<!DOCTYPE HTML>
<html>
<head>
    <meta charset='utf-8'>
    <title>Share your Travel Photos</title>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <!--[if lt IE 9]>
    <script src='//cdnjs.cloudflare.com/ajax/libs/respond.js/1.4.2/respond.js'></script>
  <![endif]-->
    <link rel='stylesheet' href='../../css/main/crop.css' media='all'>
    <link rel='stylesheet' href='../../css/main/jquery.guillotine.css' media='all'>
    <script type="text/javascript" src="../../js/lib/jquery.min.js"></script>
    <script type="text/javascript" src="../../js/lib/jquery-ui.js"></script>
    <script type="text/javascript">
        $(function() {
            $("#crop_space").draggable({
                containment: "parent"
            });
        });

        function img_info(img) {
            if (img.width >= $(window).width() * 0.7) {
                $(".frame").css("width", $(window).width() * 0.7);
                if (img.height < $(window).height())
                    $(".frame").css("height", img.height());
                else
                    $(".frame").css("height", $(window).height() * 0.7);
            } else {
                $(".frame").css("width", img.width);
                if (img.height < $(window).height() * 0.7)
                    $(".frame").css("height", img.height());
                else
                    $(".frame").css("height", $(window).height() * 0.7);
            }
            $("#data").css("left", ($(".frame").width() - $("#data").width()) / 2);

            $("#controls").children("form").children(".btn").hover(function() {
                var posi = $("#crop_space");
                var origin = $("#sample_picture");
                var start_y = posi.offset().top - origin.offset().top;
                var start_x = posi.offset().left - origin.offset().left;
                $("#top").val(start_y);
                $("#left").val(start_x);
                $("#right").val(posi.width());
                $("#bottom").val(posi.height());
                $("#cur_w").val(origin.width());
                $("#cur_h").val(origin.height());
                return true;
            });
        }

    </script>
</head>

<body>
    <?php
    if(!$_GET['path']) {
        echo "<script>
            window.alert(\"Wrong Access!\");
            location.href(\"../../index.php\");
            </script>";
    } else $path = $_GET['path'];
    ?>
        <div id='controls'>
            <form action="do_crop.php">
                <input type="hidden" value="<?=$path?>" name="path">;
                <input type="hidden" value="" id="top" name="top">
                <input type="hidden" value="" id="left" name="left">
                <input type="hidden" value="" id="right" name="right">
                <input type="hidden" value="" id="bottom" name="bottom">
                <input type="hidden" value="" id="cur_w" name="cur_w">
                <input type="hidden" value="" id="cur_h" name="cur_h">
                <button type="submit" class="btn"><img src="../../img/basic/ok.svg" alt=""></button>
            </form>
            <button class="btn" id='zoom_in' type='button'>
        <img src="../../img/basic/zoom_in.svg" alt="Zoom in"></button>
            <button class="btn" id='zoom_out' type='button'>
        <img src="../../img/basic/zoom_out.svg" alt="Zoom out"></button>
            <button class="btn" id='fit' type='button'>
        <img src="../../img/basic/fit_size.svg" alt="Fit image"></button>
        </div>
        <div id='content'>
            <h1>Please adjust the image size for use as <strong>thumbnails</strong> on the main page.</h1>
            <ul id='data'>
                <li>x: <span id='x'></span></li>
                <li>y: <span id='y'></span></li>
                <li>scale: <span id='scale'></span></li>
            </ul>
            <div class='frame'>
                <img id='sample_picture' src="../board/img/original/<?=$path?>" onload="javascript:img_info(this);">
                <div id="crop_space"></div>
            </div>
        </div>
        <script src='../../js/lib/jquery.guillotine.min.js'></script>
        <script type='text/javascript'>
            jQuery(function() {
                var picture = $('#sample_picture');
                // Make sure the image is completely loaded before calling the plugin
                picture.one('load', function() {
                    // Initialize plugin (with custom event)
                    picture.guillotine({
                        eventOnChange: 'guillotinechange'
                    });

                    // Display inital data
                    var data = picture.guillotine('getData');
                    for (var key in data) {
                        $('#' + key).html(data[key]);
                    }

                    // Bind button actions
                    $('#fit').click(function() {
                        picture.guillotine('fit');
                    });
                    $('#zoom_in').click(function() {
                        picture.guillotine('zoomIn');
                    });
                    $('#zoom_out').click(function() {
                        picture.guillotine('zoomOut');
                    });

                    // Update data on change
                    picture.on('guillotinechange', function(ev, data, action) {
                        data.scale = parseFloat(data.scale.toFixed(2));
                        for (var k in data) {
                            $('#' + k).html(data[k]);
                        }
                    });
                });

                // Make sure the 'load' event is triggered at least once (for cached images)
                if (picture.prop('complete')) picture.trigger('load')
            });

        </script>
</body>

</html>
