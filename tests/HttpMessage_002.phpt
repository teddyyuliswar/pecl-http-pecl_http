--TEST--
HttpMessage properties
--SKIPIF--
<?php
include 'skip.inc';
checkmin(5);
checkcls('HttpMessage');
?>
--FILE--
<?php
class Message extends HttpMessage
{
	public function test()
	{
		print_r($this->headers);
		$this->headers['Foo'] = 'Bar';
	}
}

error_reporting(E_ALL|E_STRICT);

echo "-TEST\n";
$m = new Message;
$m->test();
echo "Done\n";
?>
--EXPECTF--
%sTEST
Array
(
)
%sFatal error%sCannot access HttpMessage properties by reference or array key/index in%s
