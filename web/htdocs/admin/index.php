<?php
require ("../inc/version.php");
require ("functions.php");
require ("../inc/functions.php");
require_once ("../inc/auth.php");
require ("../inc/mysql.php");
$mod_name = $admin_title;
$mode = "admin";
if ($_COOKIE ['lang'] == 0) {
	include "../inc/ru.php";
}
;
if ($_COOKIE ['lang'] == 1) {
	include "../inc/en.php";
}
;

# ----------------- Аутентификация ---------------------
if (isset ( $PHP_AUTH_USER )) {
	$ok = false;
	if (auth_adm ( $PHP_AUTH_USER, $PHP_AUTH_PW )) {
		$ok = true;
	}
	if (! $ok) {
		if (isset ( $try )) {
			Header ( "WWW-Authenticate: Basic realm=\"$admin_auth_realm\"" );
			Header ( "HTTP/1.0 401 Unauthorized" );
		}
		show_head ();
		show_error ();
		echo "$dat1  <br>";
		echo "$PHP_AUTH_USER, $PHP_AUTH_PW" . md5 ( $PHP_AUTH_PW ) . "";
		show_tail ();
		@mysql_close ( $link );
		exit ();
	}
} else {
	Header ( "WWW-Authenticate: Basic realm=\"$admin_auth_realm\"" );
	Header ( "HTTP/1.0 401 Unauthorized" );
	show_head ();
	show_error ();
	show_tail ();
	exit ();
}

