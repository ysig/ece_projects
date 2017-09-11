<?php

  include 'connect.php';
  session_start();


  //define variables
   $Id=$_POST["DrugId"];
   $query= "SELECT * FROM drug WHERE DrugId=$Id "  ; 
   $result= mysqli_query($link,$query);
   $redirect="Drug-Delete.html"; 
   $_SESSION['varname'] = $Id;

  //search into db for that doctor 
   if($result){
      if ($row = mysqli_fetch_row($result)){ // an uparxei suntagh me tetoio ID

         $query= "SELECT * FROM prescription WHERE DrugId=$Id ";
         $result1= mysqli_query($link,$query);
         $query1= "SELECT * FROM sell WHERE DrugId=$Id "  ;
         $res1= mysqli_query($link,$query1);
         if($result1 || $res1){
                if($full= mysqli_fetch_row($result1) || $full1= mysqli_fetch_row($res1)){ 
                     header('Location: Drug-Delete-WithPres.html'); 
              #    echo "There are patients that are connected to Doctor with Id=$Id. Pressing submit you will also delete all of them." ;

                 }
                else{ //an den uparxoun diegrapse ton
                   $query="DELETE FROM drug WHERE DrugId=$Id ";
                   $result2= mysqli_query($link,$query);
                   if(!$result2)
                       echo "other error, can't delete this record";
                } 
         }
         else
           echo "DB ERROR1!";
      }
      else
          header('Location: Drug-Delete-WrongId.html');
   } 
   else
      echo "DB ERROR2!";        
       
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