	<?php	
		include 'connect.php';
		include 'help.php';

		$PharmacyId=$_GET['PharmacyId'];
		$DrugId=$_GET['DrugId'];
		$Price=$_GET['Price'];

		
		if ($PharmacyId==='' or $DrugId==='' or $Price===''){
			header('Location:Sell-Insert.html');
			exit;
		}

		$result = mysqli_query($con, "SELECT * FROM `pharmacy` where (`PharmacyId` = '$PharmacyId')");
		$no_pharmacy = mysqli_num_rows($result)==0;

		$result = mysqli_query($con, "SELECT * FROM `drug` where (`DrugId` = '$DrugId')");
		$no_drug = mysqli_num_rows($result)==0;

		$result = mysqli_query($con, "SELECT * FROM `sell` where (`DrugId` = '$DrugId') and (`PharmacyId` = '$PharmacyId')");
		$ex = mysqli_num_rows($result)>0;

		if (!$no_drug && !$no_pharmacy && !$ex) {
			$result = mysqli_query($con, "INSERT INTO `sell` (`PharmacyId`, `DrugId`, `Price`)
									VALUES ($PharmacyId, $DrugId, $Price)");
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
		<meta http-equiv="refresh" content=<?php header("Refresh:3; url=Sell-Insert.html"); ?> />
		<h1 align="center"><strong>
		
		<?php
			
			if($no_drug) {
				echo '<br><br> There does not exist a drug with given ID. Please try again! <br><br>';
			}
			if($no_pharmacy) {
				echo '<br><br> There does not exist a pharmacy with given ID. Please try again! <br><br>';
			}
			if ($ex) {
				echo '<br><br> This pharmacy has already registered a price for given drug. Please try again! <br><br>';
			}
		?>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html>