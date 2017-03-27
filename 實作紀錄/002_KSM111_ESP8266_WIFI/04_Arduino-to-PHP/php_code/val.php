<?php
	$val = $_GET['string'];
	$bf = fopen("test.txt","a+");
	fwrite ($bf,$val."\r\n");
	fclose($bf);
	echo "php get data\r\n";
?>