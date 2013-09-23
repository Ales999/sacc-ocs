<?php
//
//    SQUID Acconting                                            [SAcc system] 
//    Copyright (C) 2003  the.Virus
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

//--------------------------------------------------------- 
// Типа конфиг. Сделан с особым цинизмом.
// (C) the.Virus icq#210990
// Тута всяка хрень понаписана, можете почитать, 
// если нихрена непонятно - попробуйте изучить PHP, MySQL 
// основы IT security и всякую такую ботву... 
// говорят помогает ;)
// --------------------------------------------------------
// SAcc Web interface and authorization config.
//
// Web interface lang (by default)
// 0 - russian
// 1 - english (in develop)
//$debug=1;
//$PHP_AUTH_USER=$_SERVER['REMOTE_USER'];
$language=1;
$version="1.80-stable3";
$origin="SAcc ".$version;
$megabyte_cost=0;
$auth_mode=2;
$admin_mail="s@econ.psu.ru";
$logdir="/var/log/squid33/access.log";
$domain="KRBD";
$ncsa_passwd="/usr/local/sacc/etc/passwd";
$bin_prefix="/usr/local/sacc/bin";
$detailed=1;
$delimiter=" ";
$def_timeacl="fulltime";
$std_limit=314572800;
$cfg_loaded=1;
$no_mail="true";
$mysql_server="localhost";
$mysql_login="sacc";
$mysql_passwd="sacc";
$mysql_database="sacc";
if (isset($debug)) {echo "config loaded...";};
if ($_COOKIE["lang"]==0) {include "/usr/local/sacc/etc/ru.php";} else {include "/usr/local/sacc/etc/en.php";};
?>
