<?php	
	include 'connect.php';
	include 'help.php';

		$FirstName=$_GET['FirstName'];
		$LastName=$_GET['LastName'];
		$Speciality=$_GET['Speciality'];
		$ExperienceYears=$_GET['ExperienceYears'];

		if ($FirstName==='' or $LastName==='' or $Speciality==='' or $ExperienceYears===''){
			header('Location:Updateable-Insert.html');
			exit;
		}

		$years = 0;
		if ($ExperienceYears <= 15)
			$years = 1;

		$result = mysqli_query($con, "SELECT * FROM `doctor` where (`FirstName` = '$FirstName') and (`LastName` = '$LastName')");
		$ex = mysqli_num_rows($result)>0;	

		
		if (!$ex && !$years) {
			$result = mysqli_query($con, "INSERT INTO updateable (FirstName, LastName, Speciality, ExperienceYears)
									VALUES ('$FirstName', '$LastName', '$Speciality', $ExperienceYears)");
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
		<meta http-equiv="refresh" content=<?php header("Refresh:3; url=Updateable-Insert.html"); ?> />
		<h1 align="center"><strong>
		
		<?php
			
			if($ex){
				echo '<br><br> There exists a doctor with given name. Please try again! <br><br>';
			}

			if ($years) {
				echo '<br><br> Experience Years less than 15. Please try again! <br><br>';

			}
		?>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html>