<?php
  include 'connect.php';
  session_start();

  $DoctorID = $_SESSION['varname'];

  $query="DELETE FROM prescription where (`DoctorID` = '$DoctorID')"; 
  $result= mysqli_query($link,$query);
  if(!$result)
         echo "Error";
 

  $query="DELETE FROM patient where (`DoctorID` = '$DoctorID') ";   
  $result2= mysqli_query($link,$query);
  if(!$result)
         echo "Error";



  $query="DELETE FROM doctor where (`DoctorID` = '$DoctorID') ";
  $result= mysqli_query($link,$query);
  if(!$result)
          echo "Error";


  mysqli_close($link);   
?>



<html>
<head>
   <link type="text/css" rel="stylesheet" href="stylesheet.css"/>
	<title>Redirecting</title>
</head>
<body>
		<meta http-equiv="refresh" content=<?php header("Refresh:3; url=Updateable-Delete.html"); ?> />
		<h1 align="center"><strong>

		
		<?php
                    echo "Records were succesfully deleted";  
			
		?><br><br>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html           