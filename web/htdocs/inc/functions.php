<?php
// registerglobals workaround
if (! get_cfg_var ( "register_globals" )) {
	extract ( $_REQUEST, EXTR_SKIP );
	extract ( $_SERVER, EXTR_OVERWRITE );
}
//Content-Type: text/css; charset=windows-1251
header ( "Content-type:text/html; charset=utf-8" );
//
//      Функция dotize($num) - расставляет точки через каждые три разряда в числе $num,
//                             начиная с самого младшего разряда
//      Вход : $num - число для преобразования
//      Выход: возвращается преобразованное число
//
// Примечание: Спасибо Greyder-у за глючную функцию. ;)
//
function dotize($num) {
	global $delimiter;
	$num = strrev ( preg_replace ( "/(\d{3})/", "\\1" . $delimiter, strrev ( $num ) ) );
	return $num;

}

//	Функция option($name, $link)
//      Вход:
//		$name имя опции
//		$link на коннект к базе
//	Выход: значение опции
function option($name, $link) {
	$result = mysql_query ( "SELECT value from options where name='$name'", $link );
	$res = @mysql_result ( $result, 0, "value" );
	@mysql_free_result ( $result );
	return $res;
}

function logevent($message) {
	global $mysql_server, $mysql_login, $mysql_passwd, $mysql_database, $PHP_AUTH_USER;
	
	$loglink = db_connect ();
	$REMOTE_USER=mysql_real_escape_string($PHP_AUTH_USER);
	$message = date ( "d.m.Y H:i:s" ) . " $REMOTE_USER " . $_SERVER ['REMOTE_ADDR'] . " " . addslashes ( $message );
	mysql_query ( "INSERT INTO syslog (record) VALUES('$message');", $loglink );
	mysql_close ( $loglink );
}

//      ---- А эта хуета лежит тут по привычке, и выбросить надо бы, да лень.
//      Функция get_month_year() - возвращает текущий месяц и год
//      Вход : ничего
//      Выход: функция возвращает массив, нулевой элемент которого -
//             название месяца (по-русски), первый элемент - год
//
function get_month_year() {
	$date = getdate ();
	return array (date ( "F" ), $date [year] );
}

// error handling routine
function debug() {
	$debug_array = debug_backtrace ();
	$counter = count ( $debug_array );
	for($tmp_counter = 0; $tmp_counter != $counter; ++ $tmp_counter) {
		?>
<table width="558" height="116" border="1" cellpadding="0"
	cellspacing="0" bordercolor="#000000">
	<tr>
		<td height="38" bgcolor="#D6D7FC"><font color="#000000">function <font
			color="#FF3300"><?php
		echo ($debug_array [$tmp_counter] ["function"]);
		?>(</font> <font
			color="#2020F0"><?php
		//count how many args a there
		$args_counter = count ( $debug_array [$tmp_counter] ["args"] );
		//print them
		for($tmp_args_counter = 0; $tmp_args_counter != $args_counter; ++ $tmp_args_counter) {
			echo ($debug_array [$tmp_counter] ["args"] [$tmp_args_counter]);
			
			if (($tmp_args_counter + 1) != $args_counter) {
				echo (", ");
			} else {
				echo (" ");
			}
		}
		?></font><font color="#FF3300">)</font></font></td>
	</tr>
	<tr>
		<td bgcolor="#5F72FA"><font color="#FFFFFF">{</font><br>
		<font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;file: <?php
		echo ($debug_array [$tmp_counter] ["file"]);
		?></font><br>
		<font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;line: <?php
		echo ($debug_array [$tmp_counter] ["line"]);
		?></font><br>
		<font color="#FFFFFF">}</font></td>
	</tr>
</table>
<?php
		if (($tmp_counter + 1) != $counter) {
			echo ("<br>was called by:<br>");
		}
	}
	exit ();
}

