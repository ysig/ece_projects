	<?php	
		include 'connect.php';
		include 'help.php';

		$PatientId=$_GET['PatientId'];
		$DoctorId=$_GET['DoctorId'];
		$DrugId=$_GET['DrugId'];
		$Date=$_GET['Date'];
		$Quantity=$_GET['Quantity'];
		

		if ($PatientId==='' or $DoctorId==='' or $DrugId==='' or $Date==='' or $Quantity===''){
			header('Location:Prescription-Insert.html');
			exit;
		}

		$result = mysqli_query($con, "SELECT * FROM `patient` where (`PatientId` = '$PatientId')");
		$no_patient = mysqli_num_rows($result)==0;

		$result = mysqli_query($con, "SELECT * FROM `doctor` where (`DoctorId` = '$DoctorId')");
		$no_doctor = mysqli_num_rows($result)==0;

		$result = mysqli_query($con, "SELECT * FROM `drug` where (`DrugId` = '$DrugId')");
		$no_drug = mysqli_num_rows($result)==0;
	
		$result = mysqli_query($con, "SELECT * FROM `prescription` where (`DoctorId` = '$DoctorId') and (`PatientId` = '$PatientId')");
		$ex = mysqli_num_rows($result)>0;

		
		if (!$no_drug && !$no_doctor && !$no_patient && !$ex) {
			$result = mysqli_query($con, "INSERT INTO `prescription` (`PatientId`, `DoctorId`, `DrugId`, `Date`, `Quantity`)
									VALUES ($PatientId, $DoctorId, $DrugId, '$Date', $Quantity)");
			if($result === FALSE) {
				die(mysqli_error($con)); 
			}
		}

		mysqli_close($con);
	?>	

<html >
<head>
    <link type="text/css" rel="stylesheet" href="stylesheet.css"/>
	<title>Redirecting</title>
</head>
<body>
		<meta http-equiv="refresh" content=<?php header("Refresh:3; url=Prescription-Insert.html"); ?> />
		<h1 align="center"><strong>
		
		<?php
			
			if($no_doctor) {
				echo '<br><br> There does not exist a doctor with given ID. Please try again! <br><br>';
			}
			if($no_patient) {
				echo '<br><br> There does not exist a patient with given ID. Please try again! <br><br>';
			}
			if($no_drug) {
				echo '<br><br> There does not exist a drug with given ID. Please try again! <br><br>';
			}

			if ($ex) {
				echo '<br><br> There already exists a prescription for given combination of Doctor-Patient. Please try again! <br><br>';
			}
		?>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html>