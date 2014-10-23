<?php

echo exec("/var/bin/recipes/suggest " . escapeshellarg($_GET['ing']));

?>