//
//      Функция show_info($id) - для заданного id'a делает выборку в
//                               таблице (база данных) и представляет
//                               полученные данные в HTML-формате
//      Вход : $id - username
//      Выход: если все удачно - true, иначе - код ошибки
//      Примечание: если программе не удается выполнить запрос к базе
//                    данных, возвращается код ошибки 3
//
function show_info($link, $id) {
	global $megabyte_cost;
	global $lang;
	if ($lang == 0) {
		include "../inc/ru.php";
	}
	;
	if ($lang == 1) {
		include "../inc/en.php";
	}
	;
	
	$res = mysql_query ( "SELECT u.login as login, u.quota as quota, u.used as used, u.email as email, u.descr as descr, a.vname as aid FROM users u, acl a WHERE u.id=$id and u.aid=a.id", $link );
	if (! $res) {
		echo mysql_error ();
		return 3;
	}
	$i = 0;
	if (0 == mysql_numrows ( $res )) {
		echo "error: $web_client_nouser";
		return false;
	}
	
	$nick = mysql_result ( $res, $i, "login" );
	$lim = mysql_result ( $res, $i, "quota" );
	$cur = mysql_result ( $res, $i, "used" );
	$email = mysql_result ( $res, $i, "email" );
	$descr = mysql_result ( $res, $i, "descr" );
	$timeacl = mysql_result ( $res, $i, "aid" );
	$rcpt = "rcpt='" . str_replace ( ",", "' OR rcpt='", $email ) . "'";
	$res = mysql_query ( "SELECT SUM(size) FROM mail WHERE $rcpt", $link );
	$msum = @mysql_result ( $res, 0 );
	$msum = dotize ( $msum );
	$cur = $cur - 1;
	if ($megabyte_cost > 0) {
		echo "$web_client_mbcost $megabyte_cost";
	}
	;
	echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
<TR>
<TD><B>$view_user</B></TD>
<TD>&nbsp;$descr</TD>
</TR>
<TR>
<TD><B>$view_login</B></TD>
<TD>&nbsp;$nick</TD>
</TR>
<TR><TD><B>$view_email</B></TD>
<TD>&nbsp;$email</TD></TR>";
	echo "</TABLE><BR><TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>";
	if ($megabyte_cost > 0) {
		echo "<TR><TD><B>$web_client_account_total</B></TD><TD ALIGN=RIGHT>&nbsp;";
		if ($lim != "0") {
			echo " " . dotize ( ( int ) (($megabyte_cost * ( int ) ($lim - $cur)) / (1024 * 1024)) ) . " ";
		} else {
			echo "$web_client_unlim";
		}
		echo "</TD>
<TD>&nbsp;руб.</TD>
</TR>";
	}
	;
	echo "<TR><TD><B>Квота:</B></TD>
<TD ALIGN=RIGHT>&nbsp;";
	if ($lim != "0") {
		echo " " . dotize ( $lim ) . " ";
	} else {
		echo "$web_client_unlim";
	}
	echo "</TD>
<TD>&nbsp;$word_byte</TD>
</TR>
<TR>
<TD><B>$web_client_used</B></TD>
<TD ALIGN=RIGHT>&nbsp;" . dotize ( $cur ) . "</TD>";
	if (( int ) ($msum) > 0) {
		echo "<TD>&nbsp;$word_byte [$web_client_in_mail_trf $msum" . " $word_byte ]</TD>";
	}
	;
	echo "</TR><TR>
<TD><B>Осталось (байт):</B></TD>
<TD ALIGN=RIGHT>&nbsp;";
	if ($lim != "0") {
		echo " " . dotize ( ( int ) ($lim - $cur) ) . " ";
	} else {
		echo "$web_client_unlim";
	}
	echo "</TD>
<TD>&nbsp;$word_byte</TD>
</TR>
<TR>
<TD><B>$work_time</B></TD>
<TD COLSPAN=2>&nbsp;$timeacl<TD>&nbsp;</TD>
</TR>
</TABLE>
";
	@mysql_free_result ( $res );
	return true;

}