# --------------------- MAIN --------------------
$link = db_connect ();
$result = mysql_query ( "SELECT name, value FROM options;", $link );
for($i = 0; $i < mysql_numrows ( $result ); $i ++) {
	$name = mysql_result ( $result, $i, "name" );
	$value = mysql_result ( $result, $i, "value" );
	$settings [$name] = $value;
}
$origin = $settings ['origin'];
$type = $_GET ['type'];
# -------------------- Создание нового пользователя (событие "create") -----------------------
if ($type == "create") {
	$msg = "";
	$other = stripslashes ( trim ( $_GET ['other'] ) );
	$login = addslashes ( stripslashes ( trim ( $_GET ['login'] ) ) );
	$pass1 = stripslashes ( $_GET ['pass'] );
	$pass2 = stripslashes ( $_GET ['pass2'] );
	$todo = stripslashes ( $_GET ['todo'] );
	
	$email = ereg_replace ( " +", "", strtolower ( stripslashes ( trim ( $_GET ['email'] ) ) ) );
	// надо сделать поиск собаки и если нет - то добавлять (влом)
	//    if ( $email == "" ) $email = "$mail_domen";
	$lim = ( int ) stripslashes ( str_replace ( $delimiter, "", trim ( $_GET ['lim'] ) ) );
	$dlim = ( int ) stripslashes ( str_replace ( $delimiter, "", trim ( $_GET ['dlim'] ) ) );
	$cur = ( int ) stripslashes ( str_replace ( $delimiter, "", trim ( $_GET ['cur'] ) ) );
	$timeacl = ( int ) stripslashes ( trim ( $_GET ['acl'] ) );
	
	# ---------------- Нажата кнопка "Сохранить" ("createbutton") -----------------
	if (isset ( $createbutton )) {
		# проверка поля "Пользователь" ($other)
		if ($other == "")
			$msg .= "<SPAN CLASS=\"errmsg\">$user_blank</SPAN>\n<BR>\n";
			# проверка поля login
		if ($login == "")
			$msg .= "<SPAN CLASS=\"errmsg\">$login_blank</SPAN>\n<BR>\n";
		else {
			$result = mysql_query ( "SELECT * FROM users WHERE login='$login'", $link );
			if (mysql_numrows ( $result ) != 0)
				$msg .= "<SPAN CLASS=\"errmsg\">$user_exist</SPAN>\n<BR>\n";
		}
		//        if ( $cur == "0" ) $cur="1";
		# проверка поля "password" ($pass1=$pass2)
		if ($pass1 != $pass2)
			$msg .= "<SPAN CLASS=\"errmsg\">$passwd_noteq</SPAN>\n<BR>\n";
			# ошибок не обнаружилось
		if ($msg == "") {
			$result = mysql_query ( "INSERT INTO users (login, used, quota, dquota, email, descr, aid) VALUES('$login', $cur, $lim, $dlim, '" . addslashes ( $email ) . "', '" . addslashes ( $other ) . "','" . addslashes ( $timeacl ) . "')", $link );
			mysql_error ();
			if (mysql_affected_rows () != 0) {
				if ($auth_mode == 1) {
					$exc = "$bin_prefix/htpasswd -b $ncsa_passwd " . quotemeta ( $login ) . " " . quotemeta ( $pass1 );
					$data = shell_exec ( $exc );
				}
				$result = mysql_query ( "SELECT id FROM users WHERE login='$login'", $link );
				$id = mysql_result ( $result, 0, "id" );
				if ($todo == 0)
					Header ( "Location: index.php?type=edit&id=$id&data=$data" );
				if ($todo == 1)
					Header ( "Location: index.php?id=$id" );
				if ($todo == 3)
					Header ( "Location: index.php?type=create&todo=$todo" );
				logevent ( "create new user $login ($timeacl, $lim, $cur, $email, $other)" );
				@mysql_close ( $link );
				exit ();
			}
		} else { #--- информация не занесена в БД по к.-л. причинам ---
			// Какая то ботва произошла похоже...
			show_head ();
			echo "<H1>$add_user_fail</H1>\n<HR NOSHADE COLOR=#000000 SIZE=1>\n";
			echo "<FORM METHOD=get ACTION=index.php>
                        <INPUT TYPE=hidden NAME=type VALUE=edit>
                        <INPUT TYPE=hidden NAME=login VALUE=\"$login\">
                        <INPUT TYPE=button CLASS=\"inputsubmit\" ID=\"bg_blue\" VALUE=\"$back_tomain\" OnClick=\"window.location = 'index.php'\">&nbsp;&nbsp;&nbsp;&nbsp;
                        <INPUT TYPE=button CLASS=\"inputsubmit\" ID=\"bg_blue\" VALUE=\"$crt_newuser\" OnClick=\"window.location = 'index.php?type=create'\">
                      </FORM>\n";
			show_tail ();
			mysql_close ( $link );
			exit ();
		}
	}
	# -------------- Default-действие для события "create" -----------------
	$mod_name = $new_user_creation_header;
	show_head ();
	show_help ();
	//    echo "<H1>$new_user_creation_header</H1>\n";
	$acl = $settings ["def_timeacl"];
	$lim = $settings ["std_limit"];
	$todo = ( int ) stripslashes ( $_GET ['todo'] );
	if ($todo == 0) {
		$todo = $settings [create_todo];
	}
	;
	echo "$msg
          <HR NOSHADE COLOR=#000000 SIZE=1>
          <FORM ACTION=\"index.php\" METHOD=get>
          <TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
          <TR HEIGHT=20>
            <TD><BR></TD>
            <TD ALIGN=CENTER VALIGN=TOP><SPAN CLASS=\"verdanabold11px\">$fill_columns</SPAN></TD>
            <TD ALIGN=CENTER VALIGN=TOP><SPAN CLASS=\"verdanabold11px\">$like_that</SPAN></TD>
          </TR>
          <TR>
            <TD><P><B>Пользователь:</B></P></TD>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=other VALUE=\"" . htmlspecialchars ( $other ) . "\"></TD>
            <TD><SPAN CLASS=\"tahoma11px\">&nbsp;&nbsp;&nbsp;&nbsp;$descr_like</SPAN></TD>
          </TR>
          <TR>
            <TD><P><B>Login:</B></P></TD>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=login VALUE=\"" . htmlspecialchars ( $login ) . "\"></TD>
            <TD><SPAN CLASS=\"tahoma11px\">&nbsp;&nbsp;&nbsp;&nbsp;$login_like</SPAN></TD>
          </TR>";
	if ($auth_mode == 1) {
		// ncsa mode
		echo "<TR>
            <TD><P><B>Password:</B></P></TD>
            <TD><INPUT TYPE=password CLASS=\"input\" NAME=pass VALUE=\"\"></TD>
            <TD><SPAN CLASS=\"tahoma11px\">&nbsp;&nbsp;&nbsp;&nbsp;$passwd_like_1</SPAN></TD>
          </TR>

          <TR>
            <TD><P><B>Pass again:</B></P></TD>
            <TD><INPUT TYPE=password CLASS=\"input\" NAME=pass2 VALUE=\"\"></TD>
            <TD><SPAN CLASS=\"tahoma11px\">&nbsp;&nbsp;&nbsp;&nbsp;$passwd_like_2</SPAN></TD>
          </TR>";
	}
	
	echo "          <TR>
            <TD><P><B>Эл. почта:</B></P></TD>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=email VALUE=\"" . htmlspecialchars ( $email ) . "\"></TD>
            <TD><SPAN CLASS=\"tahoma11px\">&nbsp;&nbsp;&nbsp;&nbsp;ivanov$mail_domen, ivanov1$mail_domen</SPAN></TD>
          </TR>
          <TR>
            <TD><P><B>Лимит текущий(байт):</B></P></TD>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=lim VALUE=\"" . dotize ( $lim ) . "\"></TD>
            <TD><SPAN CLASS=\"tahoma11px\">&nbsp;&nbsp;&nbsp;&nbsp;$std_limit (или " . dotize ( $std_limit ) . ")</SPAN></TD>
          </TR>
          <TR>
            <TD><P><B>Лимит в месяц(байт):</B></P></TD>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=dlim VALUE=\"" . dotize ( $lim ) . "\"></TD>
            <TD><SPAN CLASS=\"tahoma11px\">&nbsp;&nbsp;&nbsp;&nbsp;$std_limit (или " . dotize ( $std_limit ) . ")</SPAN></TD>
          </TR>
          <TR>
            <TD><P><B>Трафик (байт):</B></P></TD>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=cur VALUE=\"" . dotize ( $cur ) . "\"></TD>
            <TD><SPAN CLASS=\"tahoma11px\">&nbsp;&nbsp;&nbsp;&nbsp;$limit_like</SPAN></TD>
          </TR>
          </TABLE>
        <!--  WORKTIME   -->
ограничение</td>
<td><select name=acl style=\"font-weight: bold\">";
	$result = mysql_query ( "select vname, id from acl", $link );
	
	for($i = 0; $i < mysql_numrows ( $result ); $i ++) {
		$aid = mysql_result ( $result, $i, "id" );
		$vname = mysql_result ( $result, $i, "vname" );
		echo "<option value=\"$aid\"";
		if ($acl == $aid) {
			echo "selected";
		}
		echo ">$vname";
	}
	echo "</select></td>
   </td></tr><tr>
   <br>";
	//echo $todo;
	echo "действие <select name=todo style=\"font-weight: bold\">";
	echo "<option value=\"1\"";
	echo ($todo == 1) ? "selected" : " ";
	echo ">создать и на главную";
	echo "<option value=\"0\"";
	echo ($todo == 0) ? "selected" : " ";
	echo ">создать, и перейти в редактирование";
	echo "<option value=\"3\"";
	echo ($todo == 3) ? "selected" : " ";
	echo ">создать и снова на создание";
	echo "</select>
          <BR>
            <INPUT TYPE=submit CLASS=\"inputsubmit\" ID=\"bg_green\" NAME=createbutton VALUE=\"$crt_newuser\">&nbsp;&nbsp;&nbsp;&nbsp;
            <INPUT TYPE=button CLASS=\"inputsubmit\" ID=\"bg_blue\"  VALUE=\"$back_tomain\" OnClick=\"window.location = 'index.php'\">&nbsp;&nbsp;&nbsp;&nbsp;
            <INPUT TYPE=button CLASS=\"inputsubmit\" ID=\"bg_pink\"  VALUE=\"$clear_form\" onClick=\"forms[0].elements[0].value=''; forms[0].elements[1].value=''; forms[0].elements[2].value=''; forms[0].elements[3].value='@prognoz.ru'; forms[0].elements[4].value=0; forms[0].elements[5].value=0; forms[0].elements[6].value=''; \">    
            <INPUT TYPE=hidden NAME=type VALUE=\"create\">
        </FORM>\n";
	show_tail ();
	mysql_close ( $link );
	exit ();
} # ------------ Редактирование опций (событие "options") -------------
elseif ($type == "options") {
	# ------------- Нажата кнопка "Сохранить" ----------------
	if (isset ( $savebutton )) {
		$link2 = db_connect ();
		$result = mysql_query ( "SELECT name FROM options;", $link );
		for($i = 0; $i < mysql_numrows ( $result ); $i ++) {
			$name = mysql_result ( $result, $i, "name" );
			//$login    = addslashes(stripslashes(trim($_GET['login'])));
			$value = $_GET [$name];
			$result2 = mysql_query ( "update options set value='$value' where name='$name'", $link2 );
			//		echo "update `options` set value='$value' where name='$name'<br>";echo mysql_error($link2);echo "<br>";
		}
		Header ( "Location: index.php?type=options" );
		exit ();
	}
	;
	show_head ();
	show_help ();
	
	echo "<FORM ACTION=\"index.php\" METHOD=get>
          <INPUT TYPE=hidden NAME=type VALUE=\"options\">
          <TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>";
	$result = mysql_query ( "SELECT name, value, descr, help FROM options;", $link );
	for($i = 0; $i < mysql_numrows ( $result ); $i ++) {
		$name = mysql_result ( $result, $i, "name" );
		$value = mysql_result ( $result, $i, "value" );
		$descr = mysql_result ( $result, $i, "descr" );
		$help = mysql_result ( $result, $i, "help" );
		echo "<TR>
            <TD><B>$descr</B> </TD>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=$name VALUE=\"$value\"></TD>
            <TD>$help</TD>
          </TR>";
	}
	echo "</TABLE>
	<INPUT TYPE=submit CLASS=\"inputsubmit\" ID=\"bg_green\" NAME=savebutton VALUE=\"$edit_save\">&nbsp;&nbsp;&nbsp;&nbsp;
	<INPUT TYPE=button CLASS=\"inputsubmit\" ID=\"bg_blue\"  VALUE=\"$back_tomain\" onClick=\"window.location='index.php'\">&nbsp;&nbsp;&nbsp;&nbsp;";
	show_tail ();
	exit ( 0 );
} # ------------ Редактирование существующего пользователя (событие "edit") -------------
elseif ($type == "edit") {
	//надо переделать нахуй на привязку по ID
	# ------------- Нажата кнопка "Сохранить" ----------------
	if (isset ( $savebutton )) {
		$other = stripslashes ( trim ( $other ) );
		$login = stripslashes ( trim ( $nick ) );
		$id = stripslashes ( trim ( $id ) );
		$acl = ( int ) stripslashes ( trim ( $_GET ['acl'] ) );
		$email = ereg_replace ( " +", "", strtolower ( stripslashes ( trim ( $email ) ) ) );
		//        if ( $email == "" ) $email = "";
		$lim = ( int ) stripslashes ( str_replace ( $delimiter, "", trim ( $lim ) ) );
		$dlim = ( int ) stripslashes ( str_replace ( $delimiter, "", trim ( $dlim ) ) );
		$cur = ( int ) stripslashes ( str_replace ( $delimiter, "", trim ( $cur ) ) );
		$fund = ( int ) stripslashes ( str_replace ( $delimiter, "", trim ( $fund ) ) );
		$status = ( int ) stripslashes ( str_replace ( $delimiter, "", trim ( $status ) ) );
		# проверка поля "password" ($pass=$pass2)
		if ($pass != $pass2)
			$msg .= "<SPAN CLASS=\"errmsg\">$passwd_noteq</SPAN>\n<BR>\n";
		if (($auth_mode == 1) && (isset ( $pass ))) {
			$exc = "$bin_prefix/htpasswd -b $ncsa_passwd " . quotemeta ( $login ) . " " . quotemeta ( $pass );
			$data = shell_exec ( $exc );
		}
		
		// my billing 3r = 1m
		if ($fund > 0) {
			$lim = $lim + 1024 * 1024 * (( int ) $fund / $megabyte_cost);
		}
		;
		//        $timeacl = stripslashes(trim($timeacl));
		

		# ------{ критичный кусок: необходимы проверки, как при создании пользователя
		$result = mysql_query ( "UPDATE users SET login='" . addslashes ( $login ) . "', used=$cur, quota=$lim, dquota=$dlim, status=$status, email='" . addslashes ( $email ) . "', descr='" . addslashes ( $other ) . "', aid=$acl WHERE id='$id'", $link );
		mysql_error ();
		mysql_query ( "insert into queue (command, itime) values (1, " . time () . ")", $link );
		
		logevent ( "edit user $login:$id ($timeacl, $lim, $dlim, $cur, $fund, $email, $other)" );
		Header ( "Location: index.php?type=edit&id=$id" );
		//	echo "saved";
		@mysql_close ( $link );
		exit ();
		# ------конец критичного куска }
	} # ------------- Нажата кнопка "Удалить" ----------------
	elseif (isset ( $deletebutton )) {
		$result = mysql_query ( "SELECT login FROM users WHERE id=$id", $link );
		$login = mysql_result ( $result, 0, "login" );
		$result = mysql_query ( "update users set status=3 WHERE id=$id", $link );
		logevent ( "Отметил на удаление пользователя $login:$id." );
		Header ( "Location: index.php" );
		@mysql_close ( $link );
		exit ();
	} # ------------- Нажата кнопка "Удалить сейчас" ----------------
	elseif (isset ( $killbutton )) {
		$result = mysql_query ( "SELECT email FROM users WHERE id=$id", $link );
		if (mysql_numrows ( $result ) == 1) {
			list ( $email ) = mysql_result ( $result, 0 );
			$arr = explode ( ",", $email );
			for($i = 0; $i < count ( $arr ); $i ++)
				$result = mysql_query ( "DELETE FROM mail WHERE rcpt='" . addslashes ( $arr [$i] ) . "'", $link );
		}
		
		$result = mysql_query ( "SELECT login FROM users WHERE id=$id", $link );
		$login = mysql_result ( $result, 0, "login" );
		$result = mysql_query ( "DELETE FROM site WHERE u_id=$id", $link );
		$result = mysql_query ( "DELETE FROM users WHERE id=$id", $link );
		logevent ( "Удалил пользователя $login:$id." );
		Header ( "Location: index.php" );
		@mysql_close ( $link );
		exit ();
	}
	;
	//    if ( false ) {
	//        Header("Location: index.php");
	//        @mysql_close($link);
	//        exit;
	//    }
	//    else {
	//	if (isset($loginhidden)) {$login=$loginhidden;};
	//        $result = mysql_query("SELECT * FROM users WHERE id=$id", $link);
	//        if ( mysql_numrows($result) == "0" ) 
	//	{
	//	    echo "$loginhidden not found";
	//            Header("Location: index.php");
	//	    @mysql_close($link);
	//            exit;
	//        }
	//    }
	# -------------- Default-действие для события "edit" ----------------
	$mod_name = $edit_user;
	show_head ();
	show_help ();
	$result = mysql_query ( "SELECT * FROM users WHERE id=$id", $link );
	$login1 = mysql_result ( $result, 0, "login" );
	$login = $login1;
	$cur = mysql_result ( $result, 0, "used" );
	$lim = mysql_result ( $result, 0, "quota" );
	$dlim = mysql_result ( $result, 0, "dquota" );
	$email = mysql_result ( $result, 0, "email" );
	$other = mysql_result ( $result, 0, "descr" );
	$status = mysql_result ( $result, 0, "status" );
	$acl = mysql_result ( $result, 0, "aid" );
	
	if (($auth_mode == 1) && (isset ( $passbutton ))) {
		//    $login    = addslashes(stripslashes(trim($_GET['login'])));
		$pass1 = stripslashes ( $_GET ['pass'] );
		$pass2 = stripslashes ( $_GET ['pass2'] );
		//        # проверка поля "password" ($pass1=$pass2)
	//        if ( $pass1 != $pass2 ) $msg .= "<SPAN CLASS=\"errmsg\">$passwd_noteq</SPAN>\n<BR>\n";
	//        $exc="$bin_prefix/htpasswd -b $ncsa_passwd ".quotemeta($login)." ".quotemeta($pass1);
	//        $data=shell_exec($exc);
	}
	echo "<FORM ACTION=\"index.php\" METHOD=get>
          <INPUT TYPE=hidden NAME=type VALUE=\"edit\">
          <INPUT TYPE=hidden NAME=id VALUE=\"$id\">
          <TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
          <TR>
            <TD><P><B>$view_user</B></P></TD>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=other VALUE=\"$other\"></TD>
          </TR>";
	if ($auth_mode == 1) {
		echo "<TR><TD><B>$view_login</B></TD>
            <TD>$login</TD>
          </TR>
          <INPUT TYPE=hidden NAME=nick VALUE=\"$login\">";
	} else {
		echo "<TR><TD><B>$view_login</B></TD>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=nick VALUE=\"$login\"></TD>
          </TR>";
	}
	echo "<TR>
            <TD><B>$view_traf</B></TD>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=cur VALUE=\"" . dotize ( $cur ) . "\"></TD>
          </TR>
          <TR>
            <TD><B>current $view_quota</B></TD>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=lim VALUE=\"" . dotize ( $lim ) . "\"></TD>
          </TR>
          <TR>
            <TD><P><B>default $view_quota</B></P></TD>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=dlim VALUE=\"" . dotize ( $dlim ) . "\"></TD>
            <TD><SPAN CLASS=\"tahoma11px\">&nbsp;&nbsp;&nbsp;&nbsp;$std_limit (или " . dotize ( $std_limit ) . ")</SPAN></TD>
          </TR>";
	
	if ($megabyte_cost > 0) {
		echo "<TR>
            <TD><B>$place_money</B></TD>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=fund VALUE=\"0\"></TD>
          </TR>";
	}
	;
	echo "<TR>
            <TD><B>$view_email</B></TD>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=email VALUE=\"$email\"></TD>
          </TR>";
	
	if (($auth_mode == 1) && (isset ( $passbutton ))) {
		// ncsa mode
		echo "
          <INPUT TYPE=hidden NAME=passbutton VALUE=\"YEP\">
          <TR>
            <TD><P><B>Password:</B></P></TD>
            <TD><INPUT TYPE=password CLASS=\"input\" NAME=pass VALUE=\"\"></TD>
            <TD><SPAN CLASS=\"tahoma11px\">&nbsp;&nbsp;&nbsp;&nbsp;$passwd_like_1</SPAN></TD>
          </TR>

          <TR>
            <TD><P><B>Password again:</B></P></TD>
            <TD><INPUT TYPE=password CLASS=\"input\" NAME=pass2 VALUE=\"\"></TD>
            <TD><SPAN CLASS=\"tahoma11px\">&nbsp;&nbsp;&nbsp;&nbsp;$passwd_like_2</SPAN></TD>
          </TR>";
	}
	
	echo "<TD>
            <!-- WORKTIME -->
ограничение</td>
<td><select name=acl style=\"font-weight: bold\">";
	$result = mysql_query ( "select vname, id from acl", $link );
	
	for($i = 0; $i < mysql_numrows ( $result ); $i ++) {
		$aid = mysql_result ( $result, $i, "id" );
		$vname = mysql_result ( $result, $i, "vname" );
		echo "<option value=\"$aid\"";
		if ($acl == $aid) {
			echo "selected";
		}
		echo ">$vname";
	}
	echo "</select></td>";
	
	echo "</tr><tr><TD>
статус </td>
<td><select name=status style=\"font-weight: bold\">
<option value=\"0\"";
	if ($status == "0") {
		echo "selected";
	}
	echo ">включен
<option value=\"1\" ";
	if ($status == "1") {
		echo "selected";
	}
	echo ">выключен
<option value=\"2\" ";
	if ($status == "2") {
		echo "selected";
	}
	echo ">заблокирован, разблокировать в конце месяца
<option value=\"3\" ";
	if ($status == "3") {
		echo "selected";
	}
	echo ">заблокирован, удалить в конце месяца
</select></td>";
	echo "</tr>
<tr>
          </TR>
          </TABLE>
          <BR>
          <INPUT TYPE=submit CLASS=\"inputsubmit\" ID=\"bg_green\" NAME=savebutton VALUE=\"$edit_save\">&nbsp;&nbsp;&nbsp;&nbsp;
          <INPUT TYPE=button CLASS=\"inputsubmit\" ID=\"bg_blue\"  VALUE=\"$back_tomain\" onClick=\"window.location='index.php#$id'\">&nbsp;&nbsp;&nbsp;&nbsp;
          <INPUT TYPE=button CLASS=\"inputsubmit\" ID=\"bg_blue\"   NAME=deletebutton VALUE=\"$edit_delete\" OnClick=\"window.location='index.php?type=edit&deletebutton=Удалить&id=$id'; \">
          <INPUT TYPE=button CLASS=\"inputsubmit\" ID=\"bg_red\"   NAME=deletebutton VALUE=\"удалить немедленно\" OnClick=\"if ( confirm('Warning! User &laquo;" . htmlspecialchars ( $other ) . "&raquo; will be removed without an opportunity of restoration!') ) window.location='index.php?type=edit&killbutton=Удалить&id=$id'; \">";
	if ($auth_mode == 1) {
		echo " <INPUT TYPE=submit CLASS=\"inputsubmit\" ID=\"bg_blue\" NAME=passbutton VALUE=\"$Reset_password\" OnClick=\"window.location='index.php?type=edit&passbutton=1&id=$id'; \">&nbsp;&nbsp;&nbsp;&nbsp;";
	}
	echo "</FORM>";
	show_tail ();
	mysql_close ( $link );
	exit ();
} # ------------ Просмотр электронных адресов отправителей писем (событие "email") -------------
elseif ($type == "email") {
	show_head ();
	echo "<A HREF=\"index.php#$login\">&lt;&lt;</A>\n<BR>\n";
	show_info ( $link, $login );
	echo "<HR NOSHADE COLOR=#000000 SIZE=1>\n<BR>\n";
	show_mailstat ( $link, $login );
	show_tail ();
	mysql_close ( $link );
	exit ();
} # ------------ Просмотр адресов посещенных пользователем сайтов (событие "sites") ------------
elseif ($type == "sites") {
	$id = stripslashes ( trim ( $_GET ['id'] ) );
	show_head ();
	echo "<A HREF=\"index.php#$id\">&lt;&lt;</A>\n<BR>\n";
	show_info ( $link, $id );
	echo "<HR NOSHADE COLOR=#000000 SIZE=1>\n<BR>\n";
	show_stat ( $link, $id );
	if (! isset ( $no_mail )) {
		show_mailstat ( $link, $id );
	}
	;
	show_tail ();
	mysql_close ( $link );
	exit ();
} 

