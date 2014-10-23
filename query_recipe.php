<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);
session_start();



if (!isset($_SESSION['ing']))
{
        $_SESSION['ing'] = array();
}

if (!isset($_SESSION['amt']))
{
        $_SESSION['amt'] = array();
}

if (!isset($_SESSION['rec']))
{
        $_SESSION['rec'] = array();
}

if (!isset($_SESSION['title']))
{
	$_SESSION['title'] = "My New Recipe";
}

if (!isset($_SESSION['steps']))
{
	$_SESSION['steps'] = array();
}

if (isset($_GET['rem']) && gettype($_GET['rem']) == "array")
{
        $_SESSION['ing'] = array_diff($_SESSION['ing'], $_GET['rem']);
}
else if (isset($_GET['rem']))
{
        $_SESSION['ing'] = array_diff($_SESSION['ing'], array($_GET['rem']));
}

if (isset($_GET['add']) && gettype($_GET['add']) == "array")
{
	$_SESSION['ing'] = array_merge($_SESSION['ing'], $_GET['add']);
	sort($_SESSION['ing']);
	$_SESSION['ing'] = array_unique($_SESSION['ing']);
}
else if (isset($_GET['add']))
{
	array_push($_SESSION['ing'], $_GET['add']);
	sort($_SESSION['ing']);
	$_SESSION['ing'] = array_unique($_SESSION['ing']);
}

if (isset($_GET['add-rec']) && gettype($_GET['add-rec']) == "array")
{
	$_SESSION['rec'] = array_merge($_SESSION['rec'], $_GET['add-rec']);
        sort($_SESSION['rec']);
        $_SESSION['rec'] = array_unique($_SESSION['rec']);
}
else if (isset($_GET['add-rec']))
{
	array_push($_SESSION['rec'], $_GET['add-rec']);
        sort($_SESSION['rec']);
        $_SESSION['rec'] = array_unique($_SESSION['rec']);
}

if (isset($_GET['rem-rec']) && gettype($_GET['rem-rec']) == "array")
{
        $_SESSION['rec'] = array_diff($_SESSION['rec'], $_GET['rem-rec']);
}
else if (isset($_GET['rem-rec']))
{
        $_SESSION['rec'] = array_diff($_SESSION['rec'], array($_GET['rem-rec']));
}


foreach ($_SESSION['ing'] as $value)
{
	if (!isset($_SESSION['amt'][$value]))
		$_SESSION['amt'][$value] = "some";
	printf("<li class=\"ingredient\"><textarea class=\"dyn-textarea amount\" id=\"%d\" name=\"%d\">%s</textarea><div class=\"name\" onclick=\"recipe.remove(%d)\">%s</div></li>", $value, $value, $_SESSION['amt'][$value], $value, exec("/var/bin/recipes/get_name " . escapeshellarg($value)));
}

printf("<li><form id=\"add-ingredient-form\"><textarea class=\"dyn-textarea\" id=\"add-ingredient\" autocomplete=\"off\">add ingredient</textarea><ul id=\"suggestions\"></ul></form></li>");
?>
