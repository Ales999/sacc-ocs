<?php
global $tstart;
if (isset ( $_COOKIE ['lang'] )) {
	$lang = $_COOKIE ['lang'];
} else {
	$lang = $settings ["language"];
}
;
if ($_COOKIE ['lang'] == 0) {
	include "ru.php";
}
;
if ($_COOKIE ['lang'] == 1) {
	include "en.php";
}
;
//��������� ������� ����� 
$mtime = microtime ();
//��������� ������� � ������������ 
$mtime = explode ( " ", $mtime );
//���������� ���� ����� �� ������ � ����������� 
$mtime = $mtime [1] + $mtime [0];
//���������� ��������� ����� � ���������� 
$tstart = $mtime;
unset ( $mtime );
?>