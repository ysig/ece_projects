<?php //dwse se auxousa timh ola ta farmaka sumfwna me thn elaxisth timh sthn opoia pwlountai, kai ena farmakeio sto opoio exoun thn elaxisth timh
  include 'connect.php';


   $query="SELECT D.Name,MIN(S.Price),P.Name
           FROM  pharmacy P, drug D, sell S 
           WHERE  P.PharmacyId=S.PharmacyId AND D.DrugId=S.DrugId
           GROUP BY D.Name
           ORDER BY S.Price ";


 
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
                <h2 align="center"> Every drug ordered by price</h2>
		
	<?php


                   echo '<table width=50%><thead><tr>';
                   $j=0; 
                   
                         echo '<th >'."Drug Name".'</th>' ;
                         echo '<th >'."Price".'</th>' ;
                         echo '<th >'."Pharmacy".'</th> ';



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