# ------------ Просмотр запросов пользователя (событие "detail") ------------
elseif (($type == "detail") and ($detailed == 1)) 

{
	show_head ();
	echo "<A HREF=\"index.php#$id\">&lt;&lt;</A>\n<BR>\n";
	show_info ( $link, $id );
	echo "<HR NOSHADE COLOR=#000000 SIZE=1>\n<BR>\n";
	show_detail ( $link, $_GET ['id'], $_GET ['site'] );
	show_tail ();
	mysql_close ( $link );
	exit ();
} # ------------ RSH stat view ----------------------------------------------------------
elseif ($type == "rsh") {
	$total = 0;
	$totalp = 0;
	show_head ();
	echo "<H1>RSH cisco stat</H1>\n";
	show_help ();
	echo "<center><TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0><TR><TD BGCOLOR=#93BEE2>";
	echo "<TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1><TR VALIGN=TOP ALIGN=LEFT><TD HEIGHT=18>";
	echo "destination</td><td ALIGN=LEFT>packets</td><td ALIGN=LEFT>bytes</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
	$clink = db_connect_ex ( "cisco" );
	
	$result = mysql_query ( "select distinct dst FROM traffic", $clink );
	for($i = 0; $i < mysql_numrows ( $result ); $i ++) {
		$dst = mysql_result ( $result, $i, "dst" );
		$result2 = mysql_query ( "select sum(pckts) as pkt, sum(bytes) as byt FROM traffic where dst=$dst", $clink );
		$pkts = dotize ( mysql_result ( $result2, 0, "pkt" ) );
		$bytes = dotize ( mysql_result ( $result2, 0, "byt" ) );
		$ip = long2ip ( $dst );
		echo "<a href=\"?type=rsh_ip&dst=$dst\">$ip</a></td><td ALIGN=RIGHT>$pkts</td><td ALIGN=RIGHT>$bytes</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
		$total += mysql_result ( $result2, 0, "byt" );
		$totalp += mysql_result ( $result2, 0, "pkt" );
	}
	$totalp = dotize ( $totalp );
	$total = dotize ( $total );
	echo "_TOTAL</td><td ALIGN=RIGHT>$totalp</td><td ALIGN=RIGHT>$total</td></tr>";
	echo "</table></TD></TR></TABLE><br>";
	@mysql_close ( $clink );
	exit ();
} # ---------------------------- RSH stat for IP ------------------------------------------
elseif ($type == "rsh_ip") {
	$dst = ( int ) $_GET ['dst'];
	
	$page = ( int ) $_GET ['page'];
	// ----------------------------
	$pagestep = 1000;
	//-----------------------------
	$ip = long2ip ( $dst );
	show_head ();
	echo "<H1>RSH stat for $ip</H1>\n";
	show_help ();
	echo "<center>";
	//echo "select sum(pckts) as pkt, sum(bytes) as byt FROM traffic where src=$src and dst=$dst";
	$clink = db_connect_ex ( "cisco" );
	
	$cquery = $_SERVER ['PHP_SELF'] . "?" . $_SERVER ['QUERY_STRING'];
	
	echo "<br><center><p style=\"font-size: 12\">";
	$result = mysql_query ( "select count(src) as rec FROM traffic where dst=$dst", $clink );
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
	
	echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0><TR><TD BGCOLOR=#93BEE2>";
	echo "<TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1><TR VALIGN=TOP ALIGN=LEFT><TD HEIGHT=18>";
	echo "src</td><td ALIGN=LEFT>packets</td><td ALIGN=LEFT>bytes</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
	
	$result = mysql_query ( "select distinct src, bytes FROM traffic where dst=$dst order by bytes desc limit " . $page * $pagestep . "," . $pagestep, $clink );
	//LIMIT 5000,100
	for($i = 0; $i < mysql_numrows ( $result ); $i ++) {
		$src = mysql_result ( $result, $i, "src" );
		$ip = long2ip ( $src );
		$result2 = mysql_query ( "select sum(pckts) as pkt, sum(bytes) as byt FROM traffic where src=$src and dst=$dst order by byt", $clink );
		$pkts = dotize ( mysql_result ( $result2, 0, "pkt" ) );
		$bytes = dotize ( mysql_result ( $result2, 0, "byt" ) );
		//	$ip=long2ip($src);
		echo "<a href=\"?type=rsh_detail&dst=$dst&src=$src\">$ip</a></td><td ALIGN=RIGHT>$pkts</td><td ALIGN=RIGHT>$bytes</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
	}
	echo "</td></tr>";
	echo "</table></TD></TR></TABLE><br>";
	
	@mysql_close ( $clink );
	exit ();
} # ---------------------------- RSH stat for pair ------------------------------------------
elseif ($type == "rsh_detail") {
	show_head ();
	echo "<h1>Не реализовано</h1>";
	exit ();
} # ---------------------------- daily stat, level 1-----------------------------------------
elseif ($type == "day1") {
	include_once ("../inc/bydate.php");
	show_bydays ( $link );
	exit ();
} # ---------------------------- daily stat, level 2-----------------------------------------
elseif ($type == "day2") {
	$t1 = $_GET ['t1'];
	$t2 = $_GET ['t2'];
	include_once ("../inc/bydate.php");
	show_byday2 ( $t1, $t2, $link );
	exit ();
} 

