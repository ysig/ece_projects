<?php

$servername = "localhost";
$username = "root";
$password = "";
$db ="health_system";

$link = mysqli_connect($servername,$username,$password,$db);

/* check connection */
if (mysqli_connect_errno()) {
    printf("Connect failed: %s\n", mysqli_connect_error());
    exit();
}
$con=$link;
?>