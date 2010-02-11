
install-data-local:
	@$(mkinstalldirs) ${prefix}/sacc/web
	$(CP) -Rf ../web ${prefix}/sacc
	$(RM) ${prefix}/sacc/web/Makefil*
	$(CHOWN) -R root ${prefix}/sacc/web
	$(CAT) ./htdocs/inc/version.php.in>${prefix}/sacc/web/htdocs/inc/version.php
	$(ECHO) 'include "${prefix}/sacc/etc/pref.php";\
if (!(isset($$cfg_loaded))) { echo "ботва! немогу загрузить конфиг!";}\
?>'>>${prefix}/sacc/web/htdocs/inc/version.php
	@$(INSTALL_DATA) ${prefix}/sacc/etc/pref.php ${prefix}/sacc/web/htdocs/inc/pref.php
	@if [ -h ${prefix}/sacc/etc/pref.php ]; then $(RM) ${prefix}/sacc/etc/pref.php;fi
	@if [ -r ${prefix}/sacc/etc/pref.php ]; then $(RM) ${prefix}/sacc/etc/pref.php;fi
	@if [ -h ${prefix}/sacc/etc/ru.php ]; then $(RM) ${prefix}/sacc/etc/ru.php;fi
	@if [ -h ${prefix}/sacc/etc/en.php ]; then $(RM) ${prefix}/sacc/etc/en.php;fi
	@$(LN_S) ${prefix}/sacc/web/htdocs/inc/pref.php ${prefix}/sacc/etc/pref.php
	@$(LN_S) ${prefix}/sacc/web/htdocs/inc/ru.php ${prefix}/sacc/etc/ru.php
	@$(LN_S) ${prefix}/sacc/web/htdocs/inc/en.php ${prefix}/sacc/etc/en.php
			
uninstall-local:

upgrade:
	$(INSTALL_DATA) ${prefix}/sacc/etc/pref.php ${prefix}/sacc/web/htdocs/inc/pref.php
	@if [ -h ${prefix}/sacc/etc/pref.php ]; then $(RM) ${prefix}/sacc/etc/pref.php;fi
	@if [ -r ${prefix}/sacc/etc/pref.php ]; then $(RM) ${prefix}/sacc/etc/pref.php;fi
	@if [ -h ${prefix}/sacc/etc/ru.php ]; then $(RM) ${prefix}/sacc/etc/ru.php;fi
	@if [ -h ${prefix}/sacc/etc/en.php ]; then $(RM) ${prefix}/sacc/etc/en.php;fi
	@$(LN_S) ${prefix}/sacc/web/htdocs/inc/pref.php ${prefix}/sacc/etc/pref.php
	@$(LN_S) ${prefix}/sacc/web/htdocs/inc/ru.php ${prefix}/sacc/etc/ru.php
	@$(LN_S) ${prefix}/sacc/web/htdocs/inc/en.php ${prefix}/sacc/etc/en.php

upgrade-local:
	$(INSTALL_DATA) ${prefix}/sacc/etc/pref.php ${prefix}/sacc/web/htdocs/inc/pref.php
	@if [ -h ${prefix}/sacc/etc/pref.php ]; then $(RM) ${prefix}/sacc/etc/pref.php;fi
	@if [ -r ${prefix}/sacc/etc/pref.php ]; then $(RM) ${prefix}/sacc/etc/pref.php;fi
	@if [ -h ${prefix}/sacc/etc/ru.php ]; then $(RM) ${prefix}/sacc/etc/ru.php;fi
	@if [ -h ${prefix}/sacc/etc/en.php ]; then $(RM) ${prefix}/sacc/etc/en.php;fi
	@$(LN_S) ${prefix}/sacc/web/htdocs/inc/pref.php ${prefix}/sacc/etc/pref.php
	@$(LN_S) ${prefix}/sacc/web/htdocs/inc/ru.php ${prefix}/sacc/etc/ru.php
	@$(LN_S) ${prefix}/sacc/web/htdocs/inc/en.php ${prefix}/sacc/etc/en.php
			


