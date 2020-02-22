$(document).ready(function() {
    var
        $gallery = $(".gallery"),
        $galleryPictures = $(".gallery-pictures"),
        $galleryPicture = $(".gallery-picture"),
        lastPos = {
            x: 0
        },
        galleryPos = {
            x: 0
        },
        currentImage = -1,
        imageWidth = 700,
        imageSpacing = 120,
        imageTotalWidth = imageWidth + imageSpacing,
        speedLog = [],
        speedLogLimit = 5,
        minBlur = 2,
        maxBlur = 200,
        blurMultiplier = 0.25,
        lastBlur = 0,
        dragging = false,
        lastDragPos = {
            x: 0
        },
        dragPos = {
            x: 0
        },
        totalDist = 0,
        distThreshold = 10,
        distLog = [],
        distLogLimit = 10,
        momentumTween = null;

    function setBlur(v) {
        if (v < minBlur) v = 0;
        if (v > maxBlur) v = maxBlur;
        if (v != lastBlur) {
            $("#blur").get(0).firstElementChild.setAttribute("stdDeviation", v + ",0");
        }
        lastBlur = v;
    }

    // ul 태그에 모든 그림을 삽입한 후, 마우스 드래그 값에 따라 ul의 좌표가 이동함.
    $galleryPictures.css({ // ul 태그(사진목록)에 css(svg 태그의 filter 속성) 추가
        webkitFilter: "url('#blur')",
        filter: "url('#blur')"
    });

    $galleryPicture.each(function (i) {
        var cur = $(this);
        cur.click(function() {
            if (Math.abs(totalDist) < distThreshold)
                setGalleryPos(i);
        })
    });

    function setGalleryPos(v, anim) {
        if (typeof anim == "undefined") anim = true;
        stopMomentum();
        TweenMax.to(galleryPos, anim ? 0.8 : 0, {
            x: -v * imageTotalWidth,
            ease: Quint.easeOut,
            onUpdate: updateGalleryPos,
            onComplete: updateGalleryPos
        });
    }

    function updateGalleryPos() {
        TweenMax.set($galleryPictures, {
            x: galleryPos.x + (($(window).width() - imageWidth) / 2),
            force3D: true,
            lazy: true
        });
        var speed = lastPos.x - galleryPos.x;
        var blur = Math.abs(Math.round(speed * blurMultiplier));
        setBlur(blur);
        lastPos.x = galleryPos.x;
    }

    $gallery.mousedown(function (event) { // 사진 영역(div)에서 마우스를 누른 상태일 때
        event.preventDefault();
        dragging = true;
        dragPos.x = event.pageX; // pageX : 브라우저상에서 마우스의 위치
        lastDragPos.x = dragPos.x; // 이전위치를저장
        totalDist = 0;
        distLog = [];
        stopMomentum();
        updateGalleryPosLoop();
    });
    
    $(document).mousemove(function (event) { // 화면에서 마우스가 움직일 때
        if (dragging) {
            dragPos.x = event.pageX;
        }
    });

    function updateGalleryPosLoop() {
        if (dragging) {
            updateGalleryPos();
            var dist = dragPos.x - lastDragPos.x;
            lastDragPos.x = dragPos.x;
            totalDist += dist;
            distLog.push(dist);
            while (distLog.length > distLogLimit) {
                distLog.splice(0, 1);
            };
            galleryPos.x += dist;
            requestAnimationFrame(updateGalleryPosLoop)
        }
    }

    $(document).mouseup(function (event) { // 화면에서 마우스를 눌렀다가 땔 때
        if (dragging) {
            dragging = false;
            var releaseSpeed = 0;
            for (var i = 0; i < distLog.length; i++) {
                releaseSpeed += distLog[i];
            };
            releaseSpeed /= distLog.length;

            var targetX = galleryPos.x + (releaseSpeed * 20);
            targetX = Math.round(targetX / imageTotalWidth) * imageTotalWidth;
            var targetImage = -targetX / imageTotalWidth;
            var excess = 0;
            if (targetImage < 0) {
                excess = targetImage;
                targetImage = 0;
            } else if (targetImage >= $galleryPicture.length) {
                excess = targetImage - ($galleryPicture.length - 1);
                targetImage = $galleryPicture.length - 1;
            }
            if (excess != 0) {
                targetX = -targetImage * imageTotalWidth;
            }
            momentumTween = TweenMax.to(galleryPos, 1 - (Math.abs(excess) / 20), {
                x: targetX,
                ease: Quint.easeOut,
                onUpdate: updateGalleryPos,
                onComplete: updateGalleryPos
            });

            if (Math.abs(totalDist) >= distThreshold) {
                event.preventDefault();
                event.stopPropagation();
            }
        }
    });

    function stopMomentum() {
        if (momentumTween != null) {
            momentumTween.kill();
            momentumTween = null;
            updateGalleryPos();
        }
    }

    setGalleryPos(0, false);
})
