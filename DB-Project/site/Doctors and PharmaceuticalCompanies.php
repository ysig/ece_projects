<?php // oi giatroi kai oi etairies twn opoiwn ta farmaka suntagografoun (join)
  include 'connect.php';


 $query="SELECT DISTINCT D.FirstName,D.LastName, C.Name
           FROM doctor D, prescription P, drug Dr, pharmaceuticalcompany C 
           WHERE  D.DoctorId=P.DoctorId AND P.DrugId=Dr.DrugId AND Dr.PharmaceuticalCompanyId=C.PharmaceuticalCompanyId"  ; 


// Ta duo $query einai isodunama to deutero den xerw gt den trexei

/*  $query="SELECT DISTINCT D.FirstName,D.LastName, C.Name // me to sistinct apofeugw tis diplotupes eggrafes
           FROM (doctor D JOIN prescription P ON D.DoctorId=P.DoctorId) JOIN (drug Dr JOIN pharmaceuticalcompany C ON Dr.PharmaceuticalCompanyId=C.PharmaceuticalCompanyId) ON P.DrugId=Dr.DrugId"  ; 
*/
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
</head>
<body>
                    
		<h1 align="center" style="color:darkred;font-size=200px;font-family:Lucida Handwriting;"><strong>
              	<link type="text/css" rel="stylesheet" href="tables.css"/>
                <h2 align="center"> Doctors and Pharmaceutical Companies </h2>
		
	<?php


                   echo '<table width=50%><thead><tr>';
                   $j=0; 
                   
                         echo '<th >'."Doctor's Name".'</th>' ;
                         echo '<th >'."Doctor's Last Name".'</th>' ;
                         echo '<th >'."Pharmaceutical Company".'</th> ';



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