//
//      Функция show_stat($id) - обращается к таблице traf (база данных squid),
//                               выбирает оттуда информацию для заданного логина
//                               и представляет ее в HTML-формате
//      Вход : $user - login
//      Выход: если все удачно - true, иначе - код ошибки
//      Примечание 3: если программе не удается подключится к базе данных,
//                    возвращается код ошибки 2
//      Примечание 4: если программе не удается выполнить запрос к базе
//                    данных, возвращается код ошибки 3
//
function show_stat($link, $id) {
	//BUG! date_default_timezone_get 
	date_default_timezone_set ( 'Asia/Yekaterinburg' );
	global $o;
	global $detailed;
	global $mode;
	global $lang;
	if ($lang == 0) {
		include "../inc/ru.php";
	}
	;
	if ($lang == 1) {
		include "../inc/en.php";
	}
	;
	
	switch ($o) {
		case "sizeD" :
			{
				$orderby = "size DESC";
				$o1 = "?o=sizeA";
				$o2 = "?o=siteA";
				$b1 = "#93BEE2";
				$b2 = "#FFFFFF";
				$p1 = "<A HREF=\"index.php$o1\"><IMG ALT=\"$web_client_order_desc\" BORDER=0 HEIGHT=7 WIDTH=7 SRC=../images/desc.gif></A>&nbsp;";
				$p2 = "";
				break;
			}
		case "sizeA" :
			{
				$orderby = "size ASC";
				$o1 = "?o=sizeD";
				$o2 = "?o=siteA";
				$b1 = "#93BEE2";
				$b2 = "#FFFFFF";
				$p1 = "<A HREF=\"index.php$o1\"><IMG ALT=\"$web_client_order_asc\" BORDER=0 HEIGHT=7 WIDTH=7 SRC=../images/asc.gif></A>&nbsp;";
				$p2 = "";
				break;
			}
		case "siteD" :
			{
				$orderby = "site DESC";
				$o1 = "?o=sizeA";
				$o2 = "?o=siteA";
				$b1 = "#FFFFFF";
				$b2 = "#93BEE2";
				$p1 = "";
				$p2 = "<A HREF=\"index.php$o2\"><IMG ALT=\"$web_client_order_desc\" BORDER=0 HEIGHT=7 WIDTH=7 SRC=../images/desc.gif></A>&nbsp;";
				break;
			}
		case "siteA" :
			{
				$orderby = "site ASC";
				$o1 = "?o=sizeA";
				$o2 = "?o=siteD";
				$b1 = "#FFFFFF";
				$b2 = "#93BEE2";
				$p1 = "";
				$p2 = "<A HREF=\"index.php$o2\"><IMG ALT=\"$web_client_order_asc\" BORDER=0 HEIGHT=7 WIDTH=7 SRC=../images/asc.gif></A>&nbsp;";
				break;
			}
		default :
			{
				$orderby = "size DESC";
				$o1 = "?o=sizeA";
				$o2 = "?o=siteA";
				$b1 = "#93BEE2";
				$b2 = "#FFFFFF";
				$p1 = "<A HREF=\"index.php$o1\"><IMG ALT=\"$web_client_order_asc\" BORDER=0 HEIGHT=7 WIDTH=7 SRC=../images/asc.gif></A>&nbsp;";
				$p2 = "";
			}
	}
	
	echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
<TR>
<TD BGCOLOR=#93BEE2
><TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1>
<TR>
<TD ALIGN=CENTER BGCOLOR=$b1>$p1<B><A HREF=\"index.php$o1&type=sites&id=$id\" STYLE=\"color:#000000; text-decoration: underline\">$web_client_downloaded</A></B></TD>
<TD ALIGN=CENTER BGCOLOR=$b2>$p2<B><A HREF=\"index.php$o2&type=sites&id=$id\" STYLE=\"color:#000000; text-decoration: underline\">$web_client_site</A></B></TD>
<TD ALIGN=CENTER BGCOLOR=#bfbfbf><B>$web_client_futime</B></TD>
<TD ALIGN=CENTER BGCOLOR=#bfbfbf><B>$web_client_lutime</B></TD>
</TR>
";
	$res = mysql_query ( "SELECT * FROM site WHERE u_id=$id ORDER BY $orderby", $link );
	if (! $res)
		return 3;
	for($i = 0; $i < MYSQL_NUMROWS ( $res ); $i ++) {
		$site = mysql_result ( $res, $i, "site" );
		$size = mysql_result ( $res, $i, "size" );
		$lutime = mysql_result ( $res, $i, "lutime" );
		$futime = mysql_result ( $res, $i, "futime" );
		
		echo "<TR>
<TD BGCOLOR=#FFF7E5 ALIGN=RIGHT>" . dotize ( $size ) . "</TD>
<TD BGCOLOR=#FFF7E5>";
		if ($detailed == 1)
			if ($mode == "admin") {
				echo "<a href=\"index.php?id=$id&type=detail&site=$site\">";
			} else {
				echo "<a href=\"index.php?type=detail&site=$site\">";
			}
		;
		echo "$site";
		if ($detailed == 1) {
			echo "</a>";
		}
		;
		echo "<TD BGCOLOR=#FFF7E5>" . strftime ( "%B %d, %T", ( int ) $futime ) . "</TD>";
		echo "<TD BGCOLOR=#FFF7E5>" . strftime ( "%B %d, %T", ( int ) $lutime ) . "</TD>";
		echo "</TD></TR>";
	}
	echo "</TABLE></TD>
</TR>
</TABLE>
";
	$res = mysql_query ( "SELECT SUM(size) FROM site WHERE u_id=$id", $link );
	$sum = mysql_result ( $res, 0 );
	$res = mysql_query ( "SELECT count(site) FROM site WHERE u_id=$id", $link );
	$sites = mysql_result ( $res, 0 );
	mysql_close ();
	echo "<BR><SPAN CLASS=\"smalltext\"><B>$word_total:</B> " . dotize ( $sum ) . " $word_byte $web_client_downloaded_from $sites $web_client_sites_wrd.</SPAN>";
	return true;
}

