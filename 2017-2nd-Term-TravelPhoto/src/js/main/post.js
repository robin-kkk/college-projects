$(document).ready(function () {
    var $background = $(".post-background");
    var $post = $(".post-wrapper");
    var $openbtn = $("#read_more");
    var PostArr = [];

    TweenMax.set([$background], {
        autoAlpha: 0
    });

    var Post = function (wrapper, id) {
        this.wrapper = wrapper;
        this.postclose = this.wrapper.children(".post-close");
        this.imgwrapper = this.wrapper.children(".img-wrapper");
        this.descwrapper = this.wrapper.children(".desc-wrapper");
        this.postdelete = this.descwrapper.find(".post-delete");
        this.cmtdelete = this.descwrapper.find(".reply-delete");
        this.likebtn = this.descwrapper.find(".like");
        this.cmt = this.wrapper.children(".input-comment");
        this.cmttoggle = this.cmt.children(".comment-toggle");
        this.replybtn = this.descwrapper.find(".re-reply");
        this.cmt_y = this.cmt.height();
        this.pnum = id;
        this.isRemoved = false;
        this.cmt_open = false;
        this.re_reply_open = false;
        var self = this;

        Post.prototype.init = function () {
            var self = this;
            TweenMax.set(this.wrapper, {
                autoAlpha: 0
            });
            TweenMax.set(this.cmt, {
                y: 85
            });
            if (this.descwrapper.height() > $(window).height()) {
                this.descwrapper.removeClass("set_zero_to_bottom");
                this.wrapper.css("overflow-y", "scroll");
            } else {
                this.descwrapper.addClass("set_zero_to_bottom");
            }
            this.postclose.click(function () {
                self.close();
            });
            this.postdelete.click(function() {
                var res = confirm('Would you like to delete this post?');
                if (res) {
                    var writer = $(this).data("writer");
                    $.ajax({
                        url: "../../php/board/delete_post.php",
                        type: "POST",
                        data: {
                            "pnum": self.pnum,
                            "writer": writer
                        },
                        success: function() {
                            $(".gallery-picture").eq(cur_idx).remove();
                            PostArr.splice(cur_idx, 1);
                            self.postclose.trigger("click");
                            $(".post-wrapper").remove("#"+self.pnum);
                        }
                    });
                }
            });
            this.likebtn.click(function() {
                var btn = $(this);
                var liked = !btn.hasClass("like_before");
                var pnum = btn.attr("value");
                $.ajax({
                    url: "../../php/board/like.php",
                    type: "POST",
                    data: {
                        "pnum": pnum,
                        "liked": liked
                    },
                    success: function() {
                        var path = "img/basic/";
                        var label = btn.next();
                        btn.toggleClass("like_before");
                        btn.toggleClass("like_after");
                        var liker = parseInt(label.attr("value"));
                        if (liked) {
                            liker--;
                            path += "like.svg";
                        } else {
                            liker++;
                            path += "like_after.svg";
                        }
                        label.attr("value", liker);
                        label.html(liker);
                        TweenMax.fromTo([label, btn], 0.2, {
                            opacity: 0,
                            onComplete: function() {
                                btn.children("img").attr("src", path);
                            }
                        }, {
                            opacity: 1
                        });
                    }
                });
            });
            this.cmt.children("form").submit(function(e) {
                e.preventDefault();
                var form = $(this);
                var refer = form.children(".refer").val();
                $.ajax({
                    url: "../../php/board/comment/write_comment.php",
                    type: "POST",
                    data: {
                        "pnum": self.pnum,
                        "refer": refer,
                        "contents": form.children("textarea").val()
                    },
                    success: function(data) {
                        if ((data == "Wrong Access.") || (data == "Contents is empty.")) {
                            alert(data);
                            return;
                        } 
                        var cmtTable = self.wrapper.find(".comment>table");
                        var target = null;
                        if (refer == 0) {
                            if (cmtTable.children(".reply1").length == 0)
                                target = cmtTable.html(data).children(".reply1").hide().fadeIn();
                            else
                                target = cmtTable.append(data).children(".reply1:last").hide().fadeIn();
                        } else {
                            if ($(".refer"+refer).length == 0) target = $("#cnum"+refer);
                            else target = $(".refer"+refer+":last");
                            target.after(data).next().hide().fadeIn();
                        }
                        TweenMax.fromTo(target.next(), 0.8, {color: "red"}, {color: "black"});
                        if (self.descwrapper.height() > $(window).height()) {
                            self.descwrapper.removeClass("set_zero_to_bottom");
                            self.wrapper.css("overflow-y", "scroll");
                            var pos;
                            if (refer == 0) {
                                pos = cmtTable.height() + cmtTable.offset().top;
                                self.wrapper.animate({scrollTop : pos}, 400);   
                            } else {
                                pos = target.offset().top + target.height();
                                self.wrapper.animate({scrollTop : pos}, 400);
                            }
                        } else {
                            self.descwrapper.addClass("set_zero_to_bottom");
                        }
                        var count = parseInt(self.descwrapper.find(".cmtcount").text())+1;
                        self.descwrapper.find(".cmtcount").text(count).fadeOut().fadeIn();
                        self.cmttoggle.trigger("click");
                    }
                })
            })
            this.cmttoggle.click(function () {
                if (self.cmt_open) {
                    TweenMax.to(self.cmt, 1, {
                        y: 85
                    });
                    self.re_reply_open = false;
                    self.cmt.find(".refer").val(0);
                    self.cmttoggle.html("+").fadeOut(5).fadeIn();
                } else {
                    TweenMax.to(self.cmt, 1, {
                        y: 0
                    });
                    self.cmttoggle.html("-").fadeOut(5).fadeIn();
                }
                self.cmt_open = !self.cmt_open;
                self.cmt.children("form").trigger("reset");
                self.cmt.find("textarea").attr("placeholder", "Comment : ");
            });
            this.descwrapper.on("click", ".re-reply", function() {
                var btn = $(this);
                self.cmttoggle.html("-").fadeOut(5).fadeIn();
                self.cmt.children("form").trigger("reset");
                self.cmt.find("textarea").attr("placeholder", "Reply to "+btn.attr("value"));
                var end_y =  -($(window).height()-btn.offset().top-130);
                if (end_y > 85) end_y = 85;
                TweenMax.to(self.cmt, 1, {
                    y: end_y
                });
                self.cmt_open = true;
                self.re_reply_open = true;
                self.cmt.find(".refer").val(btn.attr("name"));
            });
            
            this.descwrapper.on("click", ".reply-delete", function() {
               var btn = $(this);
                btn.click(function() {
                    var res = confirm('Would you like to delete this comment?');
                    if (res) {
                        var cnum = btn.data("cnum")
                        var c_writer = btn.data("cwriter");
                        $.ajax({
                            url: "../../php/board/comment/delete_comment.php",
                            type: "POST",
                            data: {
                                "pnum": self.pnum,
                                "cnum": cnum,
                                "c_writer": c_writer
                            },
                            success: function(data) {
                                if (data.search("Wrong") != -1) {
                                    alert(data);
                                    return;
                                }
                                var count = parseInt(self.descwrapper.find(".cmtcount").text())-1;
                                var target = btn.closest("tr");
                                if (target.hasClass("reply1") && ($(".refer"+cnum).length != 0)) {
                                    target = $(".refer"+cnum+","+"#cnum"+cnum);
                                    count -= $(".refer"+cnum).length;
                                }
                                
                                target.fadeOut("slow", function() {
                                   $(this).remove();
                                });
                                if (self.descwrapper.height() > $(window).height()) {
                                    self.descwrapper.removeClass("set_zero_to_bottom");
                                    self.wrapper.css("overflow-y", "scroll");
                                } else {
                                    self.descwrapper.addClass("set_zero_to_bottom");
                                }
                                self.descwrapper.find(".cmtcount").text(count).fadeOut().fadeIn();
                            }
                        })
                    }
                });
            });
        };
        
        Post.prototype.open = function () {
            var self = this;
            this.postclose.hide();
            this.descwrapper.hide();
            this.cmt.hide();
            TweenMax.to($(".modal-overlay"), 0.3, {
                autoAlpha: 1
            });
            TweenMax.fromTo([$background, this.wrapper], 1, {
                delay: 0.1,
                width: "0px",
                autoAlpha: 1
            }, {
                width: "100%",
                ease: Quint.easeIn
            });
            TweenMax.fromTo(this.imgwrapper, 1, {
                delay: 0.2,
                opacity: 0,
                x: $(window).width() / 2,
                width: "0px"
            }, {
                opacity: 1,
                x: 0,
                width: "50%",
                ease: Quint.easeIn,
                autoAlpha: 1,
                onComplete: function () {
                    // onComplete 시 호출하는 주체가 TweenMax 이므로 self
                    self.descwrapper.show();
                    self.postclose.show();
                    self.cmt.show();
                    TweenMax.fromTo([self.descwrapper, self.postclose, self.cmt], 0.3, {
                        scale: 0.9,
                        opacity: 0
                    }, {
                        delay: 0.2,
                        scale: 1,
                        opacity: 1
                    });
                }
            });
        };

        Post.prototype.close = function () {
            this.postclose.hide();
            this.cmt.hide();
            var self = this;
            TweenMax.to($(".modal-overlay"), 0.3, {
                delay: 0.3,
                autoAlpha: 0
            });
            TweenMax.to([this.descwrapper, this.imgwrapper], 0.2, {
                opacity: 0,
                scale: 0.9
            });
            TweenMax.to([this.wrapper, $background], 1, {
                delay: 0.3,
                width: "0px",
                ease: Quint.easeOut,
                onComplete: function () {
                    TweenMax.set([self.wrapper, $background], {
                        autoAlpha: 0
                    });
                }
            });
            TweenMax.set(this.cmt, {
                y: 85
            });
        };

        Post.prototype.getPnum = function () {
            return this.pnum;
        };

        self.init();
    }

    $post.each(function (i) {
        PostArr.push(new Post($(this), $(this).attr("id")));
    });

    $openbtn.hover(function () {
        $(".gallery-picture").each(function (idx) {
            var x = $(this).offset().left;
            var middle = ($(window).width()) / 2;
            if ((middle > x) && (middle < (x + 700))) cur_idx = idx;
        });
    });

    $openbtn.click(function () {
        var post = PostArr[cur_idx];
        if (post == null) return;
        if (post.getPnum() != $(".gallery-picture").children("img")[cur_idx].getAttribute("alt"))
            return;
        post.open();
    });
});
