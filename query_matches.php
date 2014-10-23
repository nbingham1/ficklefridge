<?php

error_reporting(-1);
ini_set('display_errors', 'On');

session_start();

$query = "/var/bin/recipes/query_matches ";
if (isset($_SESSION['ing']))
	foreach ($_SESSION['ing'] as $id)
		$query .= escapeshellarg($id) . " ";

echo exec($query);

?>
