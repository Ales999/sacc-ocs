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
//Считываем текущее время 
$mtime = microtime ();
//Разделяем секунды и миллисекунды 
$mtime = explode ( " ", $mtime );
//Составляем одно число из секунд и миллисекунд 
$mtime = $mtime [1] + $mtime [0];
//Записываем стартовое время в переменную 
$tstart = $mtime;
unset ( $mtime );
?>