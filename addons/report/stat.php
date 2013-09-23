#!/usr/local/bin/php -q
<?php
// -----------------------------------------------------------------------------
// Все копирайты принадлежат Virus-у и некоторая часть CS-y.
// Сделано с особым цинизмом.
// Last change: SAcc v.154  [27-jul-03]
// + попытка избавится от sh скриптов. и сделать всё на пхп.
// -----------------------------------------------------------------------------
// place into sacc/report
require("../etc/pref.php");
require("../web/htdocs/inc/functions.php");
# ----------------- Аутентификация ---------------------
    $link = mysql_connect("","","");
    $db = mysql_select_db("trf", $link);
if ( $link == NULL ) {
    echo "<H1>Внимание!</H1>
          <P>
            <FONT COLOR=#FF0000>Неудалось подключение к локальной БД MySQL!</FONT>
          </P>\n";
    exit;
}
// вывод заголовков и прочей ботвы.
ob_start();
echo "<HTML>
<HEAD>
<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=utf-8\">
<TITLE>SAcc v. $version: Reporter</TITLE>
<STYLE TYPE=\"text/css\">
<!--
A:link { color:#6300C6 }
A:visited { color:#6300C6 }
A:active  { color:#400080 }
A { text-decoration:none }
P, SPAN, TD { font-size:10 pt }
// -->
</STYLE></HEAD><BODY BGCOLOR=#FFFFFF TEXT=#000000>";
//вывод шапки
echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
      <TR>
        <TD BGCOLOR=#93BEE2><TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1>
         <TR VALIGN=TOP ALIGN=CENTER>
            <TD HEIGHT=30><B>Фамилия И. О. (отдел)</B></TD>
            <TD><B>Трафик,<BR>байт</B></TD>
            <TD><B>Лимит,<BR>байт</B></TD>
            <TD><B>NT login</B></TD>
            <TD><B>Почта,<br>байт</B></TD>
            <TD><B>Статус</B></TD></TR>";
// Выборка всей информации по пользователям из БД squid (таблица traf)
$result = mysql_query("SELECT * FROM traf order by used desc", $link);
for ($i = 0; $i < mysql_numrows($result); $i++) {
    $login = mysql_result( $result, $i, "login");
    $cur = mysql_result( $result, $i, "used");
    $cur = $cur -1; // надо бы избавится от этой единицы нахрен...
    $lim = mysql_result( $result, $i, "quota");
    $email = mysql_result( $result, $i, "email");
    $descr = mysql_result( $result, $i, "descr");
    $timeacl = mysql_result( $result, $i, "timeacl");
//Выводится содержимое таблицы
    echo "<TR BGCOLOR=#FFF7E5>
            <TD>".$descr."</TD>
            <TD ALIGN=RIGHT>". dotize($cur) . "</TD>
            <TD ALIGN=RIGHT>" . dotize($lim) . "</TD>
            <TD>$login</TD>
            <TD align=right>";
//  учёт почты не распространяется =) Может быть, когда нибудь :))
    $rcpt = "rcpt='" . str_replace(",", "' OR rcpt='", $email) . "'";
    $email = str_replace(",", ", ", $email);
    $result2 = mysql_query("SELECT SUM(size) FROM mail WHERE $rcpt", $link);
    $msum = @mysql_result($result2, 0);
    $msum = dotize($msum);
echo "$msum</TD><TD>";
if ($lim > 0) if ($cur>$lim) echo "Отключен";
echo "</TR>";
}
$result = mysql_query("SELECT sum(used) as sused, sum(quota) as squota FROM traf", $link);
$slim = @mysql_result($result, 0, "squota");
$lim = @mysql_result($result, 0, "sused");
// опять емайл ;))
$result = mysql_query("SELECT SUM(size) FROM mail", $link);
$msum = @mysql_result($result, 0);
// Мужик по фамилии "_Итого:"
echo "<TR BGCOLOR=#FFF7E5><TD>Итого на $i рыл</TD>
            <TD ALIGN=RIGHT>". dotize($lim) . "</TD>
            <TD ALIGN=RIGHT>" . dotize($slim) . "</TD>
            <TD></TD>
            <TD align=right>" . dotize($msum) . "</TD>
            <TD align=right><FONT COLOR=#00FF00>";
// тот самый предел в месяц.
if (($msum+$lim) > $month_limit) echo "<FONT COLOR=#FF0000>";
echo "           " . dotize(($msum+$lim)) . "</FONT></TD>
            </TR></TABLE></TD></TR></TABLE>";
$sendst=ob_get_clean();
echo $sendst;
//$hostname=shell_exec("hostname");
//mail($admin_mail, "$hostname ".date("Y-M-d")."SAcc v$version", $sendst, "Content-type: text/html");
ob_flush();
echo "OK";
exit;
?>