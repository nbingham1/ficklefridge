var highlight = -1;
var ivalue = "";

$("#add").keydown(function(e)
        {
                if (e.which == 38 || e.keyCode == 38)
                {
                        if (ivalue == "")
                        {
                                ivalue = $("#add").val();
                        }
                        $("#suggest" + highlight).css("background-color", "#FFFFFF");
                        highlight--;
                        if (highlight <= -1)
                        {
                                $("#add").val(ivalue);
                                ivalue = "";
                                highlight = -1;
                        }
                        else
                        {
                                $("#add").val($("#suggest" + highlight).html());
                                $("#suggest" + highlight).css("background-color", "#ADDFFF");
                        }
                }
                else if (e.which == 40 || e.keyCode == 40)
                {
                        if (ivalue == "")
                        {
                                ivalue = $("#add").val();
                        }
                        $("#suggest" + highlight).css("background-color", "#FFFFFF");
                        highlight++;
                        if (highlight > 9)
                        {
                                highlight = 9;
                        }
                        $("#add").val($("#suggest" + highlight).html());
                        $("#suggest" + highlight).css("background-color", "#ADDFFF");
                }
		else if (e.which != 13 && e.keyCode != 13)
                {
                        ivalue = "";
                        highlight = -1;
                        addstring = $("#add").val();
                        if (e.which == 83 || e.keyCode == 83 || e.which == 8 || e.keyCode == 8 || e.which == 46 || e.keyCode == 46)
                                addstring = addstring.substr(0, addstring.length-1);
                        else
                                addstring += String.fromCharCode(e.which || e.keyCode);
                        $.ajax(
                                {url: ("/recipes/suggest.php?ing=" + addstring.toLowerCase())}
                        ).done(
                                function(ing)
                                {
                                        $("#suggestions").html(ing);
                                        if (ing != "")
                                        {
                                                $("#suggestions").css("border-style", "solid");
                                        }
                                        else
                                        {
                                                $("#suggestions").css("border-style", "none");
                                        }
                                }
                        );
                }
	});

$("#addform").submit(function()
	{
		ivalue = "";
		highlight = -1;
		if ($("#add").val() != "")
		{
			$.ajax(
				{url: ("/recipes/get_id.php?ing=" + $("#add").val().toLowerCase())}
                	).done(
                		function(id)
                       		{
					if (id != "")
					{
                        			recipe.add(id, function() {window.location.href = "/recipes/recipe.php";});
					}
                        	});
		}
		return true;
	});
