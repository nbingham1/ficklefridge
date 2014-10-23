var highlight = -1;
var ivalue = "";
var selected_recipe = "";

function adjust_slide()
{
    var offset = $("#recipe").height() + 3 - $(window).scrollTop();
    $(".slide").css("margin-top", Math.max(0, -offset).toString() + "px");
    $(".slide").css("height", ($(window).height() - Math.max(0, offset)).toString() + "px");
}

function reset_add_ingredient()
{
    $(".amount").each(function(){
        var target = this;
        $(target).off("input propertychange");
        $(target).on("input propertychange", function(){
            $(target).height(0);
            $(target).height($(target).prop("scrollHeight") - 10);
            adjust_slide();
        });

        $(target).height(0);
        $(target).height($(target).prop("scrollHeight") - 10);
    });

    $("#steps").width($(window).width()-473);
    $("#steps").height(0);
    $("#steps").css("line-height", "normal");
    $("#steps").height(Math.max($("#steps").prop("scrollHeight") - 10, $("#recipe-box").height()-12));
    if ($("#steps").val() == "add instruction")
    {
        $("#steps").css("line-height", $("#steps").height().toString() + "px");
    }

    $("#add-ingredient").off("focus");
    $("#add-ingredient").focus(function(){
        $("#add-ingredient").val("");
    });

    $("#add-ingredient").off("blur");
    $("#add-ingredient").blur(function(){
        if (!$("#suggestions").is(":hover"))
        {
            $("#add-ingredient").val("add ingredient");
            $("#add-ingredient").height(26);
            $("#suggestions").css("display", "none");
            adjust_slide();
        }
    });

    $("#add-ingredient").off("input propertychange");
    $("#add-ingredient").on("input propertychange", function(){
        $("#add-ingredient").height(0);
        $("#add-ingredient").height($("#add-ingredient").prop("scrollHeight") - 10);
        adjust_slide();
    });

    $("#add-ingredient-form").off("submit");
    $("#add-ingredient-form").submit(function(event){
        ivalue = "";
        highlight = -1;
        addstring = $("#add-ingredient").val();
        $("#suggestions").css("display", "none");
        if (addstring != "")
        {
            $.ajax({url: ("/get_id.php?ing=" + addstring.toLowerCase())}).done(function(id){
                if (id != "")
                    recipe.add(id);
                else
                {
                    $("#add-ingredient").css("color", "red");
                    $("#add-ingredient").css("font-weight", "bold");
                    setTimeout( function() {
                        $("#add-ingredient").css("color", "black");
                        $("#add-ingredient").css("font-weight", "normal");
                    }, 700);
                }
            });
        }
        event.preventDefault();
    });
   
    $("#add-ingredient").off("keydown"); 
    $("#add-ingredient").keydown(function(e) {
        $("#suggestions").css("display", "block");
        if (e.which == 38 || e.keyCode == 38)
        {
            if (ivalue == "")
                ivalue = $("#add-ingredient").val();
            $("#suggest" + highlight).css("background-color", "#FFFFFF");
            highlight--;
            if (highlight <= -1)
            {
                 $("#add-ingredient").val(ivalue);
                 ivalue = "";
                 highlight = -1;
            }
            else
            {
                $("#add-ingredient").val($("#suggest" + highlight).html());
                $("#suggest" + highlight).css("background-color", "#ADDFFF");
            }
        }
        else if (e.which == 40 || e.keyCode == 40)
        {
                if (ivalue == "")
            ivalue = $("#add-ingredient").val();
            $("#suggest" + highlight).css("background-color", "#FFFFFF");
                highlight++;
            if (highlight > 9)
                highlight = 9;
            $("#add-ingredient").val($("#suggest" + highlight).html());
            $("#suggest" + highlight).css("background-color", "#ADDFFF");
        }
        else if (e.which != 13 && e.keyCode != 13)
        {
            ivalue = "";
            highlight = -1;
            addstring = $("#add-ingredient").val();
            if (e.which == 83 || e.keyCode == 83 || e.which == 8 || e.keyCode == 8 || e.which == 46 || e.keyCode == 46)
                addstring = addstring.substr(0, addstring.length-1);
            else
                addstring += String.fromCharCode(e.which || e.keyCode);
            $.ajax({url: ("/suggest.php?ing=" + addstring.toLowerCase())}).done(function(ing) {
                $("#suggestions").html(ing);
                if (ing != "")
                    $("#suggestions").css("border-style", "solid");
                else
                    $("#suggestions").css("border-style", "none");
            });
        }
        else
        {
            $("#add-ingredient-form").submit();
            e.preventDefault();
        }
    });
}

