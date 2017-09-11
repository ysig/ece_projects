	<?php	
		include 'connect.php';
		include 'help.php';
		
		$Name=$_GET['Name'];
		$PhoneNumber=$_GET['PhoneNumber'];

		if ($Name==='' or $PhoneNumber===''){
			header('Location:PharmaceuticalCompany-Insert.html');
			exit;
		}


		# We assumed there can not exist more than one company with the same name
		$result = mysqli_query($con, "SELECT * FROM `pharmaceuticalcompany` where (`Name` = '$Name')");
		$name_taken = mysqli_num_rows($result)>0;


		if (!$name_taken) {
			$result = mysqli_query($con, "INSERT INTO `pharmaceuticalcompany` (`Name`, `PhoneNumber`) VALUES
('$Name', $PhoneNumber)");		
			if($result === FALSE) { 
				die(mysqli_error($con)); 
			}
		}

		mysqli_close($con);		
	?>	
	
<html>
<head>
	<title>Redirecting</title>
	<link type="text/css" rel="stylesheet" href="stylesheet.css"/>
</head>
<body>
		<meta http-equiv="refresh" content=<?php header("Refresh:3; url=PharmaceuticalCompany-Insert.html"); ?> />
		<h1 align="center"><strong>
		
		<?php
			
			if($name_taken) {
				echo '<br><br> This name already exists in database. Please try again! <br><br>';
			}
		?>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html>