<!-- let's show timer -->
<?php
global $tstart, $language, $lang;
//������ ��� �� �� �����, ����� �������� ������� ����� 
$mtime = microtime ();
$mtime = explode ( " ", $mtime );
$mtime = $mtime [1] + $mtime [0];
//���������� ����� ��������� � ������ ���������� 
$tend = $mtime;
//��������� ������� 
$totaltime = ($tend - $tstart);
//������� �� ����� 
echo "<HR NOSHADE COLOR=#000000 SIZE=1><!-- $tstart : $tend -->";
echo "<center><table width=\"800\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr>
    <td width=800 class=\"main\"><p align=center class=\"sys\">
    <font face=arial color=8f8f8f size=1>";
printf ( "Creation time is %f milliseconds. ", $totaltime * 1000 );
//������� �� 217,031002 �����������. 2003-Oct-19 � 02:42:55 by SAcc v. 1.55 web engine
print ( date ( "Y-M-d --- G:i:s T" ) );
echo " by <A HREF=\"mailto:avirus@mail.ru?Subject=SAcc_$version\">SAcc v. $version</a> web engine.";
?>
</font>
<img src=../images/line_825.gif height=5 WIDTH=100>
<a href=http://www.security.perm.ru class=\"body\">home</a>
::
<?php
if ($lang == 1) {
	echo "<a href=../lang.php?lang=0&query=" . base64_encode ( $_SERVER ['REQUEST_URI'] ) . ">�������</a>";
}
;
if ($lang == 0) {
	echo "<a href=../lang.php?lang=1&query=" . base64_encode ( $_SERVER ['REQUEST_URI'] ) . ">English</a>";
}
;
echo " </font></p></td></tr>
</BODY></HTML>";
ob_end_flush ();
@mysql_close ( $link);
?>
