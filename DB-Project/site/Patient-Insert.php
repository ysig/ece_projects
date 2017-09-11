	<?php	

		include 'connect.php';
		include 'help.php';
		
		$FirstName=$_GET['FirstName'];
		$LastName=$_GET['LastName'];
		$Town=$_GET['Town'];
		$StreetName=$_GET['StreetName'];
		$Number=$_GET['Number'];
		$PostalCode=$_GET['PostalCode'];
		$Age=$_GET['Age'];
		$DoctorId=$_GET['DoctorId'];

		if ($FirstName==='' or $LastName==='' or $Town==='' or $StreetName==='' or $Number==='' or $PostalCode==='' or $Age==='' or $DoctorId===''){
			header('Location:Patient-Insert.html');
			exit;
		}

		# SELECT TOP 1 FROM

		$result = mysqli_query($con, "SELECT * FROM `doctor` where (`DoctorId` = '$DoctorId')");
		$did_non_ex = mysqli_num_rows($result)==0;	

		
		# in case we do not allow 2 people with the same name
		#$result = mysqli_query($con, "SELECT * FROM `patient` where (`FirstName` = '$FirstName') and (`LastName` = '$LastName')");
		#$name_taken = mysqli_num_rows($result)>0;

		if (!$did_non_ex) {
			$result = mysqli_query($con, "INSERT INTO `patient` (`FirstName`, `LastName`, `Town`, `StreetName`, `Number`, `PostalCode`, `Age`, `DoctorId`)
									VALUES ('$FirstName', '$LastName', '$Town', '$StreetName', $Number, $PostalCode, $Age, $DoctorId)");
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
		<meta http-equiv="refresh" content=<?php header("Refresh:3; url=Patient-Insert.html"); ?> />
		<h1 align="center"><strong>
		
		<?php
			
			if($did_non_ex){
				echo '<br><br> There is no doctor with given id. Please try again! <br><br>';
			}
			#if($name_taken) {
			#	echo '<br><br> This name already exists in database. Please try again! <br><br>';
			#}
		?>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html>