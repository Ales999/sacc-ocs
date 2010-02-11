dnl $Author: slavik $ $Date: 2006-10-18 23:33:49 +0600 (Срд, 18 Окт 2006) $
dnl $Id: mysql.m4 15 2006-10-18 17:33:49Z slavik $
AC_DEFUN([MYSQL_FIND], [
  incs="$1"
  libs="$2"
  case "$incs---$libs" in
    ---)
      for d in /usr/include /usr/mysql/include \
/usr/local/include /usr/local/mysql/include \
/opt/mysql/include /opt/freeware/mysql/include /opt/gnu/mysql/include ; do
echo "looking for inc in $d"
       if test -f $d/mysql/mysql.h  ; then
         MYSQL_INCLUDE=-I$d
       fi
      done

      for d in /usr/lib /usr/lib/mysql /usr/local/mysql/lib /usr/mysql/lib /usr/local/lib \
/usr/local/lib/mysql /usr/local/mysql/lib/mysql \
/opt/mysql/lib/mysql /opt/freeware/mysql/lib/mysql \
/usr/lib32 /usr/lib64 /usr/lib32/mysql /usr/lib64/mysql /opt/mysql/lib \
/usr/freeware/lib32 /opt/mysql/lib/mysql ; do
echo "looking for lib in $d"
      if test -f $d/libmysqlclient.a || test -f $d/libmysqlclient.so || test -f $d/libmysqlclient.dylib ; then
        MYSQL_LIB=$d
      fi
      done
      ;;
    ---* | *---)
      AC_MSG_ERROR([if either 'includes' or 'libs' is specified, both must be specified])
      ;;
    * )
      if test -f $incs/mysql/mysql.h  ; then
        MYSQL_INCLUDE=-I$incs
      fi
      if test -f $libs/libmysqlclient.a || test -f $libs/libmysqlclient.so || test -f $libs/libmysqlclient.dylib ; then
        MYSQL_LIB=$libs
      fi
      ;;
  esac

echo "lib $MYSQL_LIB, include $MYSQL_INCLUDE"
 if test -z "$MYSQL_LIB" -o -z "$MYSQL_INCLUDE" ; then
   echo "Could not find an installation of MySQL"
   if test -n "$MYSQL_LIB" ; then
    if test "$TARGET_LINUX" = "true"; then
      echo "Looks like you've forgotten to install MySQL development RPM"
    fi
   fi
  exit 1
 fi

])

