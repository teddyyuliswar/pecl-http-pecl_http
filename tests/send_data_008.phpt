--TEST--
http_send_data() HTTP_SENDBUF_SIZE long string
--SKIPIF--
<?php 
include 'skip.inc';
?>
--FILE--
<?php
http_send_data(str_repeat('0', 2097152));
?>
--EXPECT--
Content-type: text/html
X-Powered-By: PHP/5.0.2
Accept-Ranges: bytes
