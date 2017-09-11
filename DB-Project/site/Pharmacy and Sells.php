<?php // Ta farmakeia se fthinousa seira kerdous. periexei(order, join, group)
  include 'connect.php';




   $query=" SELECT  S.PharmacyId,P.Name, SUM(S.Price) as price
           FROM sell S JOIN Pharmacy P ON S.PharmacyId=P.PharmacyId
           GROUP BY  S.PharmacyId
           ORDER BY  price DESC,S.PharmacyId "  ; 

//

 /* $query=" SELECT  S.PharmacyId,P.Name, SUM(S.Price) as price
           FROM sell S, Pharmacy P
           WHERE S.PharmacyId=P.PharmacyId
           GROUP BY  S.PharmacyId
           ORDER BY  price DESC,S.PharmacyId "  ; 

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
                <h2 align="center"> Farmacies ordered by sells</h2>
		
	<?php


                   echo '<table width=30%><thead><tr>';
                   $j=0; 
                   
                         echo '<th >'."Farmacy Id".'</th>' ;
                         echo '<th >'."Farmacy Name".'</th>' ;
                         echo '<th >'."Price".'</th>' ;



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
                               echo '<td >';
                               echo($arr[$j][2]);
                               echo "  ";
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



