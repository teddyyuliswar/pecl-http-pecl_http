--TEST--
http_redirect() with params
--SKIPIF--
<?php 
include 'skip.inc';
?>
--FILE--
<?php
http_redirect('redirect', array('a' => 1, 'b' => 2));
?>
--EXPECTF--
Status: 302
Content-type: text/html
X-Powered-By: PHP/%s
Location: http://localhost/redirect?a=1&b=2

Redirecting to <a href="http://localhost/redirect?a=1&b=2">http://localhost/redirect?a=1&b=2</a>.
