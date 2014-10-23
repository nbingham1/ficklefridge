var recipe = {};

recipe.add = function(id, callback)
{
	$.ajax(
        	{url: ("/query_recipe.php?add[]=" + id.toString())}
        ).done(
                function(rec)
                {
        	        $("#ingredients").html(rec);
                        adjust_slide();
                        reset_add_ingredient();

                        $.ajax(
                		{url: ("/query_map.php")}
        		).done(
                		function(ing)
                		{
                		        $("#map").html(ing);
                		        if ($("#matches-shell").width() <= 0)
						$("#map-shell #map").width($(window).width()-41);
					else
						$("#map-shell #map").width($(window).width()-502);
                                }
        		);
        		$.ajax(
                                {url: "/query_matches.php"}
                        ).done(
                                function(rec)
                                {
					$("#matches").html(rec);
                                	reset_functions();
					$("#steps").width($(window).width()-473);
					if ($("#matches-shell").width() <= 0)
						$("#map-shell #map").width($(window).width()-41);
					else
						$("#map-shell #map").width($(window).width()-502);
				}
                        );

			if (typeof(callback) == "function")
                        	callback();
                }
        );
};

recipe.remove = function(id, callback)
{
        $.ajax(
        	{url: ("/query_recipe.php?rem[]=" + id.toString())}
        ).done(
                function(rec)
                {
                        $("#ingredients").html(rec);
                        adjust_slide();
			reset_add_ingredient();
                        $.ajax(
                		{url: "/query_map.php"}
        		).done(
                		function(ing)
                		{
                                        $("#map").html(ing);
                			if ($("#matches-shell").width() <= 0)
						$("#map-shell #map").width($(window).width()-41);
					else
						$("#map-shell #map").width($(window).width()-502);
				}
        		);
        		$.ajax(
                                {url: "/query_matches.php"}
                        ).done(
                                function(rec)
                                {
                                        $("#matches").html(rec);
					if ($("#matches-shell").width() <= 0)
						$("#map-shell #map").width($(window).width()-41);
					else
						$("#map-shell #map").width($(window).width()-502);
				}
                        );

			if (typeof(callback) == "function")
                        	callback();
        	}
        );
};
