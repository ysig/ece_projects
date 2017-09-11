	<?php	
		include 'connect.php';
		include 'help.php';

		#define vars
		$pkargs = array($_GET['DrugId']);
		$args = array($_GET['Name'],$_GET['Formula'],$_GET['PharmaceuticalCompanyId']);
		$pkfields = array('`DrugId`');
		$fields = array('`Name`','`Formula`','`PharmaceuticalCompanyId`');
		$redirect = array('index.html','Drug-Update.html');
		$table = '`drug`';
		$fk_str = '`PharmaceuticalCompanyId`';
		$fk_table = '`pharmaceuticalcompany`';
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
		#echo $result;
		$non_ex = mysqli_num_rows($result)==0;		
		$wfk_str = "$fields[2]=$args[2]";
		$sql = "SELECT $fk_str FROM $fk_table WHERE $wfk_str";
		$result = mysqli_query($con,$sql);
		$fk_non_ex = mysqli_num_rows($result)==0;		
		
		if(!$non_ex && !$fk_non_ex){
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
		<meta http-equiv="refresh" content=<?php header("Refresh:3; url=$redirect[1]"); ?> />
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
				$fstr = $fields[2];
				$fkstr = $args[2];
				echo '<br>' . $fstr . ' &rarr; ' . $fkstr . ' is non existent!<br><br>';
			}
		?>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html>

