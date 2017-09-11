	<?php	
		include 'connect.php';
		include 'help.php';
		
		$Name=$_GET['Name'];
		$Town=$_GET['Town'];
		$StreetName=$_GET['StreetName'];
		$StreetNumber=$_GET['StreetNumber'];
		$PostalCode=$_GET['PostalCode'];
		$PhoneNumber=$_GET['PhoneNumber'];

		if ($Name==='' or $Town==='' or $PhoneNumber==='' or $StreetNumber==='' or $StreetName==='' or $PostalCode===''){
			header('Location:Pharmacy-Insert.html');
			exit;
		}

		# We assume there are no duplicates in the Name field 
		$result = mysqli_query($con, "SELECT * FROM `pharmacy` where (`Name` = '$Name')");
		$name_taken = mysqli_num_rows($result)>0;

		$result = mysqli_query($con, "SELECT * FROM `pharmacy` where (`Town` = '$Town') and(`StreetName` = '$StreetName') and (`StreetNumber` = '$StreetNumber')");
		$place_taken = mysqli_num_rows($result)>0;

		if (!$name_taken && !$place_taken) {
			$result = mysqli_query($con, "INSERT INTO `pharmacy` (`Name`, `Town`, `StreetName`, `StreetNumber`, `PostalCode`, `PhoneNumber`) VALUES
('$Name', '$Town', '$StreetName', $StreetNumber, $PostalCode, $PhoneNumber)");		
			if($result === FALSE) { 
				die(mysqli_error($con)); 
			}
		}

		mysqli_close($con);		
	?>	
	
<html>
<head>
    <link type="text/css" rel="stylesheet" href="stylesheet.css"/>
	<title>Redirecting</title>
</head>
<body>
		<meta http-equiv="refresh" content=<?php header("Refresh:3; url=Pharmacy-Insert.html"); ?> />
		<h1 align="center"><strong>
		
		<?php
			
			if($name_taken) {
				echo '<br><br> This name already exists in database. Please try again! <br><br>';
			}
			else if ($place_taken) {
				echo '<br><br> There already exists a pharmacy in this address. Please try again! <br><br>';
			}
		?>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html>