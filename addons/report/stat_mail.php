#!/usr/local/bin/php -q
<?php
// -----------------------------------------------------------------------------
// ��� ��������� ����������� Virus-� � ��������� ����� CS-y.
// ������� � ������ ��������.
// created: SAcc v.154  [27-jul-03]
// + ������� ��������� �� sh ��������. � ������� �ӣ �� ���.
// -----------------------------------------------------------------------------
//$debug=1;
//
//
//
//
//

require("../etc/pref.php");
$result=shell_exec("hostname");
if (!isset($cfg_loaded)) {echo "�����! ������ ��������� ������!";die;};
//echo "to: $admin_mail\n";
$exec_str="/usr/local/sacc/report/stat.php | uuencode stat.html >/tmp/sacc_r.uue&&./stat_fix.php | mail -s \"".date("Y-M-d")." $result Sacc v $version\" $admin_mail";
//"Content-Type: application/octet-stream; name=\"stat.html\""
//"Content-Type: text/html;charset=\"koi8-r\";name=\"stat.html\""
//&&rm /tmp/stat_r.uue"
//echo "$exec_str\n";
shell_exec($exec_str);
?>