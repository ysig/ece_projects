	<?php	
		include 'connect.php';
		include 'help.php';

		#define vars
		$pkargs = array($_GET['PharmaceuticalCompanyId']);
		$args = array($_GET['Name'],$_GET['PhoneNumber']);
		$pkfields = array('`PharmaceuticalCompanyId`');
		$fields = array('Name','PhoneNumber');
		$table = '`pharmaceuticalcompany`';
		$redirect = array('index.html','PharmaceuticalCompany-Update.html');
		
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
		if(mysqli_num_rows($result)==0){
   				$non_ex = true;		
		}
		else{
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
				echo '<br> <br>' . $fstr . ' &rarr; ' . $pkstr . ' is non existent!<br><br><br>';
			}
		?>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html>

