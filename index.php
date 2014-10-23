<?php
 error_reporting(E_ALL);
 ini_set('display_errors', 1);
 
 session_start();
 
 if (isset($_SESSION['ing']))
 {
     $execstr = "/var/bin/recipes/query_matches ";
     
     foreach ($_SESSION['ing'] as $key => $value)
         $execstr .= escapeshellarg($value) . " ";
     
     $rec = exec($execstr);
 }
?>

<!DOCTYPE HTML>
<html>
 <head>
  <title>Noms</title>
  <link rel="stylesheet" type="text/css" href="style/recipe.css">
  <script src="//code.jquery.com/jquery-2.1.0.min.js"></script>
  <script src="//code.jquery.com/ui/1.10.4/jquery-ui.min.js"></script> 
  <script src="src/query_recipe.js"></script>
  <script>
   (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
   (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
   m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
   })(window,document,'script','//www.google-analytics.com/analytics.js','ga');
   ga('create', 'UA-12117773-1', 'sol-union.com');
   ga('send', 'pageview');
  </script>
  <script src="src/recipe.js"></script>
 </head>
 
 <body>
  <div class="table" id="recipe">
   <div class="row">
    <div class="cell" id="save">
     <img src="res/heart-grey.png">
    </div>
    
    <div class="cell" id="recipe-box">
     <textarea class="dyn-textarea" id="title">My New Recipe</textarea>
     <ul id="ingredients">
      <?php
       if (isset($_SESSION['ing']))
           foreach ($_SESSION['ing'] as $value)
               printf("<li class=\"ingredient\"><textarea class=\"dyn-textarea amount\" id=\"%d\" name=\"%d\">some</textarea><div class=\"name\" onclick=\"recipe.remove(%d)\">%s</div></li>", $value, $value, $value, exec("/var/bin/recipes/get_name " . escapeshellarg($value)));
      ?>
      <li>
       <form id="add-ingredient-form">
        <textarea class="dyn-textarea" id="add-ingredient" autocomplete="off">add ingredient</textarea>
        <ul id="suggestions">
         <?php
          if (isset($_SESSION['search_ing']) && $_SESSION['search_ing'] != "")
              print exec("/var/bin/recipes/suggest '" . escapeshellarg($_SESSION['search_ing']) . "'");
         ?>
        </ul>
       </form>
      </li>
     </ul>
    </div> 

    <div class="cell" id="steps-shell">
     <textarea id="steps" autocomplete="off">add instruction</textarea>
    </div>
   </div>
  </div>

  <div id="matches-shell">
   <ul id="matches">
    <?php
     if (isset($rec))
         print $rec;
    ?>
   </ul>
  </div>
  <div class="slide-shell">
   <div class="table slide">
    <div class="cell slide-text" id="slide0">
     &lt;
    </div>
   </div>
  </div>
  <div id="map-shell">
   <div id="map">
    <?php
     if (!isset($_SESSION['ing']))
         $_SESSION['ing'] = array();
     $execstr = "/var/bin/recipes/query_map ";
     foreach ($_SESSION['ing'] as $value)
         $execstr .= escapeshellarg($value) . " ";
     print exec($execstr);
    ?>
   </div>
  </div>
 </body>
</html>
