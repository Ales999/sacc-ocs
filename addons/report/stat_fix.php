#!/usr/local/bin/php -q
<?php
// -----------------------------------------------------------------------------
// ��� ��������� ����������� Virus-�
// ������� � ������ ��������.
// created: SAcc v.154  [27-jul-03]
// + ������� ��������� �� sh ��������. � ������� �ӣ �� ���.
// -----------------------------------------------------------------------------
//$debug=1;
// Changelog:
// * ������ content/type (c) Virus 28-jul
//
$fd = fopen ("/tmp/sacc_r.uue".$mode, "r");
while (!feof ($fd)) {
//    $i=1;
    $buffer = fgets($fd);
//Content-Type: application/octet-stream; name="stat.html"
//Content-Type: multipart/mixed;
    $buffer=str_replace("multipart/mixed;","text/html;charset=\"koi8-r\";",$buffer);
//    $buffer=str_replace("\r\n","",$buffer);
//    $buffer=str_replace("  ","",$buffer);
//    $buffer=str_replace("\n","",$buffer);
    echo $buffer;
}
fclose ($fd);
?>