<?php
//register_globals
ini_set ( "register_globals", "1" );
require ("../inc/version.php");
require ("../admin/functions.php");
require_once ("../inc/auth.php");
require ("../inc/functions.php");
require ("../inc/mysql.php");
require_once ("../inc/xmlarr.php");

$mod_name = $sa_title;
$mode = "sa";

# ----------------- Аутентификация ---------------------
if (isset ( $PHP_AUTH_USER )) {
	$ok = false;
//	if (auth_adm ( $PHP_AUTH_USER, $PHP_AUTH_PW )) {
		$ok = true;
//	}
	if (! $ok) {
		if (isset ( $try )) {
			Header ( "WWW-Authenticate: Basic realm=\"$sa_auth_realm\"" );
			Header ( "HTTP/1.0 401 Unauthorized" );
		}
		show_head ();
		show_error ();
		//        echo "$dat1  <br>";
		echo "$PHP_AUTH_USER, $PHP_AUTH_PW" . md5 ( $PHP_AUTH_PW ) . "";
		show_tail ();
		@mysql_close ( $link );
		exit ();
	}
} else {
	Header ( "WWW-Authenticate: Basic realm=\"$sa_auth_realm\"" );
	Header ( "HTTP/1.0 401 Unauthorized" );
	show_head ();
	show_error ();
	show_tail ();
	exit ();
}
# --------------------- MAIN --------------------
$link = db_connect ();
$login = stripslashes ( trim ( $_GET ['login'] ) );
$group = "admins";
# -------------------- Создание нового пользователя (событие "create") -----------------------
if ($type == "create") {
	$msg = "";
	$pass1 = stripslashes ( $_GET ['pass'] );
	$pass2 = stripslashes ( $_GET ['pass2'] );
	$descr = stripslashes ( $_GET ['descr'] );
	
	# ---------------- Нажата кнопка "Сохранить" ("createbutton") -----------------
	if (isset ( $createbutton )) {
		# проверка поля login
		if ($login == "")
			$msg .= "<SPAN CLASS=\"errmsg\">$login_blank</SPAN>\n<BR>\n";
		else {
			$result = mysql_query ( "SELECT * FROM admins WHERE login='$login'", $link );
			if (mysql_numrows ( $result ) != 0)
				$msg .= "<SPAN CLASS=\"errmsg\">$user_exist</SPAN>\n<BR>\n";
		}
		# проверка поля "password" ($pass1=$pass2)
		if ($pass1 != $pass2)
			$msg .= "<SPAN CLASS=\"errmsg\">$passwd_noteq</SPAN>\n<BR>\n";
			# ошибок не обнаружилось
		if ($msg == "") {
			$result = mysql_query ( "INSERT INTO admins (login, passwd, descr) VALUES('$login', md5('$pass'), '$descr')", $link );
			if (mysql_affected_rows () != "0") {
				Header ( "Location: index.php?type=edit&login=$login" );
				logevent ( "create SA $login, $descr" );
				@mysql_close ( $link );
				exit ();
			}
		} else { #--- информация не занесена в БД по к.-л. причинам ---
			// Какая то ботва произошла похоже...
			show_head ();
			echo "<H1>$add_user_fail</H1>\n<HR NOSHADE COLOR=#000000 SIZE=1>\n";
			echo "<FORM METHOD=get ACTION=index.php>
                        <INPUT TYPE=hidden NAME=type VALUE=edit>
                        <INPUT TYPE=hidden NAME=descr VALUE=\"$descr\">
                        <INPUT TYPE=hidden NAME=login VALUE=\"$login\">
                        <INPUT TYPE=button CLASS=\"inputsubmit\" ID=\"bg_blue\" VALUE=\"$back_tomain\" OnClick=\"window.location = 'index.php'\">&nbsp;&nbsp;&nbsp;&nbsp;
                        <INPUT TYPE=button CLASS=\"inputsubmit\" ID=\"bg_blue\" VALUE=\"$crt_newuser\" OnClick=\"window.location = 'index.php?type=create'\">
                      </FORM>\n";
			show_tail ();
			@mysql_close ( $link );
			exit ();
		}
	}
	
	# -------------- Default-действие для события "create" -----------------
	$mod_name = $new_sa_creation_header;
	show_head ();
	echo "<P><P><P>
$web_admin_header_influences:<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=create\">$new_sa_creation_header</A>;<BR>
 &#151;&nbsp;<A HREF=\"../admin/\">$admin_title</A>;<BR>
 &#151;&nbsp;<A HREF=\"../admin/index.php?type=event\">$web_admin_header_viewlog</A>;<BR>
 &#151;&nbsp;<A HREF=\"check.php\">Check environment configuration</A>;<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=db2xml\">export</A>;<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=xml2db\">import</A>;<BR><BR>
";
	
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
            <TD><P><B>Description:</B></P></TD>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=descr VALUE=\"" . htmlspecialchars ( $descr ) . "\"></TD>
            <TD><SPAN CLASS=\"tahoma11px\">&nbsp;&nbsp;&nbsp;&nbsp;Description</SPAN></TD>
          </TR>
          <TR>
            <TD><P><B>Login:</B></P></TD>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=login VALUE=\"" . htmlspecialchars ( $login ) . "\"></TD>
            <TD><SPAN CLASS=\"tahoma11px\">&nbsp;&nbsp;&nbsp;&nbsp;$login_like</SPAN></TD>
          </TR>";
	echo "<TR>
            <TD><P><B>Password:</B></P></TD>
            <TD><INPUT TYPE=password CLASS=\"input\" NAME=pass VALUE=\"\"></TD>
            <TD><SPAN CLASS=\"tahoma11px\">&nbsp;&nbsp;&nbsp;&nbsp;$passwd_like_1</SPAN></TD>
          </TR>

          <TR>
            <TD><P><B>Pass again:</B></P></TD>
            <TD><INPUT TYPE=password CLASS=\"input\" NAME=pass2 VALUE=\"\"></TD>
            <TD><SPAN CLASS=\"tahoma11px\">&nbsp;&nbsp;&nbsp;&nbsp;$passwd_like_2</SPAN></TD>
          </TR>
          </TABLE>
          <BR>
            <INPUT TYPE=submit CLASS=\"inputsubmit\" ID=\"bg_green\" NAME=createbutton VALUE=\"$crt_newuser\">&nbsp;&nbsp;&nbsp;&nbsp;
            <INPUT TYPE=button CLASS=\"inputsubmit\" ID=\"bg_blue\"  VALUE=\"$back_tomain\" OnClick=\"window.location = 'index.php'\">&nbsp;&nbsp;&nbsp;&nbsp;
            <INPUT TYPE=button CLASS=\"inputsubmit\" ID=\"bg_pink\"  VALUE=\"$clear_form\" onClick=\"forms[0].elements[0].value=''; forms[0].elements[1].value=''; forms[0].elements[2].value=''; forms[0].elements[3].value='@prognoz.ru'; forms[0].elements[4].value=0; forms[0].elements[5].value=0; forms[0].elements[6].value=''; \">    
            <INPUT TYPE=hidden NAME=type VALUE=\"create\">
        </FORM>\n";
	show_tail ();
	@mysql_close ( $link );
	exit ();
} 
# ------------ экспорт пользователей
elseif ($type== "db2xml")
{
	# TODO xmlexport and import
	$req="SELECT login, used, users.descr as vname, quota, email, users.id as id, status, acl.sysname as timeacl FROM users, acl where acl.id=users.aid";
	$result = mysql_query ( $req);
	echo mysql_error ( $link );
	$j=0;
for($i = 0; $i < mysql_numrows ( $result ); $i ++) 
{
	$login = mysql_result ( $result, $i, "login" );
	$cur = mysql_result ( $result, $i, "used" );
	$lim = mysql_result ( $result, $i, "quota" );
	$email = mysql_result ( $result, $i, "email" );
	$vname = mysql_result ( $result, $i, "vname" );
	$timeacl = mysql_result ( $result, $i, "timeacl" );
	$id = mysql_result ( $result, $i, "id" );
	$status = mysql_result ( $result, $i, "status" );
	$j++;
	$arr[$j] = array("uid" => $id, "used" => $cur, "lim" => $lim, "email" => $email, "login" => $login, "timeacl" => $timeacl, "vname" => $vname);
}
	$xmlwr = new CArray2xml2array();
    $xmlwr->setArray($arr);
    $tmpFile = "/tmp/sacc1.xml";
    $xmlwr->saveArray($tmpFile, "config", "UTF-8");
	$fh = fopen($tmpFile, 'r');
	$theData = fread($fh, filesize($tmpFile));
	$theData=iconv("KOI8-R", "UTF-8", $theData);
	fclose($fh);
	unlink($tmpFile);
	header('Content-Disposition: attachment; filename=sacc-export.xml');
	echo $theData;
	exit;
}