# ------------ Просмотр эффективности (событие "perf") ------------
elseif ($type == "perf") {
	$mod_name = $web_admin_perf;
	show_head ();
	//    echo "<A HREF=\"index.php\">&lt;&lt;</A>";
	show_help ();
	echo "<HR NOSHADE COLOR=#000000 SIZE=1>\n<BR>\n";
	echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
      <TR><TD BGCOLOR=#93BEE2>
      <TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1>
         <TR VALIGN=TOP ALIGN=CENTER>
            <TD HEIGHT=15>$web_admin_name</TD>
            <TD><B>$web_admin_hit_percent</B></TD>
            <TD><B>$web_admin_bytes_percent</B></TD>
            <TD><B>$web_admin_count</B></TD>
            <TD><B>$web_admin_total</B></TD>
            <TD><B>$word_byte</B></TD>
            <TD><B>$web_admin_total</B></TD>
            </TR>";
	//
	$result = mysql_query ( "select max(utime) as _max, min(utime) as _min, round((max(utime)-min(utime))/(60*60*24)) as _time from detail;", $link );
	$min = @mysql_result ( $result, 0, "_min" );
	$max = @mysql_result ( $result, 0, "_max" );
	$days = @mysql_result ( $result, 0, "_time" );
	$cmin = $min - 1;
	
	for($i = 0; $i < $days; $i ++) {
		$cmax = $cmin + (24 * 60 * 60);
		$result = mysql_query ( "SELECT sum(size) as _totalmb, count(size) as _total from detail where utime<$cmax and utime>$cmin;", $link );
		$total = @mysql_result ( $result, 0, "_total" );
		$totalmb = @mysql_result ( $result, 0, "_totalmb" );
		$result = mysql_query ( "SELECT count(size) as _hits, sum(size) as _hitsmb FROM detail where code like('%HIT%') and utime<$cmax and utime>$cmin;", $link );
		$hits = @mysql_result ( $result, 0, "_hits" );
		$hitsmb = @mysql_result ( $result, 0, "_hitsmb" );
		if (0 < $totalmb) {
			$hitsmbp = round ( ($hitsmb / $totalmb) * 100, 2 );
		}
		if (0 < $total) {
			$hitsp = round ( ($hits / $total) * 100, 2 );
		}
		;
		echo "<TR BGCOLOR=#FFF7E5 ALIGN=RIGHT>
	<td ALIGN=LEFT>" . strftime ( "%B %d, %T", ( int ) $cmin ) . "-" . strftime ( "%B %d, %T", ( int ) $cmax ) . "</td>
	<td>$hitsp</td>
	<td>$hitsmbp</td>
	<td>" . dotize ( $hits ) . "</td>
	<td>" . dotize ( $total ) . "</td>
	<td>" . dotize ( $hitsmb ) . "</td>
	<td>" . dotize ( $totalmb ) . "</td>
	</tr>";
		
		$cmin = $cmax;
	}
	
	$result = mysql_query ( "SELECT sum(size) as _totalmb, count(size) as _total from detail;", $link );
	$total = @mysql_result ( $result, 0, "_total" );
	$totalmb = @mysql_result ( $result, 0, "_totalmb" );
	$result = mysql_query ( "SELECT count(size) as _hits, sum(size) as _hitsmb FROM detail where code like('%HIT%');", $link );
	$hits = @mysql_result ( $result, 0, "_hits" );
	$hitsmb = @mysql_result ( $result, 0, "_hitsmb" );
	$hitsmbp = round ( ($hitsmb / $totalmb) * 100, 2 );
	$hitsp = @round ( ($hits / $total) * 100, 2 );
	echo "<TR BGCOLOR=#FFF7E5 ALIGN=RIGHT>
	<td ALIGN=LEFT>total (" . strftime ( "%B %d, %T", ( int ) $min ) . "-" . strftime ( "%B %d, %T", ( int ) $max ) . ")</td>
	<td>$hitsp</td>
	<td>$hitsmbp</td>
	<td>" . dotize ( $hits ) . "</td>
	<td>" . dotize ( $total ) . "</td>
	<td>" . dotize ( $hitsmb ) . "</td>
	<td>" . dotize ( $totalmb ) . "</td>
	</tr>";
	
	echo "</TABLE></TD></TR></TABLE><br>";
	show_tail ();
	mysql_close ( $link );
	exit ();
} # ------------ Просмотр истории (событие "uhist") ------------
elseif ($type == "uhist") {
	$mod_name = " users history for" . strftime ( "%B %d, %T", ( int ) $utime );
	show_head ();
	//    echo "<A HREF=\"index.php\">&lt;&lt;</A>";
	show_help ();
	
	echo "<BR>";
	//$order=(int)$settings["order_uhist"];
	switch ($settings ["order_uhist"]) {
		case 4 :
			{
				$order = 'login';
				break;
			}
			;
		case 0 :
			{
				$order = 'descr';
				break;
			}
			;
		case 1 :
			{
				$order = 'used desc';
				break;
			}
			;
		case 2 :
			{
				$order = 'quota desc';
				break;
			}
			;
		case 5 :
			{
				$order = 'utime';
				break;
			}
			;
	}
	$ORDER_BY = "";
	if (isset ( $st )) {
		if (($st == "descr") or ($st == "used") or ($st == "quota") or ($st == "login"))
			$ORDER_BY = "ORDER BY $st";
		
		if (($st == "used") or ($st == "quota"))
			$ORDER_BY .= " DESC";
	} else {
		$ORDER_BY = "ORDER BY $order";
		$st = "login";
	}
	$result = mysql_query ( "SELECT * FROM uhistory where utime=$utime $ORDER_BY", $link );
	echo mysql_error ( $link );
	//print $ORDER_BY;
	echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
      <TR>
        <TD BGCOLOR=#93BEE2>
        <TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1>
         <TR VALIGN=TOP ALIGN=CENTER>
            <TD HEIGHT=30><A HREF=\"index.php?mode=uhist&utime=$utime&st=descr\"><B>$web_admin_descr</B></A></TD>
            <TD><A HREF=\"index.php?mode=uhist&utime=$utime&st=used\"><B>$web_admin_traf</B></A></TD>
            <TD><A HREF=\"index.php?mode=uhist&utime=$utime&st=quota\"><B>$web_admin_quota</B></A></TD>
            <TD><A HREF=\"index.php?mode=uhist&utime=$utime&st=login\"><B>$web_admin_login</B></A></TD>
	    <TD><B>$web_status</B></TD>
	 </TR>";
	
	for($i = 0; $i < mysql_numrows ( $result ); $i ++) {
		$login = mysql_result ( $result, $i, "login" );
		$cur = mysql_result ( $result, $i, "used" );
		//    $cur = $cur -1;
		$lim = mysql_result ( $result, $i, "quota" );
		$descr = mysql_result ( $result, $i, "descr" );
		$id = mysql_result ( $result, $i, "id" );
		//    $login= $arr[$descr][login];
		echo "
	<TR BGCOLOR=#";
		
		if (($lim == "0") && ($settings ["uhist_ch"] == 1))
			echo "ccffcc";
		elseif (($cur > $lim) && ($settings ["uhist_ch"] == 1))
			echo "ffcccc";
		else
			echo "FFF7E5";
		echo ">
            <TD><A NAME=\"" . $id . "\">
            </A>$descr</TD>
            <TD ALIGN=RIGHT><A HREF=\"index.php?type=shist&id=$id\">" . dotize ( $cur ) . "</A></TD>
            <TD ALIGN=RIGHT>" . dotize ( $lim ) . "</TD>
            <TD>$login</TD>
	    <TD>";
		
		if ($lim == "0") {
			echo "Анлим.";
		}
		;
		if ($lim != "0")
			if ($cur > $lim) {
				echo "$web_status_negative";
			}
		;
		echo "</TR>";
	}
	
	$result = mysql_query ( "SELECT sum(used) as slim FROM uhistory where utime=$utime", $link );
	$lim = mysql_result ( $result, 0, "slim" );
	$msum = 0;
	$result = mysql_query ( "SELECT SUM(quota) as squota FROM uhistory where utime=$utime", $link );
	$slim = @mysql_result ( $result, 0, "squota" );
	// Мужик по фамилии "_Итого:"
	echo "<TR BGCOLOR=#FFF7E5><TD><A NAME=\"$word_total\">$web_admin_total $i </A></TD>
            <TD ALIGN=RIGHT>" . dotize ( $lim ) . "</TD>
            <TD ALIGN=RIGHT>" . dotize ( $slim ) . "</TD>
            <TD></TD>
            <TD></TD>
            </TR>";
	echo "</TABLE></TD>
</TR>
</TABLE>";
	
	echo "</TABLE></TD>
</TR>
</TABLE><br>";
	
	show_tail ();
	mysql_close ( $link );
	exit ();
} # ------------ Просмотр оглавления истории (событие "hist") ------------
elseif ($type == "hist") {
	$mod_name = $web_admin_header_viewhist;
	show_head ();
	show_help ();
	echo "<HR NOSHADE COLOR=#000000 SIZE=1>\n<BR>\n";
	$result = mysql_query ( "SELECT distinct utime as utime FROM uhistory order by utime desc;", $link );
	echo mysql_error ( $link );
	echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
      <TR><TD BGCOLOR=#93BEE2>
      <TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1>
         <TR VALIGN=TOP ALIGN=CENTER>
            <TD HEIGHT=15><B>$web_admin_number</B></TD>
            <TD><B>$web_admin_date</B></TD></TR>";
	for($i = 0; $i < mysql_numrows ( $result ); $i ++) {
		$utime = @mysql_result ( $result, $i, "utime" );
		# Выводится содержимое таблицы
		echo "<TR BGCOLOR=#FFF7E5><TD>$i</TD><TD><a href=index.php?type=uhist&utime=$utime>" . strftime ( "%B %d, %T", ( int ) $utime ) . "</a></TD></TR>";
	}
	echo "</TABLE></TD></TR></TABLE><br>";
	show_tail ();
	mysql_close ( $link );
	exit ();
} 

