<?php
//edw diagrafoume tis suntages kai tis pwlhseis pou sundeontai me to farmako


  include 'connect.php';
  session_start();

  $redirect="Drug-Delete.html"; 
  $Id = $_SESSION['varname'];

  $query="DELETE FROM prescription WHERE DrugId=$Id "; //diagrafw th suntagh
  $result2= mysqli_query($link,$query);
  if(!$result2)
         echo "other error, can't delete this record";

  $query="DELETE FROM sell WHERE DrugId=$Id "; //diagrafw thn pwlhsh
  $result2= mysqli_query($link,$query);
  if(!$result2)
         echo "other error, can't delete this record";

  $query="DELETE FROM drug WHERE DrugId=$Id ";   //diagrafw ton asthenh  prepei na tsekarw an yparxoun syntages pou apeuthinontai se astheneis pou den uperxoun???? den ennoeitai?????????????? mhpws na mhn valoume to doctor Id ston patient????????
  $result2= mysqli_query($link,$query);
  if(!$result2)
         echo "other error, can't delete this record";




  mysqli_close($link);   
?>


<html>
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