//
//    Функция show_tail() - выводит хвост HTML-страницы ;)
//    Вход : ничего
//    Выход: всегда true
//
function show_tail() {
	global $version;
	include ("timer_show.php");
}

//
//      Функция show_mailstat($link, $login) - обращается к таблице mail
//                               выбирает оттуда информацию для заданного login-a
//                               и представляет ее в HTML-формате
//      Вход : $link - дескриптор базы данных
//             $login
//      Выход: если все удачно - true, иначе - код ошибки
//      Примечание 1: если программе не удается подключится к базе данных,
//                    возвращается код ошибки 2
//      Примечание 2: если программе не удается выполнить запрос к базе
//                    данных, возвращается код ошибки 3
//      Примечание 3: скрипты учёта почты пока не идут в комплекте.
//
function show_mailstat($link, $login) {
	global $lang;
	if ($lang == 0) {
		include "../inc/ru.php";
	}
	;
	if ($lang == 1) {
		include "../inc/en.php";
	}
	;
	
	$result = mysql_query ( "SELECT email FROM traf WHERE login='$login'", $link );
	if (mysql_numrows ( $result ) == 1) {
		$email = mysql_result ( $result, 0, "email" );
		$rcpt = "rcpt='" . str_replace ( ",", "' OR rcpt='", $email ) . "'";
		$result = mysql_query ( "SELECT size, frm FROM mail WHERE $rcpt ORDER BY size DESC", $link );
	}
	if (mysql_numrows ( $result ) > 0) {
		
		echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
<TR>
<TD BGCOLOR=#93BEE2><TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1><TR>
<TD ALIGN=CENTER BGCOLOR=#FFF7E5><B><SPAN STYLE=\"color:#000000\">$web_client_inmail</SPAN></TD>
<TD ALIGN=CENTER BGCOLOR=#FFF7E5><B><SPAN STYLE=\"color:#000000\">$web_client_mailfrom</SPAN></B></TD>
</TR>
";
		
		if (! $result)
			return 3;
		for($i = 0; $i < mysql_numrows ( $result ); $i ++) {
			$site = mysql_result ( $result, $i, "frm" );
			$size = mysql_result ( $result, $i, "size" );
			echo "<TR>
<TD BGCOLOR=#FFF7E5 ALIGN=RIGHT>" . dotize ( $size ) . "</TD>
<TD BGCOLOR=#FFF7E5>$site</TD>
</TR>
";
		}
		echo "</TABLE></TD>
</TR>
</TABLE>
";
		$result = mysql_query ( "SELECT SUM( size ) FROM mail WHERE $rcpt", $link );
		$sum = mysql_result ( $result, 0 );
		echo "<BR>
<SPAN CLASS=\"smalltext\"><B>$word_total:</B> " . dotize ( $sum ) . " $word_byte .</SPAN>
";
	}
	;
	return true;

}