# ------------ Просмотр eventlog (событие "event") ------------
elseif ($type == "event") {
	$mod_name = $web_admin_header_viewlog;
	show_head ();
	//    echo "<A HREF=\"index.php\">&lt;&lt;</A>";
	show_help ();
	echo "<HR NOSHADE COLOR=#000000 SIZE=1>\n<BR>\n";
	$result = mysql_query ( "SELECT * FROM syslog order by id", $link );
	//echo mysql_error($link);
	echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
      <TR><TD BGCOLOR=#93BEE2>
      <TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1>
         <TR VALIGN=TOP ALIGN=CENTER>
            <TD HEIGHT=15><B>$web_admin_number</B></TD>
            <TD><B>$web_admin_description</B></TD></TR>";
	for($i = 0; $i < @mysql_numrows ( $result ); $i ++) {
		$ev_id = @mysql_result ( $result, $i, "id" );
		$ev_data = @mysql_result ( $result, $i, "record" );
		# Выводится содержимое таблицы
		echo "<TR BGCOLOR=#FFF7E5><TD>$ev_id</TD><TD>$ev_data</TD></TR>";
	}
	echo "</TABLE></TD></TR></TABLE><br>";
	show_tail ();
	mysql_close ( $link );
	exit ();
} # ------------ Просмотр заданий (событие "queue") ------------
elseif ($type == "queue") {
	$mod_name = "commands log";
	show_head ();
	show_help ();
	echo "Commands in this context:<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=queue\">refresh</A>;<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=queue&mode=clean\">clear queue</A>;<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=queue&mode=add&cmd=0\">test</A>;<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=queue&mode=add&cmd=1\">reconfigure</A>;<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=queue&mode=add&cmd=2\">recount</A>;<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=queue&mode=add&cmd=3\">mothly job</A>;<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=queue&mode=mark\">mark all as done</A>;<BR>";
	$mode = $_GET ['mode'];
	if ($mode == "add") {
		$cmd = ( int ) $_GET ['cmd'];
		mysql_query ( "insert into queue (command, itime) values ($cmd, " . time () . ")", $link );
		echo "inserted " . mysql_affected_rows () . " row<br> Please wait until job completes.";
	
	} elseif ($mode == "clean") {
		mysql_query ( "truncate table queue", $link );
		echo "affected " . mysql_affected_rows () . "";
	
	} elseif ($mode == "mark") {
		mysql_query ( "update queue set complete=5, rtime=" . time () . " where complete=0", $link );
		echo "affected " . mysql_affected_rows () . " rows";
	}
	;
	
	echo "<HR NOSHADE COLOR=#000000 SIZE=1>\n<BR>\n";
	$result = mysql_query ( "SELECT * FROM queue order by id", $link );
	//echo mysql_error($link);
	echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
      <TR><TD BGCOLOR=#93BEE2>
      <TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1>
         <TR VALIGN=TOP ALIGN=CENTER>
            <TD HEIGHT=15><B>$web_admin_number</B></TD>
            <TD><B>$web_admin_description</B></TD></TR>";
	for($i = 0; $i < @mysql_numrows ( $result ); $i ++) {
		$q_id = @mysql_result ( $result, $i, "id" );
		$q_cmd = @mysql_result ( $result, $i, "command" );
		$q_st = @mysql_result ( $result, $i, "complete" );
		$i_time = @mysql_result ( $result, $i, "itime" );
		$e_time = @mysql_result ( $result, $i, "rtime" );
		
		if (0 == $q_st) {
			$status = "waiting";
		} elseif (1 == $q_st) {
			$status = "processing";
		} elseif (2 == $q_st) {
			$status = "complete";
		} elseif (3 == $q_st) {
			$status = "done with errors";
		} elseif (4 == $q_st) {
			$status = "error";
		} elseif (5 == $q_st) {
			$status = "skipped";
		}
		;
		if (0 == $q_cmd) {
			$cmd = "test";
		} elseif (1 == $q_cmd) {
			$cmd = "reconfigure";
		} elseif (2 == $q_cmd) {
			$cmd = "recount";
		} elseif (3 == $q_cmd) {
			$cmd = "monthly job";
		}
		;
		$stime = strftime ( "%Y %B %d, %T", ( int ) $i_time );
		if (0 == $e_time) {
			$etime = "never";
		} else {
			$etime = strftime ( "%B %d, %T", ( int ) $e_time );
		}
		;
		
		# Выводится содержимое таблицы
		echo "<TR BGCOLOR=#FFF7E5><TD>$q_id</TD><TD>command \"$cmd\" ($q_cmd) status \"$status\" ($q_st), inserted $stime seen by daemon $etime </TD></TR>";
	}
	echo "</TABLE></TD></TR></TABLE><br>";
	show_tail ();
	mysql_close ( $link );
	exit ();
} # ------------ Просмотр аццесс контрол листов (событие "acl") ------------
elseif ($type == "acl") {
	show_head ();
	show_help ();
	echo "<HR NOSHADE COLOR=#000000 SIZE=1>\n<BR>\n";
	show_acl ( $link );
	show_tail ();
	mysql_close ( $link );
	exit ();
} # ------------ Редактирование аццесс контрол листов (событие "acled") ------------
elseif ($type == "acled") {
	# ------------- Нажата кнопка "Сохранить" ----------------
	if (isset ( $savebutton )) {
		$id = ( int ) stripslashes ( trim ( $id ) );
		$vname = stripslashes ( trim ( $vname ) );
		$sysname = stripslashes ( trim ( $sysname ) );
		$data = stripslashes ( trim ( $data ) );
		if (0 == $id) {
			$result = mysql_query ( "update acl set vname='$vname', data='$data', sysname='$sysname' where id=$id", $link );
			if (0 == mysql_affected_rows ( $link )) {
				echo "ERROR: " . mysql_error ( $link ) . " please contact your support";
				exit (); // write error and quit
			}
			;
		} else {
			$result = mysql_query ( "insert into acl (vname , data, sysname) values ('$vname', '$data', '$sysname' );", $link );
			if (0 == mysql_insert_id ( $link )) {
				echo "ERROR: " . mysql_error ( $link ) . " please contact your support";
				exit (); // write error and quit
			}
			;
		}
		;
		Header ( "Location: index.php?type=acl" );
		exit ();
	}
	;
	show_head ();
	show_help ();
	
	echo "<FORM ACTION=\"index.php\" METHOD=get>
          <INPUT TYPE=hidden NAME=type VALUE=\"acled\">
          <INPUT TYPE=hidden NAME=id VALUE=\"$id\">
          <TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>";
	if ($id > 0) {
		$result = mysql_query ( "SELECT * FROM acl where id=$id;", $link );
		$sysname = mysql_result ( $result, 0, "sysname" );
		$vname = mysql_result ( $result, 0, "vname" );
		$data = mysql_result ( $result, 0, "data" );
	
	}
	;
	echo "<TR>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=sysname VALUE=\"$sysname\"></TD></tr><tr>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=vname VALUE=\"$vname\"></TD></tr><tr>
            <TD>
            <textarea wrap=soft name=\"data\" rows=10 cols=60 wrap=\"physical\" tabindex=6>$data</textarea>

            <TD>$help</TD>
          </TR>";
	echo "</TABLE>
	<INPUT TYPE=submit CLASS=\"inputsubmit\" ID=\"bg_green\" NAME=savebutton VALUE=\"$edit_save\">&nbsp;&nbsp;&nbsp;&nbsp;
	<INPUT TYPE=button CLASS=\"inputsubmit\" ID=\"bg_blue\"  VALUE=\"$back_tomain\" onClick=\"window.location='index.php'\">&nbsp;&nbsp;&nbsp;&nbsp;";
	show_tail ();
	exit ( 0 );

}

