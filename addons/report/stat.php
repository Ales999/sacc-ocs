#!/usr/local/bin/php -q
<?php
// -----------------------------------------------------------------------------
// ��� ��������� ����������� Virus-� � ��������� ����� CS-y.
// ������� � ������ ��������.
// Last change: SAcc v.154  [27-jul-03]
// + ������� ��������� �� sh ��������. � ������� �ӣ �� ���.
// -----------------------------------------------------------------------------
// place into sacc/report
require("../etc/pref.php");
require("../web/htdocs/inc/functions.php");
# ----------------- �������������� ---------------------
    $link = mysql_connect("","","");
    $db = mysql_select_db("trf", $link);
if ( $link == NULL ) {
    echo "<H1>��������!</H1>
          <P>
            <FONT COLOR=#FF0000>��������� ����������� � ��������� �� MySQL!</FONT>
          </P>\n";
    exit;
}
// ����� ���������� � ������ �����.
ob_start();
echo "<HTML>
<HEAD>
<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=koi8-r\">
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
//����� �����
echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
      <TR>
        <TD BGCOLOR=#93BEE2><TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1>
         <TR VALIGN=TOP ALIGN=CENTER>
            <TD HEIGHT=30><B>������� �. �. (�����)</B></TD>
            <TD><B>������,<BR>����</B></TD>
            <TD><B>�����,<BR>����</B></TD>
            <TD><B>NT login</B></TD>
            <TD><B>�����,<br>����</B></TD>
            <TD><B>������</B></TD></TR>";
// ������� ���� ���������� �� ������������� �� �� squid (������� traf)
$result = mysql_query("SELECT * FROM traf order by used desc", $link);
for ($i = 0; $i < mysql_numrows($result); $i++) {
    $login = mysql_result( $result, $i, "login");
    $cur = mysql_result( $result, $i, "used");
    $cur = $cur -1; // ���� �� ��������� �� ���� ������� ������...
    $lim = mysql_result( $result, $i, "quota");
    $email = mysql_result( $result, $i, "email");
    $descr = mysql_result( $result, $i, "descr");
    $timeacl = mysql_result( $result, $i, "timeacl");
//��������� ���������� �������
    echo "<TR BGCOLOR=#FFF7E5>
            <TD>".$descr."</TD>
            <TD ALIGN=RIGHT>". dotize($cur) . "</TD>
            <TD ALIGN=RIGHT>" . dotize($lim) . "</TD>
            <TD>$login</TD>
            <TD align=right>";
//  �ޣ� ����� �� ���������������� =) ����� ����, ����� ������ :))
    $rcpt = "rcpt='" . str_replace(",", "' OR rcpt='", $email) . "'";
    $email = str_replace(",", ", ", $email);
    $result2 = mysql_query("SELECT SUM(size) FROM mail WHERE $rcpt", $link);
    $msum = @mysql_result($result2, 0);
    $msum = dotize($msum);
echo "$msum</TD><TD>";
if ($lim > 0) if ($cur>$lim) echo "��������";
echo "</TR>";
}
$result = mysql_query("SELECT sum(used) as sused, sum(quota) as squota FROM traf", $link);
$slim = @mysql_result($result, 0, "squota");
$lim = @mysql_result($result, 0, "sused");
// ����� ����� ;))
$result = mysql_query("SELECT SUM(size) FROM mail", $link);
$msum = @mysql_result($result, 0);
// ����� �� ������� "_�����:"
echo "<TR BGCOLOR=#FFF7E5><TD>����� �� $i ���</TD>
            <TD ALIGN=RIGHT>". dotize($lim) . "</TD>
            <TD ALIGN=RIGHT>" . dotize($slim) . "</TD>
            <TD></TD>
            <TD align=right>" . dotize($msum) . "</TD>
            <TD align=right><FONT COLOR=#00FF00>";
// ��� ����� ������ � �����.
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