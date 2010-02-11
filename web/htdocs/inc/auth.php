<?php
// authenticators


/*
auth_cli - функция авторизации клиента.
вход: $user, $password
выход: true\false
*/
function auth_cli($user, $passwd, $link) {
	global $auth_mode;
	global $msg;
	$ares = flase;
	$passwd = stripslashes ( $passwd );
	$user = stripslashes ( $user );
	if ((! isset ( $user )) or (! isset ( $passwd )))
		return false;
	$user = rawurlencode ( $user );
	$passwd = rawurlencode ( $passwd );
	$passwd = str_replace ( "\\.", ".", $passwd );
	$user = str_replace ( "\\.", ".", $user );
	switch ($auth_mode) {
		case 2 :
			{
				// AD mode
				$ares = auth_smb ( $user, $passwd );
				break;
			}
		case 1 :
			{
				// NCSA mode
				$ares = auth_ncsa ( $user, $passwd );
				break;
			}
		case 3 :
			{
				// MySQL mode
				$ares = auth_mysql ( $user, $passwd, $link );
				break;
			}
		default :
			{
				//WTF?!
				echo "incorrect auth mode!";
				die ( 0 );
			}
	}
	$msg = $e . " mode " . $auth_mode . "/" . $ares . "/" . $user . "/" . $passwd;
	//$msg2=$ares;
	if ($ares == "ERR")
		return false;
	else if (strstr ( $ares, "OK" ) != FALSE)
		return true;
	return false;
}

//      Функция smb_auth($user, $passwd) - авторизует пользователя с заданным
//                                              логином и паролем
//      Вход :
//             $user - логин (ник) пользователя)
//             $passwd - пароль пользователя
//      Выход: в случае успешного прохождения аутентификации возвращается true, иначе - false
//      Примечание 1: для проверки логина и пароля используется внешняя программа
//                    возвращающая один из двух ответов: "OK" или "ERR"
//
//
function auth_ncsa($user, $passwd) {
	global $bin_prefix;
	global $ncsa_passwd;
	$e = "echo \"" . $user . " " . $passwd . "\" | $bin_prefix/ncsa_auth $ncsa_passwd";
	//         $e = escapeshellcmd($checkstring);
	//$e=$checkstring;
	$result = shell_exec ( $e );
	return $result;
}

function auth_smb($user, $passwd) {
	global $bin_prefix;
	global $domain;
	//global $dc_ip;
	$e = "echo \"" . $user . " " . $passwd . "\" | $bin_prefix/smb_auth -W $domain";
	//         $e = escapeshellcmd($checkstring);
	//$e=$checkstring;
	$result = shell_exec ( $e );
	return $result;
}

function auth_mysql($user, $passwd, $link) {
	$link = db_connect ();
	$result = mysql_query ( "select * from users where login = '$user' and passwd='$passwd' and perm=777", $link );
	echo mysql_error ();
	$dat1 = "select * from admins where login = '$user' and passwd='" . md5 ( $passwd ) . "'";
	if (mysql_num_rows ( $result ) == 0) {
		mysql_close ( $link );
		return "ERR";
	} else if (mysql_num_rows ( $result ) != 0) {
		mysql_close ( $link );
		return "OK";
	}
	@mysql_close ( $link );
	return "ERR";
}

//      Функция auth_adm($user, $passwd) - авторизует пользователя с заданным
//                                              логином и паролем
//      Вход :
//             $user - логин (ник) пользователя)
//             $passwd - пароль пользователя
//      Выход: в случае успешного прохождения аутентификации возвращается true, иначе - false
function auth_adm($user, $passwd) {
	global $dat1;
	
	if ((! isset ( $user )) or (! isset ( $passwd )))
		return false;
	else {
		$link = db_connect ();
		$dat1 = "select * from admins where login = '$user' and passwd = '" . md5 ( $passwd ) . "'";
		$result = mysql_query ( $dat1, $link );
		echo mysql_error ();
		if (mysql_num_rows ( $result ) == 0) {
			mysql_close ( $link );
			return false;
		} else if (mysql_num_rows ( $result ) != 0) {
			mysql_close ( $link );
			return true;
		}
	}
	@mysql_close ( $link );
	return false;
}

?>