# ------------- Default-действие при отсутствии событий ----------------
show_head ();
show_help ();
echo "<BR>";
//$order=(int)$settings["order_main"];
switch ($settings ["order_main"]) {
	case 4 :
		{
			$order = 'login';
			break;
		}
		;
	case 0 :
		{
			$order = 'descr';
			break;
		}
		;
	case 1 :
		{
			$order = 'used desc';
			break;
		}
		;
	case 2 :
		{
			$order = 'quota desc';
			break;
		}
		;
	case 3 :
		{
			$order = 'timeacl';
			break;
		}
		;
	case 5 :
		{
			$order = 'email';
			break;
		}
		;
	case 6 :
		{
			$order = 'status';
			break;
		}
		;
}
$ORDER_BY = "";
if (isset ( $st )) {
	if (($st == "descr") or ($st == "used") or ($st == "quota") or ($st == "login") or ($st == "email") or ($st == "timeacl"))
		$ORDER_BY = "ORDER BY $st";
	
	if (($st == "used") or ($st == "quota"))
		$ORDER_BY .= " DESC";
} else {
	$ORDER_BY = "ORDER BY $order";
	$st = "login";
}
# Выборка всей информации по пользователям из БД squid (таблица stat)
$result = mysql_query ( "SELECT login, used, users.descr as descr, quota, email, users.id as id, status, acl.vname as timeacl FROM users, acl where acl.id=users.aid $ORDER_BY", $link );
echo mysql_error ( $link );
//print $ORDER_BY;
echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
      <TR>
        <TD BGCOLOR=#93BEE2>
        <TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1>
         <TR VALIGN=TOP ALIGN=CENTER>
            <TD HEIGHT=30><A HREF=\"index.php?st=descr\"><B>$web_admin_descr</B></A></TD>
            <TD><A HREF=\"index.php?st=used\"><B>$web_admin_traf</B></A></TD>
            <TD><A HREF=\"index.php?st=quota\"><B>$web_admin_quota</B></A></TD>
            <TD><A HREF=\"index.php?st=timeacl\"><B>$web_admin_tacl</B></A></TD>
            <TD><A HREF=\"index.php?st=login\"><B>$web_admin_login</B></A></TD>
            <TD><A HREF=\"index.php?st=email\"><B>$view_email</B></A></TD>";
