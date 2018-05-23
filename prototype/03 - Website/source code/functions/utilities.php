<?php
/*
 * Includes
 */
include_once '../login/includes/db_connect.php';
include_once '../login/includes/functions.php';



//
// Connect to the db
//
function connect_db($dbname)
{
	$servername = "localhost";
	$username   = "root";
	$password   = "7NbkYDXpkGBbp9";

	// Create connection
	$conn = new mysqli($servername, $username, $password, $dbname);

	// Check connection
	if ($conn->connect_error) {
    	die("Connection failed: " . $conn->connect_error);
	} 

	return $conn;
}

//
// close db connection
//
function disconnect_db($conn)
{
	$conn->close();
}


//
// return the webserver's root path
//
function get_root()
{
	$protocol = $_SERVER['HTTPS'] == '' ? 'http://' : 'https://';
	$folder = $protocol . $_SERVER['HTTP_HOST'];
	return $folder;
}


?>