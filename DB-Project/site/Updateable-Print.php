<!DOCTYPE html>
<html>

<head>

	<title>Print</title>
	
	<link type="text/css" rel="stylesheet" href="stylesheet.css"/>

</head>

<body>
	<header> 
	  <h1> HealthCare </h1> 

	<ul>
      <li><a href="index.html">Home</a></li>
      <li><a href="Insert.html">Insert</a></li>
      <li><a href="Update.html">Update</a></li>
      <li><a href="Delete.html">Delete</a></li>
      <li><a href="DB_Info.html">DB Info</a></li>
      <li><a class="active" href="Views.html">Views</a></li>
      <li><a href="Queries.html">Queries</a></li>
	</ul>
	</header>
	<h1 align="center"><strong>Print</strong></h1>
	<?php
		include 'connect.php';
		include 'help.php';
		
		$result=mysqli_query($con,"SELECT * FROM updateable");
		if($result === FALSE) { 
			die(mysqli_error($con));
		}
		
		echo "<table align='center' >";
		echo "<tr>";
		echo "<td>";echo "<strong>";echo "First Name";echo "</strong>";echo "</td>"; 
		echo "<td>";echo "<strong>";echo "Last Name";echo "</strong>";echo "</td>"; 
		echo "<td>";echo "<strong>";echo "Doctor ID";echo "</strong>";echo "</td>"; 
		echo "<td>";echo "<strong>";echo "Speciality";echo "</strong>";echo "</td>"; 
		echo "<td>";echo "<strong>";echo "Experience Years";echo "</strong>";echo "</td>"; 
		echo "</tr>";
		
			while($row = mysqli_fetch_array($result)) {
				echo "<tr>";
				echo "<td>";
				echo $row['FirstName'];
				echo "</td>";
				echo "<td>";
				echo $row['LastName'];
				echo "</td>";
				echo "<td>";
				echo $row['DoctorId'];
				echo "</td>";
				echo "<td>";
				echo $row['Speciality'];
				echo "</td>";	
				echo "<td>";
				echo $row['ExperienceYears'];
				echo "</td>";	
				echo "</tr>";
			}
		echo "</table>";
		mysqli_close($con);
		
	?>
	<footer>Copyright &copy; DB 2017 </footer>


</body>

</html>