if (! isset ( $no_mail )) {
	echo "<TD><A HREF=\"index.php?st=\"><B>$web_admin_mailc</B></A></TD>";
}
;
echo "<TD><A HREF=\"index.php?st=\"><B>$web_status</B></A></TD></TR>";

for($i = 0; $i < mysql_numrows ( $result ); $i ++) {
	$login = mysql_result ( $result, $i, "login" );
	$cur = mysql_result ( $result, $i, "used" );
	//    $cur = $cur -1;
	$lim = mysql_result ( $result, $i, "quota" );
	$email = mysql_result ( $result, $i, "email" );
	$descr = mysql_result ( $result, $i, "descr" );
	$timeacl = mysql_result ( $result, $i, "timeacl" );
	$id = mysql_result ( $result, $i, "id" );
	$status = mysql_result ( $result, $i, "status" );
	//    $login= $arr[$descr][login];
	echo "<TR BGCOLOR=#";
	if ((0 < ( int ) $status) && ($settings ["main_ch"] == 1))
		echo "cccccc";
	elseif (($lim == "0") && ($settings ["main_ch"] == 1))
		echo "ccffcc";
	elseif (($cur > $lim) && ($settings ["main_ch"] == 1))
		echo "ffcccc";
	
	else
		echo "FFF7E5";
	
	echo ">
            <TD><A NAME=\"" . $id . "\">
            </A><A HREF=\"index.php?type=edit&id=$id\">$descr</A></TD>
            <TD ALIGN=RIGHT><A HREF=\"index.php?type=sites&id=$id\">" . dotize ( $cur ) . "</A></TD>
            <TD ALIGN=RIGHT>" . dotize ( $lim ) . "</TD>
            <TD ALIGN=CENTER>$timeacl</TD>
            <TD>$login</TD>
            <TD><a ";
	if (! isset ( $no_mail )) {
		echo "HREF=\"index.php?type=email&login=$login\" ";
	}
	;
	echo "TITLE=\"" . $email . "\">";
	if (strlen ( $email ) > 25)
		echo substr ( $email, 0, 25 ) . "...";
	else
		echo $email;
	echo "</A></TD><TD align=right>";
	if (! isset ( $no_mail )) {
		$rcpt = "rcpt='" . str_replace ( ",", "' OR rcpt='", $email ) . "'";
		$email = str_replace ( ",", ", ", $email );
		$result = mysql_query ( "SELECT SUM(size) FROM mail WHERE $rcpt", $link );
		$msum = @mysql_result ( $result, 0 );
		$msum = dotize ( $msum );
		echo "$msum</TD><TD>";
	}
	;
	if ("2" == $status) {
		echo "blocked";
	} elseif ("1" == $status) {
		echo "disabled";
	} elseif ("3" == $status) {
		echo "for remove";
	} elseif ($lim == "0") {
		echo "Анлим.";
	} elseif ($cur > $lim) {
		echo "$web_status_negative";
	}
	;
	
	echo "</TR>";
}

