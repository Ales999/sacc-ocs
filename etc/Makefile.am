ETC_FILES = monthly.sql pref.php.orig php.ini.recommended recount.sql pref.php.in squid.orig.conf
ETC_INIT = init.d/MySQL.control init.d/SquiD.control init.d/sacc.control init.d/SAcc_counter.control init.d/apache.control

PREF_DATA= \
\$$language=1;\n\
\$$version=\"${VERSION}\";\n\
\$$origin=\"SAcc \".\$${version};\n\
\$$megabyte_cost=0;\n\
\$$auth_mode=${auth_mode};\n\
\$$admin_mail=\"s@econ.psu.ru\";\n\
\$$logdir=\"${squid_log}\";\n\
\$$domain=\"${smb_domain}\";\n\
\$$ncsa_passwd=\"${ncsa_passwd}\";\n\
\$$bin_prefix=\"${prefix}/sacc/bin\";\n\
\$$detailed=1;\n\
\$$delimiter=\" \";\n\
\$$def_timeacl=\"time1900\";\n\
\$$std_limit=20000000;\n\
\$$cfg_loaded=1;\n\
\$$no_mail=\"true\";\n\
\$$mysql_server=\"${mysql_host}\";\n\
\$$mysql_login=\"${mysql_user}\";\n\
\$$mysql_passwd=\"${mysql_passwd}\";\n\
\$$mysql_database=\"${mysql_db}\";\n\
if (isset(\$$debug)) {echo \"config loaded...\";};\n\
if (\$$_COOKIE[\"lang\"]==0) {include \"$(prefix)/sacc/etc/ru.php\";} else {include \"$(prefix)/sacc/etc/en.php\";};\n\
?>\n

install-data-local:
	@$(mkinstalldirs) ${prefix}/sacc/etc
	@$(mkinstalldirs) ${prefix}/sacc/etc/init.d
	$(INSTALL_DATA) -o root ${ETC_FILES} ${prefix}/sacc/etc
	$(INSTALL_PROGRAM) -o root ${ETC_INIT} ${prefix}/sacc/etc/init.d
	$(CHMOD) 600 ${prefix}/sacc/etc/squid.orig.conf
	$(CAT) ./pref.php.in>${prefix}/sacc/etc/pref.php
	$(ECHO) -e "$(PREF_DATA)" | sed -e 's/^[ ]//'>>${prefix}/sacc/etc/pref.php

upgrade-local:
	$(CAT) ./pref.php.in>${prefix}/sacc/etc/pref.php
	$(ECHO) -e "$(PREF_DATA)" | sed -e 's/^[ ]//'>>${prefix}/sacc/etc/pref.php

upgrade:
	$(CAT) ./pref.php.in>${prefix}/sacc/etc/pref.php
	$(ECHO) -e "$(PREF_DATA)" | sed -e 's/^[ ]//'>>${prefix}/sacc/etc/pref.php

uninstall-local:
	