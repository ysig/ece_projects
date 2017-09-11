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
		include 'sql_queries.php';
		include 'display.php';

		if(isset($_GET["query"])){
			$dd = $_GET["query"];	
		}else{
			$dd = 0;
		}
		
		if($dd>=1 && $dd<=$nq){
			$result = mysqli_query($con, $query[$dd-1]);
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
			    <li><a href="DB_Info.html">DB Info</a></li>
			    <li><a href="Views.html">Views</a></li>
			    <li><a class="active" href="Queries.html">Queries</a></li>
				</ul>
				</header>
				<div align="center">	
		   	    <h1> <?php echo $query_names[$dd-1];?> </h1> 
	    	<?php
	    	if(empty($data)){
	    		echo "<h2>Nothing..</h2>";
	    	}else{
    			display_data($data);
    		}
    		echo "</div>";
    	}
		else{
			?>
			<meta http-equiv="refresh" content=<?php header("Refresh:2; url=Queries.html"); ?> />
			<h1 align="center"><strong>
			Incorrect Input<br />Redirecting you in 3 seconds...</strong>
			</h1>
			<?php
		}
		mysqli_close($con);
	?>
	<footer>Copyright &copy; DB 2017 </footer>
</body>

</html>