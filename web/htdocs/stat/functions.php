<?php

//
//      ������� show_help() - ���������� ������� ������� �� ���������
//      ���� : ������
//      �����: ������ true;
//
function show_help() {
	global $version, $web_client_welcome, $web_client_heap_unauth;
	echo "<P><H1>Dear user</H1><P>
    $web_client_welcome
 $version</A>.
</P><P>$web_client_heap_unauth";
	return true;
}
?>