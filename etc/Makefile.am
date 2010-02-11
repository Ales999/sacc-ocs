ETC_FILES = monthly.sql pref.php.orig php.ini.recommended recount.sql pref.php.in squid.orig.conf
ETC_INIT = init.d/MySQL.control init.d/SquiD.control init.d/sacc.control init.d/SAcc_counter.control init.d/apache.control

install-data-local:
	@$(mkinstalldirs) ${prefix}/sacc/etc
	@$(mkinstalldirs) ${prefix}/sacc/etc/init.d
	$(INSTALL_DATA) -o root ${ETC_FILES} ${prefix}/sacc/etc
	$(INSTALL_PROGRAM) -o root ${ETC_INIT} ${prefix}/sacc/etc/init.d
	$(CHMOD) 600 ${prefix}/sacc/etc/squid.orig.conf
	$(CAT) ./pref.php.in>${prefix}/sacc/etc/pref.php
	$(ECHO) '$$language=1;\
$$version="${VERSION}";\
$$origin="SAcc ".$$version;\
$$megabyte_cost=0;\
$$auth_mode=${auth_mode};\
$$admin_mail="s@econ.psu.ru";\
$$logdir="${squid_log}";\
$$domain="${smb_domain}";\
$$ncsa_passwd="${ncsa_passwd}";\
$$bin_prefix="${prefix}/sacc/bin";\
$$detailed=1;\
$$delimiter=" ";\
$$def_timeacl="time1900";\
$$std_limit=20000000;\
$$cfg_loaded=1;\
$$no_mail="true";\
$$mysql_server="${mysql_host}";\
$$mysql_login="${mysql_user}";\
$$mysql_passwd="${mysql_passwd}";\
$$mysql_database="${mysql_db}";\
if (isset($$debug)) {echo "config loaded...";};\
if ($$_COOKIE["lang"]==0) {include "/usr/local/sacc/etc/ru.php";} else {include "/usr/local/sacc/etc/en.php";};\
?>'>>${prefix}/sacc/etc/pref.php

upgrade-local:
	$(CAT) ./pref.php.in>${prefix}/sacc/etc/pref.php
	$(ECHO) '$$language=1;\
$$version="${VERSION}";\
$$origin="SAcc ".$$version;\
$$megabyte_cost=0;\
$$auth_mode=${auth_mode};\
$$admin_mail="s@econ.psu.ru";\
$$logdir="${squid_log}";\
$$domain="${smb_domain}";\
$$ncsa_passwd="${ncsa_passwd}";\
$$bin_prefix="${prefix}/sacc/bin";\
$$detailed=1;\
$$delimiter=" ";\
$$def_timeacl="time1900";\
$$std_limit=20000000;\
$$cfg_loaded=1;\
$$no_mail="true";\
$$mysql_server="${mysql_host}";\
$$mysql_login="${mysql_user}";\
$$mysql_passwd="${mysql_passwd}";\
$$mysql_database="${mysql_db}";\
if (isset($$debug)) {echo "config loaded...";};\
if ($$_COOKIE["lang"]==0) {include "/usr/local/sacc/etc/ru.php";} else {include "/usr/local/sacc/etc/en.php";};\
?>'>>${prefix}/sacc/etc/pref.php

upgrade:
	$(CAT) ./pref.php.in>${prefix}/sacc/etc/pref.php
	$(ECHO) '$$language=1;\
$$version="${VERSION}";\
$$origin="SAcc ".$$version;\
$$megabyte_cost=0;\
$$auth_mode=${auth_mode};\
$$admin_mail="s@econ.psu.ru";\
$$logdir="${squid_log}";\
$$domain="${smb_domain}";\
$$ncsa_passwd="${ncsa_passwd}";\
$$bin_prefix="${prefix}/sacc/bin";\
$$detailed=1;\
$$delimiter=" ";\
$$def_timeacl="time1900";\
$$std_limit=20000000;\
$$cfg_loaded=1;\
$$no_mail="true";\
$$mysql_server="${mysql_host}";\
$$mysql_login="${mysql_user}";\
$$mysql_passwd="${mysql_passwd}";\
$$mysql_database="${mysql_db}";\
if (isset($$debug)) {echo "config loaded...";};\
if ($$_COOKIE["lang"]==0) {include "/usr/local/sacc/etc/ru.php";} else {include "/usr/local/sacc/etc/en.php";};\
?>'>>${prefix}/sacc/etc/pref.php

uninstall-local:
	