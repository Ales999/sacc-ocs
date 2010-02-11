dnl $Author: slavik $ $Date: 2006-04-02 22:50:58 +0600 (Вск, 02 Апр 2006) $
dnl $Id: apache.m4 8 2006-04-02 16:50:58Z slavik $
AC_DEFUN([APACHE_FIND], [
  HTDOCS="$1"
unset www
dnl echo "apache www, parameter $1"
  case "$HTDOCS" in
    AUTO)
      for d in /var/www/html /usr/local/apache/htdocs /var/www/htdocs /usr/local/apache/htdocs /usr/local/apache2/htdocs /usr/local/www ; do
dnl echo "looking for $d"
       if test -d $d  ; then
         www=$d
       fi
      done
      ;;
    * )
      if test -d $HTDOCS  ; then
echo "checking for $d"
        www=$HTDOCS
      fi
      ;;
  esac

 if test -z "$www" ; then
   echo "Could not find an installation of apache"
    if test "$TARGET_LINUX" = "true"; then
      echo "Looks like you've forgotten to install httpd RPM"
   fi
  exit 1
 fi
echo "found apache www at $www"
www=$www/sacc
])
