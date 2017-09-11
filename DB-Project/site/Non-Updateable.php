<?php // o mesos oros ths timhs kathe farmakou
  include 'connect.php';

 $query1=" SELECT *
           FROM avgprice";

  
  $result1= mysqli_query($link,$query1);

  $arr=array();
  $i=0; 


  if($result1){
           while ($row = mysqli_fetch_row($result1)){
                  $arr[$i]=$row;     
                  $i=$i+1; 
           }

  }
  else  
    echo "DBERROR";


 mysqli_close($link);    
    
?>

<html>
<head>
	<title>Average Price of drug</title>
  <link type="text/css" rel="stylesheet" href="stylesheet.css"/>
</head>
<body>
  <header> 
    <h1> HealthCare </h1> 

  <ul>
      <li><a href="index.html">Home</a></li>
      <li><a href="Insert.html">Insert</a></li>
      <li><a href="Update.html">Update</a></li>
      <li><a href="Delete.html">Delete</a></li>
      <li><a href="DB_Info.html">DB Info</a></li>
      <li><a class="active" href="Views.html">Views</a></li>
      <li><a href="Queries.html">Queries</a></li>
  </ul>
  </header>
                    
               <h1 class="title1"><strong>Average price for every drug</strong></h1>
  <div class="q">             
		
	<?php


                   echo '<table><thead><tr>';
                   $j=0; 
                   
                         echo '<th >'."Drug Id".'</th>' ;
                         echo '<th >'."Drug Name".'</th>' ;
                         echo '<th >'."Drug Average Price".'</th> ';



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

	</div>
  <footer>Copyright &copy; DB 2017 </footer>
	
</body>
</html>