function reset_functions()
{
    $(".match-title").off('click');
    $(".match-title").click(function() {
        var id = $(this).attr('id').substr(11);
	var old_selected = selected_recipe;
        selected_recipe = id;
        $("#title").val($(this).html());
        $("#title").height(0);
        $("#title").height($("#title").prop("scrollHeight") - 10);
        $.ajax({url: ("/update.php?selected=" + id)}).done(function(ing) {
            var step=ing.search("====");
            $("#ingredients").html(ing.substr(step+4));
            reset_add_ingredient();
            var stepstring = ing.substr(0, step);
            var result = "";
            for (var i = 0; i < stepstring.length; i++)
            {
                if (stepstring.substr(i, 4) == "<br>")
                {
                    result += "\n";
                    i += 3;
                }
                else
                    result += stepstring[i];
            }
            $("#steps").val(result.trim());
            $("#steps").css("text-align", "left");
            $("#steps").css("color", "#000000");
            $("#steps").css("line-height", "normal");
            $("#steps").trigger("input");
            adjust_slide();
        });
    });
}

$(window).resize(function() {
    $(".dyn-textarea").each(function(){
        var target = this;
        $(target).height(0);
        $(target).height($(target).prop("scrollHeight") - 10);
        adjust_slide();
    });

    $("#steps").width($(window).width() - 473);
    $("#steps").height(0);
    $("#steps").css("line-height", "normal");
    $("#steps").height(Math.max($("#steps").prop("scrollHeight") - 10, $("#recipe-box").height()-12));
    if ($("#steps").val() == "add instruction")
    {
        $("#steps").css("line-height", $("#steps").height().toString() + "px");
    }

    adjust_slide();
    if ($("#matches-shell").width() <= 0)
        $("#map-shell #map").width($(window).width()-41);
    else
        $("#map-shell #map").width($(window).width()-502);
});

$(window).load(function(){
     $(window).scroll(adjust_slide);
     $("#matches-shell").width(460);
     $("#map-shell #map").width($(window).width()-502);
     $("#steps").width($(window).width()-473);
     $("#steps").height($("#recipe-box").height()-12);
     $("#steps").css("line-height", $("#steps").height().toString() + "px");
     adjust_slide();
     reset_functions();
     reset_add_ingredient();

     $("#steps").on("input propertychange", function(){
    $("#steps").height(0);
    $("#steps").height(Math.max($("#steps").prop("scrollHeight") - 10, $("#recipe-box").height()-12));
    adjust_slide();
});

$("#steps").blur(function(){
    $("#steps").val($("#steps").val().trim());
    if ($("#steps").val() == "")
    {
        $("#steps").val("add instruction");
        $("#steps").css("text-align", "center");
        $("#steps").css("color", "#AAAAAA");
        $("#steps").css("line-height", $("#steps").height().toString() + "px");
    }
});

$("#steps").hover(function(){
    $("#steps").css("color", "#000000");
}, function(){
    if ($("#steps").val() == "add instruction")
        $("#steps").css("color", "#AAAAAA");
    else
        $("#steps").css("color", "#000000");
});

$("#steps").focus(function(){
    if ($("#steps").val() == "add instruction")
    {
        $("#steps").val("");
        $("#steps").css("text-align", "left");
        $("#steps").css("color", "#000000");
        $("#steps").css("line-height", "normal");
    }
});

    $("#slide0").click(function() {
    if ($("#matches-shell").width() <= 0)
    {
        $("#matches-shell").animate({width: "460px", borderRightWidth: "1px"}, 200);
        $("#map-shell #map").animate({width: ($(window).width()-502).toString() + "px"}, 200);
        $("#slide0").html("&lt;");
    }
    else
    {
        $("#map-shell #map").animate({width: ($(window).width()-42).toString() + "px"}, 200);
        $("#matches-shell").animate({width: "0px", borderRightWidth: "0px"}, 200);
        $("#slide0").html("&gt;");
    }
});

    $("#save").mouseenter(function(){
    $("#save").html("<img src=\"res/heart-black.png\">");
});

$("#save").mouseleave(function(){
    $("#save").html("<img src=\"res/heart-grey.png\">");
});

$(".dyn-textarea").each(function(){
    var target = this;
    $(target).on("input propertychange", function(){
        $(target).height(0);
        $(target).height($(target).prop("scrollHeight") - 10);
        adjust_slide();
    });
});

});
