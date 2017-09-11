<?php

  include 'connect.php';
  session_start();


  //define variables

   $Id1=$_POST["PatientId"];
   $Id2=$_POST["DoctorId"];
   $Id3=$_POST["DrugId"];

   $query= "SELECT * FROM prescription WHERE (PatientId=$Id1 AND DoctorId=$Id2 AND DrugId=$Id3 )"  ; 
   $result= mysqli_query($link,$query);
   $redirect="Prescription-Delete.html"; 
//   $_SESSION['varname'] = $Id;

  //search into db for that doctor 
   if($result){
      if ($row = mysqli_fetch_row($result)){ // an uparxei suntagh me tetoio ID
                   $query="DELETE FROM prescription WHERE PatientId=$Id1 AND DoctorId=$Id2 AND DrugId=$Id3";
                   $result2= mysqli_query($link,$query);
                   if(!$result2)
                       echo "other error, can't delete this record";
                 
      }
      else
          header('Location: Prescription-Delete-WrongId.html');
   } 
   else
      echo "DB ERROR2!";        
       
  mysqli_close($link);    
    
 ?>

<html>
<head>
	<title>Redirecting</title>
  <link type="text/css" rel="stylesheet" href="stylesheet.css"/>
</head>
<body>
                <meta http-equiv="refresh" content=<?php header("Refresh:3; url=$redirect"); ?> />   
		<h1 align="center"><strong>
		
		<?php
                   if($result2){// an den uparxoun astheneis tou giatrou diegrapse ton
                               echo "Record with id: $Id1, $Id2, $Id3 was succesfully deleted";  
                    }
                    else{


                    }
			
		?><br><br>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html>