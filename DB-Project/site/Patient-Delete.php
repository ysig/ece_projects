<?php

  include 'connect.php';
  session_start();


  //define variables
   $Id=$_POST["PatientId"];
   $query= "SELECT * FROM patient WHERE PatientId=$Id "  ; 
   $result= mysqli_query($link,$query);
   $redirect="Patient-Delete.html"; 
   $_SESSION['varname'] = $Id;

  //search into db for that patient
   if($result){
      if ($row = mysqli_fetch_row($result)){ // an uparxei asthenhs me tetoio ID

         $query= "SELECT * FROM prescription WHERE PatientId=$Id ";
         $result1= mysqli_query($link,$query);
         if($result1){
                if($full= mysqli_fetch_row($result1)){ //an uparxoun suntages pou sundeontai me asthenh
                     header('Location: Patient-Delete-WithPres.html'); 
                 }
                else{ //an den uparxoun diegrapse ton
                   $query="DELETE FROM patient WHERE PatientId=$Id ";
                   $result2= mysqli_query($link,$query);
                   if(!$result2)
                       echo "other error, can't delete this record";
                } 
         }
         else
           echo "DB ERROR1!";
      }
      else
          header('Location: Patient-Delete-WrongId.html');
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
                               echo "Record with id: $Id was succesfully deleted";  
                    }
                    else{


                    }
			
		?><br><br>
		Redirecting you in 3 seconds...</strong>
		</h1>
</body>
</html>