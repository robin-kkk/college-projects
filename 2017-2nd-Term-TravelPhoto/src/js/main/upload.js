(function(document, window) {
    'use strict';
    
    if (!String.prototype.trim) {
        String.prototype.trim = function() { return this.replace(/^\s+|\s+$/g, ''); };
    }
    
    function NLForm(el) {
        this.el = el;
        this.overlay = this.el.querySelector( '.nl-overlay' );
        this.fields = []; // 커스텀 필드를 담을 배열
        this.fldOpen = -1; // 각 커스텀 필드의 순서를 위한 인덱스 역할
        this._init();
    }
    
    NLForm.prototype = {
		_init : function() {
			var self = this;
            // select 태그 객체를 모두 배열로 변환하여 각 원소들을 fields 배열에 추가
			Array.prototype.slice.call( this.el.querySelectorAll( 'select' ) ).forEach( function( el, i ) {
				self.fldOpen++;
				self.fields.push( new NLField( self, el, 'dropdown', self.fldOpen ) );
			} );
			this.overlay.addEventListener( 'click', function(ev) { self._closeFlds(); } );
			this.overlay.addEventListener( 'touchstart', function(ev) { self._closeFlds(); } );
		},
		_closeFlds : function() {
			if( this.fldOpen !== -1 ) {
				this.fields[ this.fldOpen ].close(); // 커스텀 필드 창을 닫는다.
			}
		}
	}
    function NLField( form, el, type, idx ) {
		this.form = form;
		this.elOriginal = el; // original HTML element -> selector ... etc
		this.pos = idx;
		this.type = type;
		this._create();
		this._initEvents();
	}

	NLField.prototype = {
		_create : function() {
			if( this.type === 'dropdown' ) {
				this._createDropDown();	
			}
		},
		_createDropDown : function() {
			var self = this;
			this.fld = document.createElement( 'div' );
			this.fld.className = 'nl-field nl-dd';
			this.toggle = document.createElement( 'a' );
            this.toggle.id = 'toggle'+this.pos;
			this.toggle.innerHTML = this.elOriginal.options[ this.elOriginal.selectedIndex ].innerHTML;
			this.toggle.className = 'nl-field-toggle';
			this.optionsList = document.createElement( 'ul' );
            this.optionsList.id = 'dropdown'+this.pos;
			var ihtml = '';
			Array.prototype.slice.call( this.elOriginal.querySelectorAll( 'option' ) ).forEach( function( el, i ) {
				ihtml += self.elOriginal.selectedIndex === i ? '<li class="nl-dd-checked">' + el.innerHTML + '</li>' : '<li>' + el.innerHTML + '</li>';
				// selected index value
				if( self.elOriginal.selectedIndex === i ) {
					self.selectedIdx = i;
				}
			} );
			this.optionsList.innerHTML = ihtml;
			this.fld.appendChild( this.toggle );
			this.fld.appendChild( this.optionsList );
			this.elOriginal.parentNode.insertBefore( this.fld, this.elOriginal );
			this.elOriginal.style.display = 'none';
		},
		_initEvents : function() {
			var self = this;
			this.toggle.addEventListener( 'click', function( ev ) { ev.preventDefault(); ev.stopPropagation(); self._open(); } );
			this.toggle.addEventListener( 'touchstart', function( ev ) { ev.preventDefault(); ev.stopPropagation(); self._open(); } );

			if( this.type === 'dropdown' ) {
				var opts = Array.prototype.slice.call( this.optionsList.querySelectorAll( 'li' ) );
				opts.forEach( function( el, i ) {
					el.addEventListener( 'click', function( ev ) { ev.preventDefault(); self.close( el, opts.indexOf( el ) ); } );
					el.addEventListener( 'touchstart', function( ev ) { ev.preventDefault(); self.close( el, opts.indexOf( el ) ); } );
				} );
			}
		},
		_open : function() {
			if( this.open ) {
				return false;
			}
			this.open = true;
			this.form.fldOpen = this.pos;
			var self = this;
			this.fld.className += ' nl-field-open';
		},
		close : function( opt, idx ) {
			if( !this.open ) {
				return false;
			}
			this.open = false;
			this.form.fldOpen = -1;
			this.fld.className = this.fld.className.replace(/\b nl-field-open\b/,'');

			if( this.type === 'dropdown' ) {
				if( opt ) {
					// remove class nl-dd-checked from previous option
					var selectedopt = this.optionsList.children[ this.selectedIdx ];
					selectedopt.className = '';
					opt.className = 'nl-dd-checked';
					this.toggle.innerHTML = opt.innerHTML;
					// update selected index value
					this.selectedIdx = idx;
					// update original select element´s value
					this.elOriginal.value = this.elOriginal.children[ this.selectedIdx ].value;
                    $("#nation").trigger("change");
				}
			}
		}
	}

    var input = document.getElementById("file");
    var label	 = input.nextElementSibling;
    var labelVal = label.innerHTML;

    input.addEventListener( 'change', function( e )
    {
        var fileName = '';
        if( this.files && this.files.length > 1 )
            fileName = ( this.getAttribute( 'data-multiple-caption' ) || '' ).replace( '{count}', this.files.length );
        else
            fileName = e.target.value.split( '\\' ).pop();

        if( fileName )
            label.querySelector( 'span' ).innerHTML = fileName;
        else
            label.innerHTML = labelVal;
    });

    // Firefox bug fix
    input.addEventListener( 'focus', function(){ input.classList.add( 'has-focus' ); });
    input.addEventListener( 'blur', function(){ input.classList.remove( 'has-focus' ); });
    
    $("#nl-form").submit(function(e) {
        e.preventDefault();
        var datas = new FormData(this);
        $.ajax({
			url: "../../php/board/write_post.php",
			type: "POST",
			data: datas,
            success: function(data) {
                if(data.search("_") != -1)
                    location.href="../../php/board/make_main_thumb.php?path="+data;
                else alert(data);
            },
            cache: false,
            contentType: false,
            processData: false
		});
        return false; // 새로고침방지
    });
    
	// add to global namespace
	window.NLForm = NLForm;
} )( document, window );