<?php // Ta farmakeia pou ta sumvolaia tous lhgoun mesa ston marth kai o epopths tou sumvolaiou (Join kai LIKE)

  include 'connect.php';


  $query="SELECT P.Name, C.EndDate, C.Supervisor
           FROM pharmacy P JOIN contract C ON P.PharmacyId=C.PharmacyId 
           WHERE  C.EndDate LIKE '2017-03-%' "  ; 
  $result= mysqli_query($link,$query);

  $arr=array();
  $i=0; 


  if($result){
           while ($row = mysqli_fetch_row($result)){
                  $arr[$i]=$row;     
                  $i=$i+1; 
           }

  }
  else  
    echo "DBERROR";


 mysqli_close($link);    
    
?>

<html style="background-color:#f8f0e2;">
<head>
	<title>Farmacy and Contracts</title>
  <style>
<style>
body {
    background-image: url("how-to-become-a-pharmacy-technician.jpg");
}
</style>
</head>
<body>
                    
		<h1 align="center" style="color:darkred;font-size=200px;font-family:Lucida Handwriting;"><strong>
                <link type="text/css" rel="stylesheet" href="tables.css"/>
                <h2 align="center"> Farmacy and Contracts </h2>
		
	<?php


                   echo '<table width=30%><thead><tr>';
                   $j=0; 
                   
                         echo '<th >'."Farmacy Name".'</th>' ;
                         echo '<th >'."End Date".'</th>' ;
                         echo '<th >'."Supervisor".'</th> ';



                  echo '</tr></thead><tbody>';
                   
 
                    while($j!=$i){
                      
                             echo '<tr>';
                               echo '<td >';
                               echo($arr[$j][0]);
                               echo "  ";
                               echo '</td>';
                               echo ' <td >';
                               echo($arr[$j][1]); 
                               echo "  ";
                               echo '</td>';
                               echo '<td >'  ; 
                               echo($arr[$j][2]);
                               echo '</td>';
                             echo '</tr>'; 
                        $j=$j+1;

               

//                        echo"<br>"; 
                    }  

               
                echo '<tbody></table>';




       		?>


                <br><br><strong>
		</h1>


	<div>
		<h2><strong><a href="Queries.html">Back</a></strong></h2>	
	</div>
</body>
</html>