$result = mysql_query ( "SELECT sum(used) as slim FROM users", $link );
$lim = mysql_result ( $result, 0, "slim" );
$msum = 0;
if (! isset ( $no_mail )) {
	$result = mysql_query ( "SELECT SUM(size) as ssize FROM mail", $link );
	$msum = @mysql_result ( $result, 0, "ssize" );
}
$result = mysql_query ( "SELECT SUM(quota) as squota, count(quota) as _total FROM users", $link );
$slim = @mysql_result ( $result, 0, "squota" );
$_total = @mysql_result ( $result, 0, "_total" );
$result = mysql_query ( "SELECT offset FROM sys_trf", $link );
$sys_off = @mysql_result ( $result, 0, "offset" );
// Мужик по фамилии "_Итого:"
echo "<TR BGCOLOR=#FFF7E5><TD><A NAME=\"$word_total\">$web_admin_total $i ($_total)</A></TD>
            <TD ALIGN=RIGHT>" . dotize ( $lim ) . "</TD>
            <TD ALIGN=RIGHT>" . dotize ( $slim ) . "</TD>
            <TD></TD>
            <TD></TD>
            <TD></TD>
            <TD align=right>" . dotize ( $msum ) . "</TD>
            </TR>";

echo "</TABLE></TD>
</TR>
</TABLE><br>";

echo "</TABLE></TD>
</TR>
</TABLE><br>";
clearstatcache () . $fsize = filesize ( $logdir );
echo " $web_admin_logfree_space " . ( int ) (disk_free_space ( "$logdir" ) / (1024 * 1024)) . " Mb. log offset: $sys_off log size: $fsize  ";
show_tail ();
@mysql_close ( $link );
?>
