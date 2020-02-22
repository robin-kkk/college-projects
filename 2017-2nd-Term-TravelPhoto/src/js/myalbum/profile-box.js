// read file img
var upload = document.getElementById('upload'),
    holder = document.getElementById('holder');

upload.onchange = function (e) {
    e.preventDefault();
    
    var file = upload.files[0],
        reader = new FileReader(),
        file_rout = document.getElementById('file_route');
    reader.onload = function (event) {
        var img = new Image();
        img.src = event.target.result;
        // note: no onload required since we've got the dataurl...I think! :)
        if (img.width > 80) { // holder width
            img.width = 80;
        }
        holder.innerHTML = '';
        holder.appendChild(img);
    };
    reader.readAsDataURL(file);
    file_rout.value=this.value;
    return false;
};

$("#ch-photo").click(function() {
    $("#overlay").fadeIn();
    $(".change-box").slideDown();
    $(".change-box input, .change-box label, .change-box img").fadeIn();
    $(".change-box form").trigger("reset");
    $("#holder").children("img").attr("src", "../../img/profile/basic_profile.svg");
    return false
});

$("#btn-box").children("button").each(function(idx){
    var btn = $(this);
    btn.click(function() {    
        if (idx == 0) {
            var datas = new FormData(document.getElementById("change-form"));
            $.ajax({
                url: "../../php/member/change_profile.php",
                type: "POST",
                data: datas,
                success: function(data) {
                    if(data == "Available after login." || data == "") {
                        alert(data);
                        return;
                    }
                    $("#profile-photo").children("img").attr("src", "../../img/profile/"+data);
                },
                cache: false,
                contentType: false,
                processData: false
            })
        }
        $(".change-box img, .change-box label").hide();
        $(".change-box").slideUp();
        $("#overlay").fadeOut();
        $(".change-box input").fadeOut();
        return false;
    });
});
