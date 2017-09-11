<?php
//edw diagrafoume tis suntages kai tous astheneis pou sundeontai me to giatro


  include 'connect.php';
  session_start();

  $redirect="Patient-Delete.html"; 
  $Id = $_SESSION['varname'];

  $query="DELETE FROM prescription WHERE PatientId=$Id "; //diagrafw th suntagh
  $result2= mysqli_query($link,$query);
  if(!$result2)
         echo "other error, can't delete this record";
 

  $query="DELETE FROM patient WHERE PatientId=$Id ";   //diagrafw ton asthenh  prepei na tsekarw an yparxoun syntages pou apeuthinontai se astheneis pou den uperxoun???? den ennoeitai?????????????? mhpws na mhn valoume to doctor Id ston patient????????
  $result2= mysqli_query($link,$query);
  if(!$result2)
         echo "other error, can't delete this record";




  mysqli_close($link);   
?>


<html >
<head>
 <link type="text/css" rel="stylesheet" href="stylesheet.css"/>
	<title>Redirecting</title>
</head>
<body>
		<meta http-equiv="refresh" content=<?php header("Refresh:3; url=$redirect"); ?> />
		<h1 align="center"><strong>
		
		<?php
                    echo "Records were succesfully deleted";  
			
		?><br><br>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html           