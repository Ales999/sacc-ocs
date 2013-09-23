<?php
$mod_name = "$web_client_header";
global $tstart;
include ("../inc/version.php");
include ("../inc/functions.php");
include ("functions.php");
require_once ("../inc/auth.php");
require ("../inc/mysql.php");

$mode = "user";
$link = db_connect ();
if (! (isset ( $PHP_AUTH_USER ))) {
	Header ( "WWW-Authenticate: Basic realm=\"$web_client_auth_realm\"" );
	Header ( "HTTP/1.0 401 Unauthorized" );
	show_head ();
	show_help ();
	//            echo "$o 1. Введено: $PHP_AUTH_USER : $PHP_AUTH_PW <br>";
	//            echo "2. Введено: $ulogin : $passwd ";
	

	//            show_form();
	show_tail ();
	exit ();
} else //        if ( smb_auth($ulogin, $passwd) )
if (auth_cli ( $PHP_AUTH_USER, $PHP_AUTH_PW, $link )) 

//        if (true)
{
	// ------------ Просмотр запросов пользователя (событие "detail") ------------
	if (($type == "detail") and ($detailed == 1)) {
		show_head ();
		echo "<A HREF=\"index.php#$id\">&lt;&lt;</A>\n<BR>\n";
		$REMOTE_USER=mysql_real_escape_string($REMOTE_USER);
		$result = mysql_query ( "SELECT id FROM users where login='$REMOTE_USER'", $link );
		$id = mysql_result ( $result, 0, "id" );
		$result = mysql_query ( "SELECT name, value FROM options;", $link );
		for($i = 0; $i < mysql_numrows ( $result ); $i ++) {
			$name = mysql_result ( $result, $i, "name" );
			$value = mysql_result ( $result, $i, "value" );
			$settings [$name] = $value;
		}
		show_info ( $link, $id );
		echo "<HR NOSHADE COLOR=#000000 SIZE=1>\n<BR>\n";
		show_detail ( $link, $id, $_GET ['site'] );
		show_tail ();
		@mysql_close ( $link );
		exit ();
	} // ------------ Просмотр обычной статистики ----------------------------------
else //if (!(isset($type))) 
	{
		show_head ();
		list ( $month, $year ) = get_month_year ();
		//            echo "1. Введено: $PHP_AUTH_USER : $PHP_AUTH_PW <br>";
		//             echo "2. Введено: $ulogin : $passwd ";
		echo "<H1>$web_client_your_stat $month $year</H1>\n";
		$REMOTE_USER=mysql_real_escape_string($REMOTE_USER);
		$result = mysql_query ( "SELECT id FROM users where login='$REMOTE_USER'", $link );
		$id = mysql_result ( $result, 0, "id" );
		show_info ( $link, $id );
		echo "<HR NOSHADE COLOR=#000000 SIZE=1>\n<BR>\n";
		show_stat ( $link, $id );
		echo "<HR NOSHADE COLOR=#000000 SIZE=1>\n";
		if (! isset ( $no_mail )) {
			show_mailstat ( $link, $PHP_AUTH_USER );
		}
		;
		echo "<HR NOSHADE COLOR=#000000 SIZE=1>\n";
		//                show_form();
		show_tail ();
		@mysql_close ( $link );
	}
} else {
	Header ( "WWW-Authenticate: Basic realm=\"$web_client_auth_realm\"" );
	Header ( "HTTP/1.0 401 Unauthorized" );
	show_head ();
	echo "1. Введено: $PHP_AUTH_USER : $PHP_AUTH_PW <br>";
	echo "$msg $msg2";
	show_help ();
	echo "<P><FONT COLOR=#FF0000>$web_client_auth_wrong</FONT></P>";
	show_tail ();
	@mysql_close ( $link );
	exit ();
}
?>