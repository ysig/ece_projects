<?php
//edw diagrafoume ta sumvolaia kai tis pwlhseis pou sundeontai me to farmako


  include 'connect.php';
  session_start();

  $redirect="Pharmacy-Delete.html"; 
  $Id = $_SESSION['varname'];

  $query="DELETE FROM contract WHERE PharmacyId=$Id "; //diagrafw to sumvolaio
  $result2= mysqli_query($link,$query);
  if(!$result2)
         echo "other error, can't delete this record";

  $query="DELETE FROM sell WHERE PharmacyId=$Id "; //diagrafw thn pwlhsh
  $result2= mysqli_query($link,$query);
  if(!$result2)
         echo "other error, can't delete this record";

  $query="DELETE FROM pharmacy WHERE PharmacyId=$Id ";   //diagrafw to farmakeio
  $result2= mysqli_query($link,$query);
  if(!$result2)
         echo "other error, can't delete this record";




  mysqli_close($link);   
?>


<html>
<head>
	<title>Redirecting</title>
  <link type="text/css" rel="stylesheet" href="stylesheet.css"/>
</head>
<body>
		<meta http-equiv="refresh" content=<?php header("Refresh:3; url=$redirect"); ?> />
		<h1 align="center" ><strong>
		
		<?php
                    echo "Records were succesfully deleted";  
			
		?><br><br>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html           