# ------------ импорт пользователей
elseif ($type== "xml2db")
{
	show_head();
	echo "<form enctype=\"multipart/form-data\" action=\"index.php?type=xmlimp\" method=\"post\">
 <input type=\"hidden\" name=\"MAX_FILE_SIZE\" value=\"3000000\" />
 файл xml для импорта: <input name=\"userfile\" type=\"file\" />
 <input type=\"submit\" value=\"Send File\" />
</form>";
	show_tail();
exit;

}

elseif ($type== "xmlimp")

{
	# TODO xmlimport
$uploaddir = '/tmp/';
$uploadfile = $uploaddir . basename($_FILES['userfile']['name']);	
if (move_uploaded_file($_FILES['userfile']['tmp_name'], $uploadfile)) {
    echo "File $uploadfile successfully uploaded. ";
	$xmlr = new Xml();
	$arro = $xmlr->parse($uploadfile, 'FILE');
	//unlink($uploadfile);
	print_r($arro); 
}	
    
//for (reset($arr); $descr = key($arr); next($arr)) {
    //$login= $arr[$descr][dat];
//}
	exit;
}
# ------------ Редактирование существующего пользователя (событие "edit") -------------
elseif ($type == "edit") {
	# ------------- Нажата кнопка "Сохранить" ----------------
	if (isset ( $savebutton )) {
		$login = stripslashes ( trim ( $_GET ['nick'] ) );
		$group = addslashes ( stripslashes ( trim ( $_GET ['group'] ) ) );
		$loginhidden = addslashes ( stripslashes ( trim ( $_GET ['loginhidden'] ) ) );
		# ------{ критичный кусок: необходимы проверки, как при создании пользователя
		$result = mysql_query ( "UPDATE admins SET descr='" . addslashes ( $descr ) . "', login='" . addslashes ( $login ) . "' WHERE login='$loginhidden'", $link );
		logevent ( "modify SA $login, $descr)" );
		Header ( "Location: index.php?type=edit&login=$login" );
		@mysql_close ( $link );
		exit ();
		# ------конец критичного куска }
	} # ------------- Нажата кнопка "Удалить" ----------------
	elseif (isset ( $deletebutton )) {
		$result = mysql_query ( "DELETE FROM admins WHERE login='$login'", $link );
		logevent ( "delete SA $login" );
		
		Header ( "Location: index.php" );
		@mysql_close ( $link );
		exit ();
	}
	if (false) {
		Header ( "Location: index.php" );
		@mysql_close ( $link );
		exit ();
	} else {
		$result = mysql_query ( "SELECT * FROM $group WHERE login='$login'", $link );
		if (mysql_numrows ( $result ) == 0) {
			Header ( "Location: index.php" );
			@mysql_close ( $link );
			exit ();
		}
	}
	
	# -------------- Default-действие для события "edit" ----------------
	show_head ();
	echo "<H1>$edit_user</H1>\n<HR NOSHADE COLOR=#000000 SIZE=1>\n";
	$result = mysql_query ( "SELECT * FROM $group WHERE login='$login'", $link );
	$login1 = mysql_result ( $result, 0, "login" );
	$descr = mysql_result ( $result, 0, "descr" );
	echo "<FORM ACTION=\"index.php\" METHOD=get>
          <INPUT TYPE=hidden NAME=type VALUE=\"edit\">
          <INPUT TYPE=hidden NAME=loginhidden VALUE=\"$login\">
          <TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
          <TR>
            <TD><B>$view_login</B></TD>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=descr VALUE=\"$descr\"></TD>
          </tr>
          <tr>
            <td><b>description</b></td>
            <TD><INPUT TYPE=text CLASS=\"input\" NAME=nick VALUE=\"$login\"></TD>
          </TR>
</tr>
<tr>
          </TR>
          </TABLE>
          <BR>
          <INPUT TYPE=submit CLASS=\"inputsubmit\" ID=\"bg_green\" NAME=savebutton VALUE=\"$edit_save\">&nbsp;&nbsp;&nbsp;&nbsp;
          <INPUT TYPE=button CLASS=\"inputsubmit\" ID=\"bg_blue\"  VALUE=\"$back_tomain\" onClick=\"window.location='index.php#$login'\">&nbsp;&nbsp;&nbsp;&nbsp;
          <INPUT TYPE=button CLASS=\"inputsubmit\" ID=\"bg_red\"   NAME=deletebutton VALUE=\"$edit_delete\" OnClick=\"if ( confirm('Warning! User &laquo;" . htmlspecialchars ( $login ) . "&raquo; will be removed without an opportunity of restoration!') ) window.location='index.php?type=edit&deletebutton=Удалить&login=$login'; \">
          </FORM>
    ";
	show_tail ();
	@mysql_close ( $link );
	exit ();
}