//
//    Функция show_head() - выводит шапку HTML-страницы
//    Вход : ничего
//    Выход: всегда true
//
function show_head() {
	global $version;
	global $mod_name, $origin;
	
	include ("timer_set.php");
	
	echo "<HTML>
<HEAD>
<META HTTP-EQUIV=\"Expires\" CONTENT=\"0\">
<META HTTP-EQUIV=\"Pragma\"  CONTENT=\"no-cache\">
<META HTTP-EQUIV=\"Cache-Control\" CONTENT=\"no-cache\">
<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=utf-8\">";
	if (isset ( $origin )) {
		echo "<TITLE>$origin: $mod_name</TITLE>";
	} else {
		echo "<TITLE>SAcc v. $version: $mod_name</TITLE>";
	}
	;
	echo "<LINK REL=STYLESHEET TYPE=\"text/css\" HREF=../1.css>
</HEAD>
<BODY BGCOLOR=#FFFFFF TEXT=#000000>
";
	return true;

}

//
//    Функция show_error() - выводит стандартное сообщение об ошибке
//    Вход : ничего
//    Выход: всегда true
//
function show_error() {
	global $word_warning, $web_client_noaccess;
	echo "<H1>$word_warning</H1><P>
        <FONT COLOR=#FF0000>$web_client_noaccess</FONT></P>";
	//<FORM ACTION=index.php METHOD=get>
	//<INPUT TYPE=submit CLASS=inputsubmit NAME=try VALUE=\"Попробовать еще раз\">
	//</FORM>";
	return true;
}

