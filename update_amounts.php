<?php

session_start();

if (isset($_SESSION['ing']))
{
	$query = "/var/bin/recipes/update_amounts ";

	$query .= escapeshellarg($_GET['selected']) . " ";
	foreach ($_SESSION['ing'] as $value)
		$query .= escapeshellarg($value) . " ";

	print exec($query);
}

?>
