<?php
// registerglobals workaround
if (! get_cfg_var ( "register_globals" )) {
	extract ( $_REQUEST, EXTR_SKIP );
}
include "./inc/pref.php";
header ( "Content-type:text/html; charset=utf-8" );
if (! (isset ( $cfg_loaded ))) {
	echo "ботва! немогу загрузить конфиг!";
}
;
if ($_COOKIE ['lang'] == 0) {
	include "inc/ru.php";
}
;
if ($_COOKIE ['lang'] == 1) {
	include "inc/en.php";
}
;

echo "<HTML><HEAD>
<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=utf-8\">
<META HTTP-EQUIV=\"Pragma\" CONTENT=\"no-cache\">
<META HTTP-EQUIV=\"Cache-Control\" CONTENT=\"no-cache\">
<TITLE>$origin</TITLE></HEAD><BODY BGCOLOR=#ffffff><center>
<p align=right><a href=\"http://www.mysql.com\" alt=\"MySQL database engine\"><img src=\"mysql.png\" alt=\"MySQL database engine\" border=0></a></p>
<p align=right><a href=\"http://www.security.perm.ru\" alt=\"Админ не дремлет...\"><img src=\"openbsd_pb.gif\" alt=\"Админ не дремлет...\" border=0></a></p>
<p align=right><a href=\"http://www.php.net\" alt=\"PHP scripting language\"><img src=\"php_logo.gif\" alt=\"PHP scripting language\" border=0></a></p>
<br><br><br>
<a href=\"stat/\">$web_client_enry_user.</a>
<br><br><br>
<a href=\"admin/\" style=\"color: #FFFFFF;cursor: default\">$web_client_entry_admin</a>
</center><br><br><br><br><br><br><center>
<a href=\"http://www.apache.org\" alt=\"Apache http server\"><img src=\"apache_pb.gif\" alt=\"Apache http server\" border=0></a><br>
<A HREF=\"mailto:$admin_mail?Subject=main SAcc www page\">SAcc administrator</A></SPAN></center></body></html>";
?>