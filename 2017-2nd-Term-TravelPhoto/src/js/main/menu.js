$(document).ready(function() {
    function SVGMenu(el, options) {
        this.el = el;
        this.init();
    }

    SVGMenu.prototype.init = function () {
        this.trigger = this.el.querySelector('button.menu-handle');
        this.shapeEl = this.el.querySelector('div.morph-shape');

        var s = Snap(this.shapeEl.querySelector('svg'));
        this.pathEl = s.select('path');
        this.paths = {
            reset: this.pathEl.attr('d'),
            open: this.shapeEl.getAttribute('data-morph-open'),
            close: this.shapeEl.getAttribute('data-morph-close')
        };

        this.isOpen = false;

        this.initEvents();
    };

    SVGMenu.prototype.initEvents = function () {
        this.trigger.addEventListener('click', this.toggle.bind(this));
    };

    SVGMenu.prototype.toggle = function () {
        var self = this;

        if (this.isOpen) {
            classie.remove(self.el, 'menu--anim');
            setTimeout(function () {
                classie.remove(self.el, 'menu--open');
            }, 250);

            this.pathEl.stop().animate({
                'path': this.paths.close
            }, 350, mina.easeout, function () {
                self.pathEl.stop().animate({
                    'path': self.paths.reset
                }, 700, mina.elastic);
            });

            TweenMax.to($(".modal-overlay"), 0.3, {
                delay: 0.3,
                autoAlpha: 0
            });
        } else {
            classie.add(self.el, 'menu--anim');
            setTimeout(function () {
                classie.add(self.el, 'menu--open');
            }, 250);

            this.pathEl.stop().animate({
                'path': this.paths.open
            }, 350, mina.backin, function () {
                self.pathEl.stop().animate({
                    'path': self.paths.reset
                }, 700, mina.elastic);
            });

            TweenMax.to($(".modal-overlay"), 0.3, {
                autoAlpha: 1
            });
        }
        this.isOpen = !this.isOpen;
    };
    var menu = new SVGMenu(document.getElementById('menu'));
});
