<?php
echo "<HTML><HEAD>
<META HTTP-EQUIV=\"Expires\" CONTENT=\"0\">
<META HTTP-EQUIV=\"Pragma\"  CONTENT=\"no-cache\">    
<META HTTP-EQUIV=\"Cache-Control\" CONTENT=\"no-cache\">
<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=koi8-r\">
<TITLE>SAcc system checker.</TITLE><LINK REL=STYLESHEET TYPE=\"text/css\" HREF=../1.css>
</HEAD><BODY BGCOLOR=#FFFFFF TEXT=#000000>";
echo "<P><P><P><A HREF=\"index.php\">&lt;&lt;</A></p></p></p>";
//echo "<BR>\n<HR NOSHADE COLOR=#000000 SIZE=1>\n<BR>\n";
echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0><TR><TD BGCOLOR=#93BEE2>";
echo "<TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1><TR VALIGN=TOP ALIGN=LEFT><TD HEIGHT=18>";
echo "php.ini name</td><td ALIGN=LEFT>current value</td><td ALIGN=LEFT>recommended value</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'display_errors</td><td ALIGN=RIGHT>' . ( int ) ini_get ( 'display_errors' ) . "</td><td ALIGN=RIGHT>1</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'output_buffering</td><td ALIGN=RIGHT>' . ( int ) ini_get ( 'output_buffering' ) . "</td><td ALIGN=RIGHT>0</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'short_open_tag</td><td ALIGN=RIGHT>' . ( int ) ini_get ( 'short_open_tag' ) . "</td><td ALIGN=RIGHT>1</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'zlib.output_compression</td><td ALIGN=RIGHT>' . ( int ) ini_get ( 'zlib.output_compression' ) . "</td><td ALIGN=RIGHT>0</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'implicit_flush</td><td ALIGN=RIGHT>' . ( int ) ini_get ( 'implicit_flush' ) . "</td><td ALIGN=RIGHT>0</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'safe_mode</td><td ALIGN=RIGHT>' . ( int ) ini_get ( 'safe_mode' ) . "</td><td ALIGN=RIGHT>0</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'safe_mode_gid</td><td ALIGN=RIGHT>' . ( int ) ini_get ( 'safe_mode_gid' ) . "</td><td ALIGN=RIGHT>0</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'max_execution_time</td><td ALIGN=RIGHT>' . ( int ) ini_get ( 'max_execution_time' ) . "</td><td ALIGN=RIGHT>30</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'max_input_time</td><td ALIGN=RIGHT>' . ( int ) ini_get ( 'max_input_time' ) . "</td><td ALIGN=RIGHT>60</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'register_argc_argv</td><td ALIGN=RIGHT>' . ( int ) ini_get ( 'register_argc_argv' ) . "</td><td ALIGN=RIGHT>1</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'magic_quotes_gpc </td><td ALIGN=RIGHT>' . ( int ) ini_get ( 'magic_quotes_gpc' ) . "</td><td ALIGN=RIGHT>1</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'magic_quotes_runtime</td><td ALIGN=RIGHT>' . ( int ) ini_get ( 'magic_quotes_runtime' ) . "</td><td ALIGN=RIGHT>0</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'default_mimetype</td><td ALIGN=RIGHT>' . ini_get ( 'default_mimetype' ) . "</td><td ALIGN=RIGHT>text/html</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'mysql.allow_persistent</td><td ALIGN=RIGHT>' . ( int ) ini_get ( 'mysql.allow_persistent' ) . "</td><td ALIGN=RIGHT>1</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'mysql.connect_timeout</td><td ALIGN=RIGHT>' . ini_get ( 'mysql.connect_timeout' ) . "</td><td ALIGN=RIGHT>180</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'display errors</td><td ALIGN=RIGHT>' . ini_get ( 'display_errors' ) . "</td><td ALIGN=RIGHT>1</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'error reporting</td><td ALIGN=RIGHT>' . ini_get ( 'error_reporting' ) . "</td><td ALIGN=RIGHT>E_ALL & ~E_NOTICE</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'variables_order</td><td ALIGN=RIGHT>' . ini_get ( 'variables_order' ) . "</td><td ALIGN=RIGHT>EGPCS</td></tr><TR BGCOLOR=#FFF7E5><td ALIGN=LEFT>";
print 'register_globals</td><td ALIGN=RIGHT>' . ( int ) ini_get ( 'register_globals' ) . "</td><td ALIGN=RIGHT>1</td></tr>";
echo "</table></TD></TR></TABLE><br>";
//@mysql_close($link);
?>