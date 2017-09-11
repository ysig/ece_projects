	<?php	
		include 'connect.php';
		include 'help.php';
		
		$Name=$_GET['Name'];
		$Formula=$_GET['Formula'];
		$PharmaceuticalCompanyId=$_GET['PharmaceuticalCompanyId'];

		if ($Name==='' or $Formula==='' or $PharmaceuticalCompanyId===''){
			header('Location:Drug-Insert.html');
			exit;
		}

		$result = mysqli_query($con, "SELECT * FROM `pharmaceuticalcompany` where (`PharmaceuticalCompanyId` = '$PharmaceuticalCompanyId')");
		$company_non_ex = mysqli_num_rows($result)==0;		

		$result = mysqli_query($con, "SELECT * FROM `drug` where (`Name` = '$Name')");
		$name_taken = mysqli_num_rows($result)>0;

		if (!$name_taken && !$company_non_ex) {
				$result = mysqli_query($con, "INSERT INTO `drug` (`Name`, `Formula`, `PharmaceuticalCompanyId`) VALUES ('$Name', '$Formula', $PharmaceuticalCompanyId)");		
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
		<meta http-equiv="refresh" content=<?php header("Refresh:3; url=Drug-Insert.html"); ?> />
		<h1 align="center"><strong>
		
		<?php
			
			if($company_non_ex){
				echo '<br><br> There is no pharmaceutical company with given id. <br><br>';
			}
			if($name_taken) {
				echo '<br><br> This name already exists in database. Please try again! <br><br>';
			}
		?>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html>