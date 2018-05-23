<?php

//
// Create URL to movie info from movie's title
//
function get_URL($title)
{
	// create url to movie info
	$url = "http://www.omdbapi.com/?t=" . $title . "&y=&plot=full&r=xml";
	// set url in the correct format
	return strtolower(str_replace(" ","+",$url));
}

?>