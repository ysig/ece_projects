<?php
session_start();
?>
<!DOCTYPE html>

<html>

<head>

	<title>Info</title>
	
	<link type="text/css" rel="stylesheet" href="stylesheet.css"/>

</head>

<body>

	<?php
		// create a connection to the database 
		include 'connect.php';
		include 'display.php';
		if(isset($_POST['table'])){
			$dd = $_POST['table'];
			$_SESSION['dd'] = $dd;
		}else if(!isset($_SESSION['dd'])){
			?>
			<meta http-equiv="refresh" content=<?php header("Refresh:2; url=DB_Info.html"); ?> />
			<h1 align="center"><strong>Uknown Request<br />Redirecting you in 3 seconds...</strong></h1>
			</body>
			</html>
			<?php
			session_destroy();
			exit(1);
		}else{
			$dd = $_SESSION['dd'];
		}
		$result = mysqli_query($con, "SELECT * FROM $dd");
		if (mysqli_connect_errno())
		{
			die('Could not connect: ' . mysqli_error($con));
		}
		$data = array();
	    while($row = mysqli_fetch_array($result)){
	    	array_push($data,$row);
	    }
	    ?>
	    <header> 
	    	<h1> HealthCare </h1> 
			<ul>
		    	<li><a href="index.html">Home</a></li>
		    	<li><a href="Insert.html">Insert</a></li>
		    	<li><a href="Update.html">Update</a></li>
		    	<li><a href="Delete.html">Delete</a></li>
		    	<li><a class="active" href="DB_Info.html">DB Info</a></li>
		    	<li><a href="Views.html">Views</a></li>
			    <li><a href="Queries.html">Queries</a></li>
			</ul>
			</header>
		<div align="center">	
		<?php   	  
	    display_data($data);
		mysqli_close($con);	
		?>
	</div align="center">
	
	<footer>Copyright &copy; DB 2017 </footer>
</body>

</html>