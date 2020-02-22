(function(){
	var minBlur=2,
        maxBlur=200,
        isUpdatingBlur=false,
        updateBlurStopTimeout=null,
        multiplier=0.25
    ;
    
    // $.fn = $.prototype
    // 제이쿼리에 속성(메소드)를 추가한다.
    // 제이쿼리 객체 대상으로 모두 사용가능.
	$.fn.toggleBlur=function(v){ // 블러 필터 적용을 결정하는 함수
		var blurId=$(this).data("blur-id");
		var value=v?"url(#"+blurId+")":"none"; // set the CSS
		$(this).css({
			webkitFilter:value,
			filter:value
		});
	}
	$.fn.setBlur=function(v){ // 애니메이션의 각 프레임마다 블러효과를 추가한다.
		var blur=$(this).data("blur"); // data("blur") == blurClone
		v=Math.round(v);
		if($(this).data("blur-value")!=v){
			if(v==0){
				$(this).toggleBlur(false);
			}else{
				$(this).toggleBlur(true);

                // 속도를 계산해서 모든 프레임마다 블러필터를 동적으로 갱신함
				blur.firstElementChild.setAttribute("stdDeviation",v+",0");
				$(this).data("blur-value",v);
			}
		}
	}
	$.fn.initBlur=function(_multiplier){
		if(typeof _multiplier=="undefined") _multiplier=1;
		multiplier=_multiplier;
		var defs=$(".filters defs").get(0);
		var blur=$("#blur").get(0);
		$(this).each(function(i){ // this -> 함수가 속한 객체(div 태그)
			var blurClone=blur.cloneNode(true); // 필터복사(생성)
			var blurId="blur"+i; // create a new ID
			blurClone.setAttribute("id",blurId); // set a new ID so we can use the filter through CSS
			defs.appendChild(blurClone); // 복제한 필터를 svg -> defs 에 추가.
			$(this) // 함수형식 : data(key, value)
				.data("blur",blurClone) // 실제 객체에 블러 필터를 추가
				.data("blur-id",blurId) // 생성한 new ID 추가
				.data("blur-value",0) // 초기 블러 값 저장
				.data("last-pos",$(this).offset()) // 현재 위치 저장
			;
		});
	}

	$.updateBlur=function(){
		$(".modal").each(function(){ // blur 를 적용할 요소의 class 이름은 .modal
			var pos=$(this).offset(); // 현재 오프셋 가져오기
			var lastPos=$(this).data("last-pos"); // 초기or이전 오프셋 가져오기
			var v=Math.abs(pos.left-lastPos.left)*multiplier; // 거리 계산
			$(this).data("last-pos",pos); // 위치 저장
			$(this).setBlur(v); // 계산한 속도를 복제한 블러와 함께 적용시키는 함수
		})
		if(isUpdatingBlur){
			// setTimeout : 설정한 주파수 간격으로 그리기 요청이 발생하는데, 디스플레이의 주파수 간격 (그리기)과 맞지
			// 않을 경우, 프레임이 손실되어 애니메이션이 끊기는 현상이 발생한다.
			// 그러나 requestAnimationFrame 은 브라우저가 페이지 디스플레이를 업데이트해야 할 때만 앱에 알리도록
			// 할 수 있으므로 프레임 손실 문제를 해결한다. 따라서 앱이 브라우저 그리기 간격에 맞춰 완벽하게 조정된다. 
			// 시스템이 프레임을 그릴 준비가 되었을 때 애니메이션 프레임을 호출하여 애니메이션 웹 페이지를 만들 수 있게 한다. 
			// 애니메이션을 지속하려면 애니메이션 함수(여기서는 updateBlur)가 호출된 후에 다시 호출한다.
			requestAnimationFrame($.updateBlur);
			// 매 위치 변경 후 다시 호출되어 다음 이동을 설정한다.
			// 브라우저를 닫거나 isUpdatingBlur = false 일 때까지 계속된다.
		}
	}
	$.startUpdatingBlur=function(stopDelay){
		if(typeof stopDelay=="undefined"){
			stopDelay=-1;
		}
		if(updateBlurStopTimeout!=null){
			clearTimeout(updateBlurStopTimeout);
			updateBlurStopTimeout=null;
		}
		if(!isUpdatingBlur){ // 맨처음 실행되는 로직
			isUpdatingBlur=true;
			$.updateBlur(); // 매 프레임마다 블러 효과를 갱신하는 함수
		}
		if(stopDelay>-1){
			updateBlurStopTimeout=setTimeout($.stopUpdatingBlur,stopDelay);
		}
	}
	$.stopUpdatingBlur=function(){
		isUpdatingBlur=false; // 정해진 시간(여기서는 열 때 0.8초, 닫을 때 0.4초)이후에 updateBlur 함수를 종료
	}
})();