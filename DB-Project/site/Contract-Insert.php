	<?php	
		include 'connect.php';
		include 'help.php';
		
		$PharmacyId=$_GET['PharmacyId'];
		$PharmaceuticalCompanyId=$_GET['PharmaceuticalCompanyId'];
		$StartDate=$_GET['StartDate'];
        $EndDate=$_GET['EndDate'];
        $Text=$_GET['Text'];
		$Supervisor=$_GET['Supervisor'];
		
		if ($PharmacyId==='' or $PharmaceuticalCompanyId==='' or $StartDate==='' or $EndDate==='' or $Text==='' or $Supervisor===''){
			header('Location:Contract-Insert.html');
			exit;
		}

		$dates_wrong = 0;
		if ($StartDate > $EndDate)
			$dates_wrong = 1;

		$result = mysqli_query($con, "SELECT * FROM `pharmacy` where (`PharmacyId` = '$PharmacyId')");
		$no_pharmacy = mysqli_num_rows($result)==0;

		$result = mysqli_query($con, "SELECT * FROM `pharmaceuticalcompany` where (`PharmaceuticalCompanyId` = '$PharmaceuticalCompanyId')");
		$no_company = mysqli_num_rows($result)==0;

		$result = mysqli_query($con, "SELECT * FROM `contract` where (`PharmacyId` = '$PharmacyId') and (`PharmaceuticalCompanyId` = '$PharmaceuticalCompanyId')");
		$ex = mysqli_num_rows($result)>0;


		if (!$no_pharmacy && !$no_company && !$ex && !$dates_wrong) {
			$result = mysqli_query($con, "INSERT INTO `contract` (`PharmacyId`, `PharmaceuticalCompanyId`, `StartDate`, `EndDate`, `Text`, `Supervisor`) VALUES ($PharmacyId, $PharmaceuticalCompanyId, '$StartDate', '$EndDate', '$Text', '$Supervisor')");
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
		<meta http-equiv="refresh" content=<?php header("Refresh:3; url=Contract-Insert.html"); ?> />
		<h1 align="center"><strong>
		
		<?php
			
			if($no_pharmacy) {
				echo '<br><br> There does not exist a pharmacy with given ID. Please try again! <br><br>';
			}
			if($no_company) {
				echo '<br><br> There does not exist a pharmaceutical company with given ID. Please try again! <br><br>';
			}
			if ($ex) {
				echo '<br><br> This pharmacy has already registered a contract for given pharmaceutical company. Please try again! <br><br>';
			}

			if ($dates_wrong) {
				echo '<br><br> Start Date later than End Date. Please try again! <br><br>';
			}

		?>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html>