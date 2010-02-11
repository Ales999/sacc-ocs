<?php
function show_bydays($link)
{
$result = mysql_query("select max(utime) as _max, min(utime) as _min, round((max(utime)-min(utime))/(60*60*24)) as _time from detail;", $link);
$min = @mysql_result( $result, 0, "_min");
$max = @mysql_result( $result, 0, "_max");
$days = @mysql_result( $result, 0, "_time");
$cmin=$min-1;

$mod_name="day stat, daily level";
    show_head();
//    echo "<A HREF=\"index.php\">&lt;&lt;</A>";
    show_help();
    echo "<HR NOSHADE COLOR=#000000 SIZE=1>\n<BR>\n";
echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
      <TR><TD BGCOLOR=#93BEE2>
      <TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1>
         <TR VALIGN=TOP ALIGN=CENTER>
            <TD HEIGHT=15>day</TD>
            <TD><B>count</B></TD>
            <TD><B>bytes</B></TD>
            </TR>";

for ($i = 0; $i < $days; $i++)
{
$cmax=$cmin+(24*60*60);
$result = mysql_query("SELECT sum(size) as _totalmb, count(size) as _total from detail where utime<$cmax and utime>$cmin;", $link);
$total = @mysql_result( $result, 0, "_total");
$totalmb = @mysql_result( $result, 0, "_totalmb");
echo "<TR BGCOLOR=#FFF7E5 ALIGN=RIGHT>
	<td ALIGN=LEFT>".strftime ("%B %d, %T", (int)$cmin)."-".strftime ("%B %d, %T", (int)$cmax)."</td>
	<td>$total</td>
	<td><a href=\"index.php?type=day2&t1=$cmin&t2=$cmax\">".dotize($totalmb)."</a></td>
	</tr>";
$cmin=$cmax;
}

$result = mysql_query("SELECT sum(size) as _totalmb, count(size) as _total from detail;", $link);
$total = @mysql_result( $result, 0, "_total");
$totalmb = @mysql_result( $result, 0, "_totalmb");

echo "<TR BGCOLOR=#FFF7E5 ALIGN=RIGHT>
	<td ALIGN=LEFT>total (".strftime ("%B %d, %T", (int)$min)."-".strftime ("%B %d, %T", (int)$max).")</td>
	<td>$total</td>
	<td>".dotize($totalmb)."</td>
	</tr>";

echo "</TABLE></TD></TR></TABLE><br>";
    show_tail();
    mysql_close($link);
    exit;

}

function show_byday2($tmin, $tmax, $link)
{
$result=mysql_query("select distinct u.login as uname, d.id as id, count(d.size) as _count, sum(d.size) as _ssize from detail d, users u where d.u_id=u.id and utime<$tmax and utime>$tmin group by u_id order by _ssize desc;", $link);	
$mod_name="day stat, daily leve2";
    show_head();	

    show_help();
//    echo "select distinct u.login as uname, sum(d.size) as _ssize from detail d, users u where d.u_id=u.id and utime<$tmax and utime>$tmin group by u_id;";	
    echo "<br>".strftime ("%d %B, %T", (int)$tmin)."-".strftime ("%d, %B %T", (int)$tmax)." ";
    echo "<HR NOSHADE COLOR=#000000 SIZE=1>\n<BR>\n";
echo "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
      <TR><TD BGCOLOR=#93BEE2>
      <TABLE BORDER=0 CELLPADDING=1 CELLSPACING=1>
         <TR VALIGN=TOP ALIGN=CENTER>
            <TD HEIGHT=15>login</TD>
            <TD><B>count</B></TD>
            <TD><B>bytes</B></TD>
            </TR>";
$max=mysql_num_rows($result);
for ($i = 0; $i < $max; $i++)
{
	$login = @mysql_result( $result, $i, "uname");
	$uid = @mysql_result( $result, $i, "id");
	$size = @mysql_result( $result, $i, "_ssize");
	$count = @mysql_result( $result, $i, "_count");
echo "<TR BGCOLOR=#FFF7E5 ALIGN=RIGHT>
	<td>$login</td>
	<td>".dotize($count)."</a></td>
	<td>".dotize($size)."</a></td>
	</tr>";
}
    
    
    
    
}


?>