<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml"><head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Funge game!</title>
<style type="text/css">
<!--
body,td,th {
  font-family: Verdana, Arial, Helvetica, sans-serif;
  font-size: x-large;
  color: #CCCCCC;
}

body {
  background-color: #333399;
}

.title {
  font-family: "Courier New", Courier, monospace;
  font-weight: bold;
  font-size: 48px;
  color: #00FF66;
}

.question {color: #FFCC33}
.number {color: #FFFF33}
.md5sum {color: #FFCCFF}
.emph {color: #99ee99}
.alert {color: #ee77aa}

.right {
  color: #33FF66;
  font-weight: bold;
}

.wrong {
  color: #FF3366;
  font-weight: bold;
}

a:link {
  color: #CCFFFF;
}

a:visited {
  color: #CCFFFF;
}

textarea {
  background-color: #eeee66;
  color: #333399;
}

textarea.wide {
  font-family: monospace;
  font-size: x-large;
  color: #333333;
  border: 1px solid black;
  padding: 8px;
}
-->
<?php
//php 7.0
session_start();
$lifetime = 3600;

if(
  (isset($_SESSION['LAST']) && (time() - $_SESSION['LAST'] > $lifetime))
  || (
        isset($_POST['reset'])
        &&(
            !strcmp($_POST['reset'],"Change number!")
          ||!strcmp($_POST['reset'],"reset")
          )
      )
  ) {
        session_unset();
        session_destroy();
        session_start();
}
$_SESSION['LAST'] = time();

//ini_set("session.gc_maxlifetime", 6);
function num_gen(){
	$r=rand(100000000+1,1000000000-1);
	return $r;
}
function initSession($ses, $life_time = 6) {
    //ini_set("session.gc_maxlifetime", 6);
    session_name($ses);
    $_SESSION["number"] = num_gen();
    $_SESSION['LAST'] = time();
}
function get($what = "number",$ses = "session"){
  $ses .= $_SERVER['REMOTE_ADDR'];
	if (isset($_SESSION[$what])){
    //print "Hi from isset session";
	  $_SESSION['LAST'] = time(); //refresh session
		return $_SESSION[$what];
	}else{
    //print "Hi from else:isset session";
		initSession($ses);
		return $_SESSION[$what];
	}
}
function calculate_area($program){
  $r = array('lines' => 0, 'cols' => 0);
  $grid = explode("\n", $program);
  $r['lines'] = count($grid);
  $strl = array_map('strlen',$grid);
  $r['cols'] = max($strl);
  return $r;
}
function validate_program($program, $val){
  $fr = array('Your program is not legal', 'Your program outputs'); //fail reasons
  //$api_url = 'http://courses.softlab.ntua.gr/pl2/2016b/exercises/befunge93-api/?restrict';
  $api_url = 'http://localhost/pl2/7/api/?restrict';
  $r = array('Correct' => true, 'fail' => array('Fail_reason' => $fr[0], 'Specific' => 'timeout'), 'success' => array('Program_Area' => '1 x 392 = 392','Timestamp' => '3081.991 seconds'));

  $data = trim($program, "\r\n") . "\r\n";
  //print $data;
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $api_url);
  curl_setopt($ch, CURLOPT_POST, true);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($ch, CURLOPT_HTTPHEADER, array("Content-Type: text/plain")); 
  curl_setopt($ch, CURLOPT_POSTFIELDS, $data);
  $time = microtime();
  $output = curl_exec($ch);
  $time = microtime() - $time;
  $response = curl_getinfo($ch, CURLINFO_HTTP_CODE);
  curl_close($ch);
  if($response == "200"){
    //echo "Response 200\n";
    //echo "Message = " . $output . "\n";
    if((int)$val==(int)$output){
      $area = calculate_area($program);
      $r['Correct'] = true;
      $r['success']['Program_Area'] = $area['lines'] . ' x ' . $area['cols'] . ' = ' . ($area['lines']*$area['cols']);
      $r['success']['Timestamp'] = $time . ' seconds';
    }else{
      $r['Correct'] = false;
      $r['fail']['Fail_reason'] = $fr[1];
      $r['fail']['Specific'] = $output;
    }
  }else if($response == "400"){
    //echo "Response 400\n";
    //echo "Message = " . $output . "\n";
    $r['Correct'] = false;
    $r['fail']['Fail_reason'] = $fr[0];
    $r['fail']['Specific'] = $output;
  }
  /*else{
    echo "Other Response: ";
    print $response;
  }*/
  //echo "Response :" . $response . "\n";
  //echo "Output :" . $output . "\n";
  return $r;
}
?>
</style>
</head>
<body>
<h1 class="title">Help!</h1>
<!-- <h2><?php print num_gen();?> </h2> -->
<!-- <?php print "<h2>"; print $_SERVER['REMOTE_ADDR']; print "</h2>";?> -->
<p>I need to find a <a href="http://catseye.tc/view/befunge-93/doc/Befunge-93.markdown">Befunge-93</a> program that outputs the number <span class="question"><?php $val = get(); print $val;?></span>.</p>
<p>But I also need the program's total area to be as small as possible.<br>
(Don't worry, it doesn't have to be optimal...)</p>
<p>Oh, one more thing: The commands
  <code class="emph">0-9</code>,
  <code class="emph">?</code>,
  <code class="emph">"</code>,
  <code class="emph">p</code>,
  <code class="emph">g</code>,
  <code class="emph">&amp;</code>, and
  <code class="emph">~</code>
  cannot be used.</p>
<?php
if(isset($_POST['program']) && isset($_POST['submit']) && (!strcmp($_POST['submit'],"Submit!")))
{
  $v = validate_program($_POST['program'],$val);
  if($v['Correct']){
    $suc = $v['success'];
  ?>
    <p class="right">Right!  :-)</p>
  <?php
    session_unset();
    session_destroy();
    session_start();
    echo '<p>Program area: ' . $suc['Program_Area'] . '.</p>';
    echo '<p>It took you ' . $suc['Timestamp'] . '.</p>';
  ?>
    <form action="/pl2/7/funge.php" id="r" name="r" method="post">
    <input id="reset" name="reset" value="reset" type="hidden">
    <input name="again" id="again" value="Play again!" type="submit">
  <?php
  }else{
    $fail = $v['fail'];
  ?>
    <p class="wrong">Wrong!  :-(</p>
  <?php
      echo '<p>' . $fail['Fail_reason'] . ':</p>';
      echo '<pre>' . $fail['Specific'] . '</pre>';
  ?>
    <form action="/pl2/7/funge.php" id="r" name="r" method="post">
    <input name="again" id="again" value="Try again!" type="submit">
    </form>
  <?php
  }
}else{
?>
<p>Enter your program that will print this number!</p>
<form action="/pl2/7/funge.php" id="f" name="f" method="post">
  <textarea name="program" id="program" class="wide" rows="10" cols="80"></textarea>
  <br>
  <input name="submit" id="submit" value="Submit!" type="submit">
  <input name="reset" id="reset" value="Change number!" type="submit">
</form>
<?php
}
?>
</body></html>