//
//      Функция show_detail($id, $site) - обращается к таблице detail (база данных trf),
//                               выбирает оттуда информацию для заданного логина и сайта
//                               и представляет ее в HTML-формате
//      Вход : $id - login
//      Выход: если все удачно - true, иначе - код ошибки
//      Примечание 3: если программе не удается подключится к базе данных,
//                    возвращается код ошибки 2
//      Примечание 4: если программе не удается выполнить запрос к базе
//                    данных, возвращается код ошибки 3
//
function show_detail($link, $id, $site) {
	global $o;
	global $mode;
	global $lang;
	global $settings;
	
	$page = ( int ) $_GET ['page'];
	// ----------------------------
	$pagestep = $settings ['pagelen'];
	//-----------------------------
	if ("" == $site) {
		echo "строка сайта не может быть пустой!";
		return 1;
	}
	;
	$cquery = $_SERVER ['PHP_SELF'] . "?" . $_SERVER ['QUERY_STRING'];
	if (0 < strpos ( $cquery, "&page=" )) {
		$cquery = substr ( $cquery, 0, strpos ( $cquery, "&page=" ) );
	}
	;
	echo "<br><center><p style=\"font-size: 12\">";
	$result = mysql_query ( "SELECT count(utime) as rec FROM detail WHERE u_id='$id' and url like('%" . mysql_escape_string ( $site ) . "%')", $link );
	$pages = floor ( mysql_result ( $result, 0, "rec" ) / $pagestep );
	
	if ($page != 0)
		echo "<a href=\"$cquery" . "&page=0\"><<</a> ";
	if ($page > 10)
		echo "<a href=\"$cquery&page=" . ($page - 10) . "\"><</a> ";
	if ($page - 5 > 0) {
		for($i = $page - 5; $i < $page; $i ++) {
			echo "<a href=\"$cquery&page=$i\">$i</a> ";
		}
	} else {
		for($i = 1; $i < $page; $i ++) {
			echo "<a href=\"$cquery&page=$i\">$i</a> ";
		}
	}
	echo "<b>$page</b> ";
	if ($page + 5 < $pages) {
		for($i = $page + 1; $i < $page + 6; $i ++) {
			echo "<a href=\"$cquery&page=$i\">$i</a> ";
		}
	} else {
		for($i = $page + 1; $i < $pages; $i ++) {
			echo "<a href=\"$cquery&page=$i\">$i</a> ";
		}
	}
	
	if (($page + 10) < $pages) {
		echo "<a href=\"$cquery&page=" . ($page + 10) . "\">></a> ";
	}
	if ($page < $pages) {
		echo "<a href=\"$cquery&page=" . $pages . "\">>></a> ";
	}
	
	echo "</p>";
	
	if ($mode == "admin") {
		$query = "&type=detail&id=" . $_GET ['id'] . "&site=$site";
	} else {
		$query = "&type=detail&site=$site";
	}
	;
	switch ($o) {
		case "sizeD" :
			{
				$orderby = "size DESC";
				$o1 = "?o=sizeA";
				$o2 = "?o=timeA";
				$b1 = "#93BEE2";
				$b2 = "#FFFFFF";
				$p1 = "<A HREF=\"index.php" . $o1 . $query . "\"><IMG ALT=\"$web_client_order_desc\" BORDER=0 HEIGHT=7 WIDTH=7 SRC=../images/desc.gif></A>&nbsp;";
				$p2 = "";
				break;
			}
		case "sizeA" :
			{
				$orderby = "size ASC";
				$o1 = "?o=sizeD";
				$o2 = "?o=timeA";
				$b1 = "#93BEE2";
				$b2 = "#FFFFFF";
				$p1 = "<A HREF=\"index.php" . $o1 . $query . "\"><IMG ALT=\"$web_client_order_asc\" BORDER=0 HEIGHT=7 WIDTH=7 SRC=../images/asc.gif></A>&nbsp;";
				$p2 = "";
				break;
			}
		case "timeD" :
			{
				$orderby = "utime DESC";
				$o1 = "?o=sizeA";
				$o2 = "?o=timeA";
				$b1 = "#FFFFFF";
				$b2 = "#93BEE2";
				$p1 = "";
				$p2 = "<A HREF=\"index.php" . $o2 . $query . "\"><IMG ALT=\"$web_client_order_desc\" BORDER=0 HEIGHT=7 WIDTH=7 SRC=../images/desc.gif></A>&nbsp;";
				break;
			}
		case "timeA" :
			{
				$orderby = "utime ASC";
				$o1 = "?o=sizeA";
				$o2 = "?o=timeD";
				$b1 = "#FFFFFF";
				$b2 = "#93BEE2";
				$p1 = "";
				$p2 = "<A HREF=\"index.php" . $o2 . $query . "\"><IMG ALT=\"$web_client_order_asc\" BORDER=0 HEIGHT=7 WIDTH=7 SRC=../images/asc.gif></A>&nbsp;";
				break;
			}
		default :
			{
				$orderby = "size DESC";
				$o1 = "?o=sizeA";
				$o2 = "?o=timeA";
				$b1 = "#93BEE2";
				$b2 = "#FFFFFF";
				$p1 = "<A HREF=\"index.php" . $o1 . $query . "\"><IMG ALT=\"$web_client_order_desc\" BORDER=0 HEIGHT=7 WIDTH=7 SRC=../images/desc.gif></A>&nbsp;";
				$p2 = "";
			}
	}
	
	if ($_COOKIE ['lang'] == 0) {
		include "../inc/ru.php";
	}
	;
	if ($_COOKIE ['lang'] == 1) {
		include "../inc/en.php";
	}
	;
	
	echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0><TR>
<TD BGCOLOR=#93BEE2><TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1><TR>
<TD ALIGN=CENTER BGCOLOR=$b2>$p2<B><A HREF=\"index.php$o2$query\" STYLE=\"color:#000000; text-decoration: underline\">$word_time</A></B></TD>
<TD ALIGN=CENTER BGCOLOR=$b1>$p1<B><A HREF=\"index.php$o1$query\" STYLE=\"color:#000000; text-decoration: underline\">$word_byte</A></B></TD>
<TD ALIGN=LEFT BGCOLOR=#FFFFFF><b>$web_client_query</B></TD>
<TD ALIGN=RIGHT BGCOLOR=#FFFFFF><b>msec</B></TD>
<TD ALIGN=LEFT BGCOLOR=#FFFFFF><b>$web_client_src_ip</B></TD>
<TD ALIGN=LEFT BGCOLOR=#FFFFFF><b>$web_client_server_rply</B></TD></TR>";
	
	$res = mysql_query ( "SELECT utime, qtime, ip_addr, code, size, url FROM detail WHERE u_id='$id' and url like('%" . mysql_escape_string ( $site ) . "%')  ORDER BY $orderby limit " . ($page * $pagestep) . ",$pagestep", $link );
	//    if ( !$res )
	//        return 3;
	for($i = 0; $i < MYSQL_NUMROWS ( $res ); $i ++) {
		$utime = mysql_result ( $res, $i, "utime" );
		$size = mysql_result ( $res, $i, "size" );
		$qtime = mysql_result ( $res, $i, "qtime" );
		$ip_addr = long2ip ( mysql_result ( $res, $i, "ip_addr" ) );
		$code = mysql_result ( $res, $i, "code" );
		$url = mysql_result ( $res, $i, "url" );
		
		echo "<TR>
<TD BGCOLOR=#FFF7E5>" . strftime ( "%B %d, %T", ( int ) $utime ) . "</TD>
<TD BGCOLOR=#FFF7E5 ALIGN=RIGHT>" . dotize ( $size ) . "</TD>
<TD BGCOLOR=#FFF7E5 ALIGN=LEFT>$url</TD>
<TD BGCOLOR=#FFF7E5 ALIGN=RIGHT>$qtime</TD>
<TD BGCOLOR=#FFF7E5 ALIGN=RIGHT>$ip_addr</TD>
<TD BGCOLOR=#FFF7E5 ALIGN=LEFT>$code</TD>
</TR>
";
	}
	echo "</TABLE></TD>
</TR>
</TABLE>
";
	$res = mysql_query ( "SELECT SUM(size) FROM detail WHERE u_id='$id' and url like('%" . mysql_escape_string ( $site ) . "%')", $link );
	$sum = mysql_result ( $res, 0 );
	$res = mysql_query ( "SELECT count(utime) FROM detail WHERE u_id='$id' and url like('%" . mysql_escape_string ( $site ) . "%')", $link );
	$sites = mysql_result ( $res, 0 );
	mysql_close ();
	echo "<BR>
<SPAN CLASS=\"smalltext\"><B>$word_total:</B> " . dotize ( $sum ) . " $word_byte $web_client_downloaded_with " . dotize ( $sites ) . " $web_client_query.</SPAN>
";
	return true;

}

