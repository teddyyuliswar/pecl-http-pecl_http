--TEST--
etags with hash
--SKIPIF--
<?php 
include "skipif.inc"; 
_ext("hash"); 
version_compare(PHP_VERSION, "5.4.0", ">=") or die("skip PHP>=5.4 required");
?>
--FILE--
<?php
$body = new http\Message\Body;
$body->append("Hello, my old fellow.");
foreach (hash_algos() as $algo) {
    switch ($algo) {
    case "gost-crypto":
    case "fnv1a32":
    case "fnv1a64":
        continue 2;
    }
    ini_set("http.etag.mode", $algo);
    printf("%10s: %s\n", 
        $algo, 
        $body->etag()
    );
}
?>
DONE
--EXPECT--
       md2: 9bf7d8506d7453a85dc34fa730cbc16a
       md4: 137008b9144843f5bfcc6651688acc41
       md5: 6ce3cc8f3861fb7fd0d77739f11cd29c
      sha1: ad84012eabe27a61762a97138d9d2623f4f1a7a9
    sha224: 91be9bd30cec7fb7fb0279e40211fa71f8a7ab933f9f1a832d7c60cb
    sha256: ed9ecfe5c76d51179c3c1065916fdb8d94aee05577f187bd763cdc962bba1f42
    sha384: 923a756152da113db192958da485c7881e7c4811d2d34e22f4d74cd45310d983f7fb1c5527a5f9037a4c7b649a6cc2b2
    sha512: ad5ea693b8df4457d08d835ad5ccf7b626b66285f8424b3ec59e54c63bf63feef9a92baaba71c38d7bd9a1135488499fc835a8818390965c9ce8a5e4c40e519f
 ripemd128: b9e8d5864b5821d72e66101a9a0e730a
 ripemd160: d697a33676aece781b72f6fcb95f4c730367706b
 ripemd256: 9c3a73ab03e6d7d3471cf70316c4ff3ec56212d25730d382fb1480346529742b
 ripemd320: 5a6ee6b7c35c64d9c91019b9a1ceb2ab2ae19915f3dc96b0f244e15581d750a775a3682c5e70ee23
 whirlpool: 2cb738084edaede8b36e9c8d81f5d30d9afe12bf60715073a6651c32c3448a6eeeff9f9715a8c996291ab3cd6c9a9caac8bea3b0eeb1c88afe6ad46fdd0cef83
tiger128,3: f3055bdb40b06abac716a27a654b295d
tiger160,3: f3055bdb40b06abac716a27a654b295dc07e1ab9
tiger192,3: f3055bdb40b06abac716a27a654b295dc07e1ab915b56529
tiger128,4: e6a1628a4da8fa6adf4ca866c5e235b5
tiger160,4: e6a1628a4da8fa6adf4ca866c5e235b51939bb61
tiger192,4: e6a1628a4da8fa6adf4ca866c5e235b51939bb61ecf8423f
    snefru: 8f50c66c8f0a1510f9c591a2b7a070853d4770c60a38394c8857918dd91a2e5b
 snefru256: 8f50c66c8f0a1510f9c591a2b7a070853d4770c60a38394c8857918dd91a2e5b
      gost: efc79cdd01331adf80b30af816ff7a934f3f3df3085294a310918cacff3500f0
   adler32: 4ff5075d
     crc32: 757b06f7
    crc32b: e56655c5
    fnv132: ebd1fa1f
    fnv164: 9790ce01eba3ae9f
     joaat: 70a407c9
haval128,3: 68a1bee33d2a4fa5543be7fa871f84ea
haval160,3: b4204e8c4f3c993385d997539afa723888700bbd
haval192,3: 6c7f3442f5b5c7d338bd31ab9de1216576ce6633f8de9e03
haval224,3: 4edf7debeea48b94af73f47c1a4449dff516b69ba36f6659ed59689c
haval256,3: eb919a27c9e598cf3559e79fca10119d54b6f704b779cd665ab5352eb17726c4
haval128,4: 184195034f2e5b2a0d04dcc42fac3275
haval160,4: b13d521378d7b74b226430355fa6f4ceba0782c2
haval192,4: 4e53f767e7dbff4abb8ebf767d672db3df77de7d9de6e9d9
haval224,4: 1208cc9fc1c23de3985f5a5214ebb67c846cecd32f96d950ef3ef770
haval256,4: 658d40b21f87ebe45cf6ec822402d1ca6965f263358e3927a92beba837785735
haval128,5: 938933eefe94e217d73a27909f89f8c6
haval160,5: 07b9e4a6c451acb5930081f414a06d948c1b70ba
haval192,5: 997ca1515369b0051e9fcc736c1096618ef936f185a19ebe
haval224,5: b46f2aada87d9e7a38b126268dce9779303aa4999d42f5c74427e362
haval256,5: 4e0b601e5ee93d6c2a449793e756e9ca6e03fb618c9f2ed849a7f8ca29ef9112
DONE
