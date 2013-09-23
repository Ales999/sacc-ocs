<?php
// db_connect
function db_connect() {
	global $mysql_database;
	
	if ($mysql_database != "sacc") {
		echo "<H1>Warning!</H1>
          <P>
            <FONT COLOR=#FF0000>Look's like shit, database name != sacc </FONT>
          </P>
          Если это сделано намеренно - удалите эту проверку нафиг и сами виноваты...";
		@mysql_close ( $link );
		debug ();
	}
	$link = db_connect_ex ( $mysql_database );
	return $link;
}
;

function db_connect_ex($dbase) {
	global $mysql_server, $mysql_login, $mysql_passwd, $try_again;
	
	$link = mysql_connect ( $mysql_server, $mysql_login, $mysql_passwd );
	$db = mysql_select_db ( $dbase, $link );
	
	if ($db == NULL) {
		show_head ();
		echo "<H1>Warning!</H1>
          <P>
            <FONT COLOR=#FF0000>can't connect to MySQL or select database! Check rights on tables!</FONT>
          </P>
          <FORM>
            <INPUT TYPE=button CLASS=\"inputsubmit\" VALUE=\"$try_again\" OnClick=\"window.location='index.php'\">
          </FORM>\n";
		show_tail ();
		@mysql_close ( $link );
	}
	mysql_query ( "set character_set_results= utf8;", $link );
	mysql_query ( "set character_set_connection= utf8;", $link );
	mysql_query ( "set character_set_client= utf8;", $link );
	mysql_query ( "set character_set_database = utf8;", $link );
	return $link;
}
;

?>