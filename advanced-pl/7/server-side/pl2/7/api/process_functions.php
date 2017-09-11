<?php 
//php 7.0
  function rfbe($var){
	if($var==false){
		return "";
	}else{
		return $var;
	}
  }
  function diagnose($error){
	$message = $error;
	$success = true;
	if($error!=""){
		if(preg_match("/.*[d|D]iv.*zero.*/",$error)){
			$message = "division with zero";
			$success = false;
		}else if(preg_match("/.*[m|M]od.*zero.*/",$error)){
			$message = "modulo with zero";
			$success = false;
		}else if(preg_match("/.*[e|E]rror.*uknown.*inst.*/",$error)){
			$message = "uknown instruction";
			$success = false;
		}else if(preg_match("/.*[w|W]rong.*[i|I]n.*[ext|great].*grid.*size.*/",$error)){
			$message = "program doesn't fit inside grid [80x25]";
			$success = false;
		}else if(preg_match("/.*[i|I]n.*file.*not.*exist.*/",$error)){
			$message = " wrong input file (programmer beware!)";
			$success = false;
		}
		//else if(preg_match("/.*[p|P]ut.*inst.*out.*grid.*/",$error)){
		//	$message = "p outside of grid zero";
		//	$success = false;
		//}
	}
	return Array("success" => $success, "message" => $message);
  }
  //replace false by empty
  function PsExecute($program, $timeout = 1, $sleep = 1000) { 
        // First, execute the process, get the process ID 
		$usleep = $sleep*0.000001;
        $pid = PsExec($program); 
        if( $pid === false ) 
            return Array("Success" => false, "Output" => "", "Error" => "Unable to start process");

        $cur = 0; 
        // Second, loop for $timeout seconds checking if process is running 
        while( $cur < $timeout ) { 
			$t = microtime(TRUE);
            usleep($sleep);
			$t = microtime(TRUE) - $t;
            $cur += $t;
			//+$usleep; 
            // If process is no longer running, return true; 

            //echo "\n ---- $cur ------ \n"; 
			//echo "\n ---- $cur --- Sleeped for $t ------ \n"; 
            
            if( !PsExists($pid) ){ 
				$stdout = rfbe(file_get_contents('stdout.tmp'));
				$stderr = rfbe(file_get_contents('stderr.tmp'));
				exec("rm -f stdout.tmp stderr.tmp temp.in");
				$z = diagnose($stderr);
                return Array("Success" => $z["success"], "Output" => $stdout, "Error" => $z["message"]);
			} // Process must have exited, success! 
        } 
		// If process is still running after timeout, kill the process and return false 
        PsKill($pid); 
		exec("rm -f stdout.tmp stderr.tmp temp.in");
		return Array("Success" => false, "Output" => "", "Error" => "Timeout"); 
    } 

    function PsExec($program, $executable="befunge") { 
        $temp_fn = 'temp.in';
		$program = str_replace(array("\t", "\r"), '', $program);
		//print $program . "\n";
		//print "php_eol = " . $PHP_EOL;
        file_put_contents($temp_fn,$program);
		$execu = './' . $executable . ' ' . $temp_fn . ' > stdout.tmp 2> stderr.tmp';
		$to_pid = ' & echo $!';
		$command = $execu . $to_pid;        
		exec($command,$op);
		//print $command;
        $pid = (int)$op[0]; 
		//print $pid;
        if($pid!="") return $pid; 
        return false; 
    } 

    function PsExists($pid) { 
        exec("ps -A -o pid | grep -c $pid 2>&1", $output);
		//print "count = " . $output[0];
		return ($output[0]>=1); 
    } 

    function PsKill($pid) { 
        exec("kill -9 $pid", $output); 
    } 
?>
