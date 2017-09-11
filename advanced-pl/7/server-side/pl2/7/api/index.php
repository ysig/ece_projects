<?php
//php 7.0
	include "process_functions.php";
	function execute($program, $timeout=1){
		return PsExecute($program,$timeout);
	}
	if($input = file_get_contents("php://input")){
		if(isset($_GET['restrict'])){
			if(preg_match("/^.*[0-9]|[?\"pg&~].*$/", $input)){
				echo "Program contains illegal commands";
				http_response_code(400);
				exit;
			}
		}
		$r = execute($input);
		if($r["Success"]){
			echo $r["Output"];
			http_response_code(200);
		}else{
			echo $r["Error"];
			http_response_code(400);
		}
		exit;
	}else{
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Befunge93 API</title>
<style type="text/css">
<!--
body,td,th {
  font-family: Verdana, Arial, Helvetica, sans-serif;
  font-size: x-large;
  color: #CCCCCC;
}

h1,h2 {
  font-family: "Courier New", Courier, monospace;
  font-weight: bold;
  color: #00FF66;
}

h1 {
  font-size: 48px;
}

h2 {
  font-size: 36px;
}

body {
  background-color: #333399;
}

.emph {color: #99ee99}

a:link {
  color: #CCFFFF;
}

a:visited {
  color: #CCFFFF;
}

pre {
  font-family: monospace;
  font-size: x-large;
  background-color: #eeee66;
  color: #333333;
  border: 1px solid black;
  padding: 8px;
  overflow-x: auto;
}
-->
</style>
</head>
<body>
<h1>Befunge93 API</h1>
<p>This URL provides an API for interpreting <a href="http://catseye.tc/view/befunge-93/doc/Befunge-93.markdown">Befunge-93</a> programs.</p>
<p>For obvious reasons, program execution is aborted after <span class="emph">1 second</span>.</p>
<p>The API is provided <span class="emph">as-is</span> to the students of NTUA/ECE's course in <a href="http://courses.softlab.ntua.gr/pl2/">Advanced Programming Languages</a>, and to anyone else who cares to use it for non-commercial purpose.</p>

<h2>How to use the API</h2>
<p>In brief, just <span class="emph">POST</span> to this URL, passing 
the program as raw data.  If the program is valid and execution does not
 timeout, the output will be returned as plain text.</p>

<h2>Usage with curl</h2>
<p>From your command-line prompt:</p>
<pre>cat &gt; hello.bf &lt;&lt; EOF
25*"!dlrow olleH"&gt;:#,_@
EOF

cat hello.bf | curl -H "Content-Type: text/plain" --data-binary @- \
  'http://courses.softlab.ntua.gr/pl2/2016b/exercises/befunge93-api/'</pre>
This produces a response code of <span class="emph">200 (OK)</span> with the following content (program output):
<pre>Hello world!
</pre>
<p>Try this, if you fancy:</p>
<pre>45*65*65*"2"30p20p10p::00p2*40p4*5vv&lt;
&gt;60p140g-&gt;:::*00g50g*60g40g-:*-\-v0&gt;1
^_@#`\g0&lt;|`\g04:+1, &lt;*84$$_v#`\0:&lt;&gt;p^
&gt;v&gt;g2+:5^$&gt;g:*++*/7g^&gt;*:9$#&lt;"~"/:"~"v
g:^06,+55&lt;^03*&lt;v09p07%"~"p09/"~"p08%&lt;
^&gt;#0 *#12#&lt;0g:^&gt;+::"~~"90g*80g+*70gv|
g-10g*+:9**00gv|!*`\2\`-20::/2-\/\+&lt;&gt;
%#&amp;eo*!:..^g05&lt;&gt;$030g-*9/\20g*+60g40^
</pre>

<h2>Restricting the source language</h2>
<p>When the modifier <span class="emph">?restrict</span> is used, the following commands are disallowed:
  <code class="emph">0-9</code>,
  <code class="emph">?</code>,
  <code class="emph">"</code>,
  <code class="emph">p</code>,
  <code class="emph">g</code>,
  <code class="emph">&amp;</code>, and
  <code class="emph">~</code>.</p>

<p>Try to use this with the "hello world" example:</p>
<pre>cat hello.bf | curl -H "Content-Type: text/plain" --data-binary @- \
  'http://courses.softlab.ntua.gr/pl2/2016b/exercises/befunge93-api/?restrict'</pre>
Now, it produces a response code of <span class="emph">400 (Bad Request)</span> with the following content (error message):
<pre>Program contains illegal commands
</pre>

<h2>Usage from a PHP script</h2>
<p>Here's how I suggest you use this API from a PHP script, but you're really on your own here...</p>
<pre>$data = trim($program, "\r\n") . "\r\n";

$ch = curl_init();
curl_setopt($ch, CURLOPT_URL, 'http://courses.softlab.ntua.gr/pl2/2016b/exercises/befunge93-api/?restrict');
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_HTTPHEADER, array("Content-Type: text/plain")); 
curl_setopt($ch, CURLOPT_POSTFIELDS, $data);
$output = curl_exec($ch);
$response = curl_getinfo($ch, CURLINFO_HTTP_CODE);
curl_close($ch);
</pre>
<address>
  Enjoy, <span class="emph">-- yanie</span>
</address>

</body></html>
<?php
}
?>
