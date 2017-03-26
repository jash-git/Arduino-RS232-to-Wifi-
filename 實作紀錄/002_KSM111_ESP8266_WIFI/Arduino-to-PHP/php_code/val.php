<?php
	$val = $_GET['string'];
	$bf = fopen("test.txt","w+");
	fwrite ($bf,$val);
	fclose($bf);
?>