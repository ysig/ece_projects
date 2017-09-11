<?php	
		include 'connect.php';
		include 'help.php';
	
		$FirstName=$_GET['FirstName'];
		$LastName=$_GET['LastName'];
		$DoctorID=$_GET['DoctorID'];
		$Speciality=$_GET['Speciality'];
		$ExperienceYears=$_GET['ExperienceYears'];

		if ($FirstName==='' or $LastName==='' or $DoctorID==='' or $Speciality==='' or $ExperienceYears==='' or $ExperienceYears<=15){
			header('Location:Updateable-Update.html');
			exit;
		}
		
		$result = mysqli_query($con, "SELECT * FROM `doctor` where (`DoctorID` = '$DoctorID')");
		$ex = mysqli_num_rows($result)==0;	


		$result = mysqli_query($con, "SELECT * FROM updateable where (`DoctorID` = '$DoctorID')");
		$not_in_view = mysqli_num_rows($result)==0;


	
		if (!$ex && !$not_in_view) {
			$result = mysqli_query($con, "UPDATE updateable SET FirstName='$FirstName', LastName='$LastName', Speciality='$Speciality', ExperienceYears='$ExperienceYears'
									WHERE DoctorID=$DoctorID ");
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
		<meta http-equiv="refresh" content=<?php header("Refresh:3; url=Updateable-Update.html"); ?> />
		<h1 align="center"><strong>
		
		<?php
			
			if($ex){
				echo '<br><br> There is no doctor with given ID. Please try again! <br><br>';
			}

			if ($not_in_view) {
				echo '<br><br> Doctor with given ID can not be processed through the updateable view. Please try again! <br><br>';

			}
		?>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html>