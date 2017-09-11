<?php
	include 'connect.php';
	include 'help.php';
	session_start();

	$DoctorID=$_GET['DoctorID'];
	$_SESSION['varname'] = $DoctorID;


	if ($DoctorID==='' ){
			header('Location:Updateable-Delete.html');
			exit;
	}

	$result = mysqli_query($con, "SELECT * FROM `doctor` where (`DoctorID` = '$DoctorID')");
	$not_ex = mysqli_num_rows($result)==0;	

	$result = mysqli_query($con, "SELECT * FROM updateable where (`DoctorID` = '$DoctorID')");
	$no_view = mysqli_num_rows($result)==0;	

	$result1 = mysqli_query($con, "SELECT * FROM `patient` where (`DoctorID` = '$DoctorID')");    
	$ex_patient = mysqli_num_rows($result1)>0;

    if($ex_patient) {
        header('Location: Updateable-Delete-withPatient.html'); 
	}
    else{
       	$result = mysqli_query($con, "DELETE FROM updateable WHERE DoctorID=$DoctorID");
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
		<meta http-equiv="refresh" content=<?php header("Refresh:3; url=Updateable-Delete.html"); ?> />
		<h1 align="center"><strong>

		

		
		<?php
			
			if($not_ex){
				echo '<br><br> There is no doctor with given ID. Please try again! <br><br>';
			}

			if ($no_view) {
				echo '<br><br> Not in view. Please try again! <br><br>';

			}
		?>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html>