//      // NOT IMPLEMENTED NOW //
//      Функция show_acl() - обращается к таблице acl (база данных sacc),
//                               выбирает оттуда информацию
//                               и представляет ее в HTML-формате
//      Вход : $link - MySQL database connection descriptor
//      Выход: если все удачно - true, иначе - код ошибки
//      Примечание 3: если программе не удается подключится к базе данных,
//                    возвращается код ошибки 2
//      Примечание 4: если программе не удается выполнить запрос к базе
//                    данных, возвращается код ошибки 3
//
function show_acl($link) {
	global $o;
	
	switch ($o) {
		case "nameD" :
			{
				$orderby = "name DESC";
				$o1 = "?o=nameA";
				$o2 = "?o=descA";
				$b1 = "#93BEE2";
				$b2 = "#FFFFFF";
				$p1 = "<A HREF=\"index.php" . $o1 . "&type=acl\"><IMG ALT=\"Отсортировано в обратном порядке\" BORDER=0 HEIGHT=7 WIDTH=7 SRC=../images/desc.gif></A>&nbsp;";
				$p2 = "";
				break;
			}
		case "nameA" :
			{
				$orderby = "name ASC";
				$o1 = "?o=nameD";
				$o2 = "?o=descA";
				$b1 = "#93BEE2";
				$b2 = "#FFFFFF";
				$p1 = "<A HREF=\"index.php" . $o1 . "&type=acl\"><IMG ALT=\"Отсортировано в прямом порядке\" BORDER=0 HEIGHT=7 WIDTH=7 SRC=../images/asc.gif></A>&nbsp;";
				$p2 = "";
				break;
			}
		case "descD" :
			{
				$orderby = "vname DESC";
				$o1 = "?o=nameA";
				$o2 = "?o=descA";
				$b1 = "#FFFFFF";
				$b2 = "#93BEE2";
				$p1 = "";
				$p2 = "<A HREF=\"index.php" . $o2 . "&type=acl\"><IMG ALT=\"Отсортировано в обратном порядке\" BORDER=0 HEIGHT=7 WIDTH=7 SRC=../images/desc.gif></A>&nbsp;";
				break;
			}
		case "descA" :
			{
				$orderby = "vname ASC";
				$o1 = "?o=nameA";
				$o2 = "?o=descD";
				$b1 = "#FFFFFF";
				$b2 = "#93BEE2";
				$p1 = "";
				$p2 = "<A HREF=\"index.php" . $o2 . "&type=acl\"><IMG ALT=\"Отсортировано в прямом порядке\" BORDER=0 HEIGHT=7 WIDTH=7 SRC=../images/asc.gif></A>&nbsp;";
				break;
			}
		default :
			{
				$orderby = "id ASC";
				$o1 = "?o=nameA";
				$o2 = "?o=descD";
				$b1 = "#FFFFFF";
				$b2 = "#93BEE2";
				$p1 = "<A HREF=\"index.php" . $o2 . "&type=acl\"><IMG ALT=\"Отсортировано в прямом порядке\" BORDER=0 HEIGHT=7 WIDTH=7 SRC=../images/asc.gif></A>&nbsp;";
				$p2 = "";
			}
	}
	
	echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0><TR>
<TD BGCOLOR=#93BEE2><TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1><TR>
<a href=\"index.php?type=acled&id=0\">add new acl</a>
</tr>
<tr>
<TD ALIGN=CENTER BGCOLOR=$b2>$p2<B><A HREF=\"index.php$o2\" STYLE=\"color:#000000; text-decoration: underline\">Описание</A></B></TD>
<TD ALIGN=CENTER BGCOLOR=$b1>$p1<B><A HREF=\"index.php$o1\" STYLE=\"color:#000000; text-decoration: underline\">Системное имя</A></B></TD>";
	$res = mysql_query ( "SELECT sysname, vname, id FROM acl ORDER BY $orderby", $link );
	if (! $res)
		return 3;
	for($i = 0; $i < MYSQL_NUMROWS ( $res ); $i ++) {
		$descr = mysql_result ( $res, $i, "vname" );
		$name = mysql_result ( $res, $i, "sysname" );
		$id = mysql_result ( $res, $i, "id" );
		echo "<TR>
<TD BGCOLOR=#FFF7E5 ALIGN=LEFT><a href=\"index.php?type=acled&id=$id\">" . $descr . "</a></TD>
<TD BGCOLOR=#FFF7E5 ALIGN=LEFT>" . $name . "</TD>
</TR>
";
	}
	echo "</TABLE></TD>
</TR>
</TABLE>
";
	$res = mysql_query ( "SELECT count(id) FROM acl", $link );
	$acl_count = mysql_result ( $res, 0 );
	mysql_close ();
	echo "<BR>
<SPAN CLASS=\"smalltext\"><B>Итого:</B> " . dotize ( $acl_count ) . " правил.</SPAN>
";
	return 0;
}
;

?>
