<?php
//edw diagrafoume ta sumvolaia kai ta farmaka pou sundeontai me thn PharmaceuticalCompany


  include 'connect.php';
  session_start();

  $redirect="PharmaceuticalCompany-Delete.html"; 
  $Id = $_SESSION['varname'];

  $query="DELETE FROM contract WHERE PharmaceuticalCompanyId=$Id "; //diagrafw to sumvolaio
  $result2= mysqli_query($link,$query);
  if(!$result2)
         echo "other error, can't delete this record   1";

  $query="DELETE FROM sell WHERE DrugId IN ( SELECT DrugId FROM drug WHERE PharmaceuticalCompanyId=$Id  )  "; //diagrafw to sell
  $result2= mysqli_query($link,$query);
  if(!$result2)
         echo "other error, can't delete this record   2";


  $query="DELETE FROM prescription WHERE DrugId IN ( SELECT DrugId FROM drug WHERE PharmaceuticalCompanyId=$Id  )  "; //diagrafw to prescription
  $result2= mysqli_query($link,$query);
  if(!$result2)
         echo "other error, can't delete this record   3";  

  $query="DELETE FROM drug WHERE PharmaceuticalCompanyId=$Id "; //diagrafw to farmako
  $result2= mysqli_query($link,$query);
  if(!$result2)
         echo "other error, can't delete this record 4";

  $query="DELETE FROM pharmaceuticalcompany WHERE PharmaceuticalCompanyId=$Id "; //diagrafw to farmako
  $result2= mysqli_query($link,$query);
  if(!$result2)
         echo "other error, can't delete this record 5";




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