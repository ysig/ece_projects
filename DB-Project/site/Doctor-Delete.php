<?php

  include 'connect.php';
  session_start();


  //define variables
   $Id=$_POST["DoctorId"];
   $query= "SELECT * FROM doctor WHERE DoctorId=$Id "  ; 
   $result= mysqli_query($link,$query);
   $redirect="Doctor-Delete.html"; 
   $_SESSION['varname'] = $Id;

  //search into db for that doctor 
   if($result){
      if ($row = mysqli_fetch_row($result)){ // an uparxei giatros me tetoio ID

         $query= "SELECT patientId FROM patient WHERE DoctorId=$Id ";
         $result1= mysqli_query($link,$query);
         if($result1){
                if($full= mysqli_fetch_row($result1)){ // an uparxoun astheneis pou sundeontai me giatro , edw logika den prepei na rwthsw an uparxoun suntages giati tha uparxoun mono an uparxoun astheneis???????????????????????????/
                     header('Location: Doctor-Delete-WithPatient.html'); 
              #    echo "There are patients that are connected to Doctor with Id=$Id. Pressing submit you will also delete all of them." ;

                 }
                else{ //an den uparxoun diegrapse ton
                   $query="DELETE FROM doctor WHERE DoctorId=$Id ";
                   $result2= mysqli_query($link,$query);
                   if(!$result2)
                       echo "other error, can't delete this record";
                } 
         }
         else
           echo "DB ERROR1!";
      }
      else
          header('Location: Doctor-Delete-WrongId.html');
   } 
   else
      echo "DB ERROR2!";        
       
  mysqli_close($link);    
    
 ?>

<html">
<head>
   <link type="text/css" rel="stylesheet" href="stylesheet.css"/>
	<title>Redirecting</title>
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