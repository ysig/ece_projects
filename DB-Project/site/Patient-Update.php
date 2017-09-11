	<?php	
		include 'connect.php';
		include 'help.php';

		#define vars
		$pkargs = array($_GET['PatientId']);
		$args = array($_GET['FirstName'],$_GET['LastName'],$_GET['Town'],$_GET['Number'],$_GET['StreetName'],$_GET['PostalCode'],$_GET['Age'],$_GET['DoctorId']);
		$pkfields = array('`PatientId`');
		$fields = array('FirstName','LastName','Town','Number','StreetName','PostalCode','Age','DoctorId');
		$redirect = array('index.html','Patient-Update.html');
		$table = '`patient`';
		$fk_str = '`DoctorId`';
		$fk_table = '`doctor`';
		#check for input correctness
		for($i=0; $i<count($args); $i+=1){
			if($args[$i]==''){
				header('Location: ' . $redirect[1]);
				exit;
			}
		}

		#check that there exists such a doctor ID
		$pk_str = join(', ', $pkfields);
		$wpk_str = join(' AND ',array_map("eq_app",$pkfields, $pkargs));
		$set_str = join(',',array_map("eq_app",$fields, $args));
		$sql = "SELECT $pk_str FROM $table WHERE $wpk_str";
		$result = mysqli_query($con,$sql);
		#pk_check
		$non_ex = mysqli_num_rows($result)==0;
		$wfk_str = "$fields[7]=$args[7]";
		$sql = "SELECT $fk_str FROM $fk_table WHERE $wfk_str";
		$result = mysqli_query($con,$sql);
		#fk_check
		$fk_non_ex = mysqli_num_rows($result)==0;
		if(!$non_ex && !$fk_non_ex){
			$non_ex = false;
			$result = mysqli_query($con, "UPDATE $table SET $set_str WHERE $wpk_str");
			if($result == FALSE) { 
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
		<meta http-equiv="refresh" content=<?php header("Refresh:2; url=$redirect[1]"); ?> />
		<h1 align="center"><strong>
		
		<?php
			if($non_ex){
				$fstr = join(', ', $pkfields);
				$pkstr = join(', ',$pkargs);
				if(count($pkargs)>1){
					$pkargs = '(' . $pkstr . ')';
					$fstr = '(' . $fstr . ')';
				}
				echo '<br>' . $fstr . ' &rarr; ' . $pkstr . ' is non existent!<br><br>';
			}
			if($fk_non_ex){
				$fstr = $fields[7];
				$fkstr = $args[7];
				echo '<br>' . $fstr . ' &rarr; ' . $fkstr . ' is non existent!<br><br>';
			}
		?>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html>

