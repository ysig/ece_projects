<?php
function eq_app($v1,$v2){
	if(is_numeric($v2)){
		return $v1 . '=' . $v2;	
	}
	else{
		return $v1 . '=' . "'" . $v2 . "'";
	}
}
?>