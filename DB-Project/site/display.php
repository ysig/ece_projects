<?php
function display_data($data) {
    $output = "<table>";
    foreach($data as $key => $var) {
        if($key===0) {
            $output .= '<tr>';
            foreach($var as $col => $val) {
               if(!is_numeric($col)){ 
                   $output .= "<td>" . $col . '</td>';
               }
            }
            $output .= '</tr>';
            foreach($var as $col => $val) {
                if(!is_numeric($col)){
                    $output .= '<td>' . $val . '</td>';
                }
            }
            $output .= '</tr>';
        }
        else {
            $output .= '<tr>';
            foreach($var as $col => $val) {
                if(!is_numeric($col)){
                    $output .= '<td>' . $val . '</td>';
                }
            }
            $output .= '</tr>';
        }
    }
    $output .= '</table>';
    echo $output;
}
?>