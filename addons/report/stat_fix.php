#!/usr/local/bin/php -q
<?php
// -----------------------------------------------------------------------------
// Все копирайты принадлежат Virus-у
// Сделано с особым цинизмом.
// created: SAcc v.154  [27-jul-03]
// + попытка избавится от sh скриптов. и сделать всё на пхп.
// -----------------------------------------------------------------------------
//$debug=1;
// Changelog:
// * Фиксим content/type (c) Virus 28-jul
//
$fd = fopen ("/tmp/sacc_r.uue".$mode, "r");
while (!feof ($fd)) {
//    $i=1;
    $buffer = fgets($fd);
//Content-Type: application/octet-stream; name="stat.html"
//Content-Type: multipart/mixed;
    $buffer=str_replace("multipart/mixed;","text/html;charset=\"utf-8\";",$buffer);
//    $buffer=str_replace("\r\n","",$buffer);
//    $buffer=str_replace("  ","",$buffer);
//    $buffer=str_replace("\n","",$buffer);
    echo $buffer;
}
fclose ($fd);
?>