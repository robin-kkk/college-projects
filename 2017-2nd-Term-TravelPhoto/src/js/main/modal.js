$(document).ready(function() {
    var $modal = null,
        $login = $(".login-modal"),
        $upload = $(".upload-modal"),
        $overlay = $(".modal-overlay"),
        $joindiv = $(".member"),
        blocked = false,
        unblockTimeout = null;

    $joindiv.hide();
    TweenMax.set($login, {
        autoAlpha: 0
    });
    TweenMax.set($upload, {
        autoAlpha: 0
    });

    var isOpen = false;

    function openModal(which) {
        if (which == 1) {
            $modal = $(".login-modal");
            if ($joindiv.is(':visible')) return;
        } else if (which == 2) $modal = $(".upload-modal");

        if (!blocked) {
            block(400); // 0.4초 동안 대기시킨다.

            // $(".modal") 을 0.5초 동안 시작값에서 끝값으로 변화
            TweenMax.fromTo($modal, 0.5, {
                // 시작값
                x: (-$(window).width() - $modal.width()) / 2 - 50,
                scale: 0.9,
                autoAlpha: 1
            }, {
                // 끝값
                delay: 0.1,
                rotationY: 0,
                scale: 1,
                opacity: 1,
                x: 0,
                z: 0,
                ease: Elastic.easeOut,
                easeParams: [0.4, 0.3],
                force3D: false
            });
            $.startUpdatingBlur(800);
        }
    }

    function closeModal() {
        if (!blocked) {
            block(400);

            TweenMax.to($modal, 0.3, {
                x: ($(window).width() + $modal.width()) / 2 + 100,
                scale: 0.9,
                ease: Quad.easeInOut,
                force3D: false,
                onComplete: function () {
                    TweenMax.set($modal, {
                        autoAlpha: 0
                    });
                }
            });
            $.startUpdatingBlur(400);
        }
    }

    function block(t) {
        blocked = true; // openModal 내부의 로직이 블록킹 상태에서 여러번 실행되지 않도록하기 위함.
        if (unblockTimeout != null) {
            clearTimeout(unblockTimeout); // 해당 타이머 취소
            unblockTimeout = null;
        }
        // 타이머가 만료된 뒤 특정 로직을 실행하는 타이머를 설정
        // 반환값은 타이머를 식별할 수 있는 정수형의 timeoutID
        unblockTimeout = setTimeout(unblock, t);
    }

    function unblock() {
        blocked = false;
    }
    $(".login").click(function () {
        $login.children("form").trigger("reset");
        openModal(1);
    })
    $(".upload").click(function () {
        var tmp = getCookie("COOKIES");
        if (tmp == "") alert("Available after login.");
        else {
            $("#nl-form").trigger("reset");
            $(".file-div").find("label").children("span").text("");
            $("#dropdown0").children("li").each(function (i) {
                $(this).removeClass("nl-dd-checked");
            });
            $("#toggle0").text("Not feeling");
            $("#dropdown1").children("li").each(function (i) {
                $(this).removeClass("nl-dd-checked");
            });
            $("#toggle1").text("Any country");
            openModal(2);
        }
    })
    $(".close-modal, .modal-overlay").click(function () {
        if ($joindiv.is(':visible')) return;
        if ($upload.children("form").has(".nl-field-open").length != 0) return;

        closeModal();
    })

    $login.initBlur(0.5);
    $upload.initBlur(0.5);

    /* SIGN-UP HELPER FUNCTION */
    $(".join-btn").click(function () {
        memberform_focus();
        $joindiv.children("form").trigger('reset');
        TweenMax.to($login.children("form"), 0.3, {
            opacity: 0,
            onComplete: function () {
                $login.children("form").hide();
            }
        });
        TweenMax.to($login, 1, {
            width: "600px",
            height: "550px",
            onComplete: function () {
                $joindiv.show();
                TweenMax.fromTo($joindiv, 0.5, {
                    opacity: 0,
                    scale: 0.8
                }, {
                    opacity: 1,
                    scale: 1
                });

                TweenMax.fromTo($("#close-join"), 0.5, {
                    opacity: 0,
                    scale: 0.9
                }, {
                    opacity: 1,
                    scale: 1
                });
            }
        });
    });
    
    $("#signup").click(function() {
        $.ajax({
			url: "../../php/member/join.php",
			type: "POST",
			data: $joindiv.children("form").serialize(),
            success: function(data) {
                alert(data);
                if(data.search("congratulate") != -1) {
                    memberform_focus();
                    $joindiv.children("form").trigger('reset');
                    $("#close-join").trigger("click");
                }
            }
		});
        return false; // 새로고침방지
    });

    $("#close-join").click(function () {
        $joindiv.hide();
        TweenMax.to($login, 0.8, {
            width: "400px",
            height: "350px",
            onComplete: function () {
                var $closelogin = $("#close-login");
                $login.children("form").trigger("reset");
                $login.children("form").show();
                TweenMax.fromTo([$login.children("form"), $closelogin], 0.5, {
                    opacity: 0
                }, {
                    opacity: 1,
                    onComplete: function () {
                        $closelogin.removeClass(".close-modal");
                        $closelogin.addClass(".close-modal");
                    }
                });
            }
        });

        [].slice.call(document.querySelectorAll('input.join-input-field')).forEach(function (inputEl) {
            // in case the input is already filled..
            if (inputEl.value.trim() !== '') {
                classie.remove(inputEl.parentNode, 'input--filled');
            }
        });
    });

    function memberform_focus() {
        if (!String.prototype.trim) {
            (function () {
                var rtrim = /^[\s\uFEFF\xA0]+|[\s\uFEFF\xA0]+$/g;
                String.prototype.trim = function () {
                    return this.replace(rtrim, '');
                };
            })();
        }

            [].slice.call(document.querySelectorAll('input.join-input-field')).forEach(function (inputEl) {
            // in case the input is already filled..
            if (inputEl.value.trim() !== '') {
                classie.add(inputEl.parentNode, 'input--filled');
            }

            // events:
            inputEl.addEventListener('focus', onInputFocus);
            inputEl.addEventListener('blur', onInputBlur);
        });

        function onInputFocus(ev) {
            classie.add(ev.target.parentNode, 'input--filled');
        }

        function onInputBlur(ev) {
            if (ev.target.value.trim() === '') {
                classie.remove(ev.target.parentNode, 'input--filled');
            }
        }
    };
});