# ------------- Default-действие при отсутствии событий ----------------
show_head ();
echo "<P><P><P>
$web_admin_header_influences:<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=create\">$new_sa_creation_header</A>;<BR>
 &#151;&nbsp;<A HREF=\"../admin/\">$admin_title</A>;<BR>
 &#151;&nbsp;<A HREF=\"../admin/index.php?type=event\">$web_admin_header_viewlog</A>;<BR>
 &#151;&nbsp;<A HREF=\"check.php\">Check environment configuration</A>;<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=db2xml\">export</A>;<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=xml2db\">import</A>;<BR><BR>
 ";
//echo "<BR>\n<HR NOSHADE COLOR=#000000 SIZE=1>\n<BR>\n";
echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
      <TR>
        <TD BGCOLOR=#93BEE2
        ><TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1>
         <TR VALIGN=TOP ALIGN=CENTER>
            <TD HEIGHT=30><A HREF=\"index.php?st=descr\"><B>$web_admin_descr</B></A></TD>
            <TD><A HREF=\"index.php?st=login\"><B>$web_admin_login</B></A></TD>
        </TR>
";

$ORDER_BY = "ORDER BY login";
$st = "login";
//Greyder посоветовал как, вроде помогает, но херово.
setlocale ( LC_ALL, "ru_RU.KOI8-R" );
$result = mysql_query ( "SELECT * FROM admins", $link );
//$arr = mysql_fetch_array($result, MYSQL_NUM);
for($i = 0; $i < mysql_numrows ( $result ); $i ++) {
	$login = mysql_result ( $result, $i, "login" );
	$descr = "Administrator";
	$descr = @mysql_result ( $result, $i, "descr" );
	//    $passwd = mysql_result( $result, $i, "passwd");
	# Выводится содержимое таблицы
	//    $login= $arr[$descr][login];
	echo "<TR BGCOLOR=#FFF7E5>
            <TD><A NAME=\"" . $login . "\">
            </A><A HREF=\"index.php?type=edit&login=$login\">$descr</A></TD>
            <TD>$login</TD></TR>";
}
echo "</TABLE>
</TD>
</TR>
</TABLE><br>";
echo " $web_admin_logfree_space " . ( int ) (disk_free_space ( "$logdir" ) / (1024 * 1024)) . " Mb";
show_tail ();
@mysql_close ( $link );
?>