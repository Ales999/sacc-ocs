<?php
//      Функция show_help() - отображает краткую справку по программе
//      Вход : ничего
//      Выход: всегда true;
//
function show_help() {
	global $settings;
	global $web_admin_header_managesa, $web_admin_header_viewlog, $admin_title, $web_admin_history, $web_admin_queue, $web_admin_acl, $web_admin_dom;
	global $web_admin_header_adduser, $web_admin_header_influences, $web_admin_perf, $web_admin_header_options, $web_admin_header_viewrsh, $web_admin_daystat;
	echo "<p>$web_admin_header_influences:<BR>
 &#151;&nbsp;<A HREF=\"index.php\">$admin_title</A>;<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=create\">$web_admin_header_adduser</A>;<BR>";
	$crsh = $settings ['cisco'];
	if ("1" == $crsh) {
		echo "
 &#151;&nbsp;<A HREF=\"index.php?type=rsh\">$web_admin_header_viewrsh</A>;<BR>";
	}
	;
	echo "
 &#151;&nbsp;<A HREF=\"index.php?type=hist\">$web_admin_history</A>;<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=queue\">$web_admin_queue</A>;<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=acl\">$web_admin_acl</A>;<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=perf\">$web_admin_perf</A>;<BR>
 &#151;&nbsp;<A HREF=\"../sa/\">$web_admin_header_managesa</A>;<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=event\">$web_admin_header_viewlog</A>;<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=options\">$web_admin_header_options</A>;<BR>
 &#151;&nbsp;<A HREF=\"index.php?type=day1\">$web_admin_daystat</A>;<BR>
 ";

//  &#151;&nbsp;<A HREF=\"index.php?type=doms\">$web_admin_dom</A>;<BR>
}
?>
