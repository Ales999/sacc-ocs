/********************************************************************************
#           SAccounting                                            [SAcc system]
#			   Copyright (C) 2003-2007  Vyacheslav Nikitin
#		$Author: slavik $ $Date: 2008-08-07 14:46:06 +0600 (п╖я┌п╡, 07 п░п╡пЁ 2008) $
#		$Id: main.cpp 38 2008-08-07 08:46:06Z slavik $
#           -----------------------------------------------------
#   			This file is part of SAcc system.
#                    [http://sacc.cybersec.ru]
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
******************************************************************************/

// TODO:
// ?. переписать функции лога, сделать несколько режимов.
// очередь команд
// обработку acl
// группы.
// 2 [Проблема была в том, что изначально размер файла access.log был равен нулю, что вызывало зацикливание. ]
//#define BROKEN_CACHE 1
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
/* includes */
#include "squid.h"
#include "mylog.h"
/* system includes */
#include <iostream>
#include <cstring>
#include <string>
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mysql/mysql.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STDARG_H		/* gcc 3 */
#include <stdarg.h>
#else				/* gcc 2 */
#ifndef DEBIAN
#include <varargs.h>
#else
#include <stdarg.h>
#endif				// End DEBIAN
#endif				/* HAVE_STDARG_H */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <regex.h>
#include <errno.h>
#include <err.h>

#ifndef HAVE_FSEEKO
#define fseeko(f, offset, whence)       fseek(f, (long)(offset), whence)
#define ftello(f)                       (off_t)ftell(f)
#endif

//#define _FILE_OFFSET_BITS 64
// define this for per ip statistic only
//#define IP_STAT 1
//
#ifndef HAVE_STRSEP
#define strsep(str1, str2) getToken(str1,str2)
#endif
/* configuartion */
#define WWW_SITE "http://sacc.cybersec.ru"
#define MAXLEN 10*1024*SIZEOF_CHAR
//#define DEBUG 1 /* define to 1 for developer debugging */
#define QUERY_BUFSIZE   (MAXLEN-1)
#define PING_TIMEOUT    60*5
#define SESSION_LOG_FNAME "session.log"
/* counter sleep time mcsec */
#define SLEEP_TIME 200000
/* user testing delay, max = 10 sec, or 500 access.log strings */
#define USER_TEST_DELAY 50
#define MAXBUFSIZE QUERY_BUFSIZE
#define STR_MAX_SIZE  QUERY_BUFSIZE-1
#define APP_NAME "SAcc squid control daemon"
#define SYSLOG_NAME "SAcc-squid"
/* not implemented */
#define DAEMON_LOG "/usr/local/sacc/var/counter.log"
#define CONF_FILE "/usr/local/sacc/etc/sacc.conf"
/* end of not implemented */
/* дробление лога */
//#define LOG_SPLIT 1
#define _T(txt) (char*)(txt)
#define _V2PC(data) (char*)(data)
#define _nstrcmp(str1, str2) if (*str1==*str2) if (0==strcmp( str1, str2))
#define _strcmp(str1, str2) if (strstr(str1, str2)!=NULL)
//rtrrtr
char *tmpbf;
/* command var's */
int config_noseek = 0;
unsigned long long offset = 0;
int detailed = 1;
int autolearn = 0;
#ifndef DEBUG
int config_nodaemon = 0;
bool config_insyslog = true;
int loglevel = 6;
#else
int config_nodaemon = 1;
//int config_nosyslog = 1;
bool config_insyslog = false;
int loglevel = 7;
#endif
char *squid_filename = NULL;
int exit_imm = 0;
int ErrNumber = 0;
/* auto generation parameters */
int users_positive = 0;
int users_negative = 0;
int users_total = 0;
int cycle_counter = 0;
struct tm *timer;
time_t cur_time;
int logrotated = 0;
/* end of agp */
pid_t PID;
char *temp = _V2PC(malloc(MAXLEN));
//caching
#define USER_CACHE_SIZE 10
uid_cache ucache[USER_CACHE_SIZE];
int ucpointer = 0;
int uccsize = 0;
#define STAT
#ifdef STAT
int cache_hit = 0;
int cache_miss = 0;
int cache_lines = 0;
time_t start_time1, stop_time1;
#endif
time_t sql_timeping;
int squid_loffset = 0;
/* signals processing */
#define SIG_RECONFIG 1
#define SIG_SHUTDOWN 2
#define SIG_ROTATE 3
#define SIG_RECOUNT 4
static sig_atomic_t sig_num = 0;
FILE *fp;
/* mysql stuff */
MYSQL mysql;
int squid_setlogoffset(off_t log_offset, MYSQL * lmysql);
void squid_reconfig();
//int sql_exec(char* sql_filename);
static void sql_dbping();
void exit_mysql();
void exit_all();
void _logerr(char *, char *, int);
void _logmsg(char *, char *, int);
void _logcrit(char *, char *, int);
mylog logger;
//#define logerr(mesg) printf("%s:%d %d %s",__FILE__, __LINE__, 3, mesg)
//#define logcrt(mesg) printf("%s:%d %d %s",__FILE__, __LINE__, 4, mesg)
//#define logmsg(mesg) printf("%s:%d %d %s",__FILE__, __LINE__, 5, mesg)

#define logerr(mesg) logger.msg(__FILE__, __LINE__, 3, mesg)
#define logcrt(mesg) logger.msg(__FILE__, __LINE__, 4, mesg)
#define logmsg(mesg) logger.msg(__FILE__, __LINE__, 5, mesg)
#define loginf(mesg) logger.msg(__FILE__, __LINE__, 6, mesg)
#define logdbg(mesg) logger.msg(__FILE__, __LINE__, 7, mesg)
off_t get_offset(void);
pid_t PidFile_read(void);
int sys_logfile;
char *sql_query = _V2PC(malloc(STR_MAX_SIZE));
// From krbconv.cpp:
//extern char *ConvertKrbName(const std::string, bool obr = true);
extern std::string ConvertKrbName(const std::string, bool obr = true);

/* code itself */
/* Get time ticket */
void sys_gettime(time_t * t)
{
//    time_t tmp;
	time(t);
//    *t=tmp;
}

#ifndef HAVE_STRSEP
/*
 * This is a replacement for strsep which is not portable (missing on Solaris).
 */
static char *getToken(char **str, const char *delims)
{
	char *token;

	if (*str == NULL) {
		/* No more tokens */
		return NULL;
	}

	token = *str;
	while (**str != '\0') {
		if (strchr(delims, **str) != NULL) {
			**str = '\0';
			(*str)++;
			return token;
		}
		(*str)++;
	}
	/* There is no other token */
	*str = NULL;
	return token;
}
#endif				/* strsep */

char *_fgets(char *s, int len, FILE * fp)
{
#ifndef BROKEN_CACHE
	return fgets(s, len, fp);
#else
	static int getted, i;
	static off_t off;
	static ssize_t size = 0;
	static bool eolreached;
	getted = 0;
	eolreached = false;
//bzero(s, len);
	off = ftello(fp);
	while (getted < len) {
		if (-1 == (size = fread(s + size, len - getted - 1, 1, fp)))
//      if (-1==(size=read(fp->_file, s+size, len-getted-1)))
		{
			usleep(20000);
			if (errno == EINTR)
				return NULL;
			continue;
		};
		for (i = 0; i < size; getted++, i++)
			if (s[getted] == 0x0a) {
				s[getted] = 0;
				eolreached = true;
				break;
			};
		if (eolreached)
			break;
		if (sig_num)
			return NULL;
		usleep(20000);
//      if (errno==EINTR) return NULL;
	}
	fseeko(fp, off + getted + 1, SEEK_SET);
	return s;
#endif
}

/*
const char *LogTime(time_t t, char *fmt)
{
    struct tm *tm;
    static time_t last_t = 0;
    if (t != last_t) {
        tm = localtime(&t);
        strftime(temp, 127, fmt, tm);
        last_t = t;
    }
    return temp;
}
*/
/* check for running copy */
static int checkRunningPid(void)
{
	pid_t pid;
	pid = PidFile_read();
	if (pid < 2)
		return 0;
	if (kill(pid, 0) < 0)
		return 0;
	printf("%s is already running!  Process ID %d\n", APP_NAME, pid);
	return 1;
}

/* Read PID file and return it */
pid_t PidFile_read(void)
{
	FILE *pid_fp = NULL;
	const char *f = PID_FILE;
	pid_t pid = -1;
	int i;
	pid_fp = fopen(f, "r");
	if (pid_fp != NULL) {
		pid = 0;
		if (fscanf(pid_fp, "%d", &i) == 1)
			pid = (pid_t) i;
		fclose(pid_fp);
	}
	return pid;
}

int squid_setlogoffset(off_t log_offset, MYSQL * lmysql)
{
#ifdef DEBUG
	printf("log set offset %llu\n", (long long int)log_offset);
#endif
//    if (got_recount==1) {log_offset=get_offset();}
	snprintf(sql_query, STR_MAX_SIZE,
		 "UPDATE sys_trf SET offset=%llu where id=1",
		 (long long int)log_offset);
#ifdef DEBUG
	printf(sql_query);
	printf("\n");
#endif
	if (mysql_query(lmysql, sql_query) != 0) {
		/* log message about error */
		snprintf(temp, STR_MAX_SIZE, "MySQL Error - %d: %s\n",
			 mysql_errno(lmysql), mysql_error(lmysql));
		logerr(temp);
		/* exit */
		ErrNumber = 1;
		exit_mysql();
		return 1;
	}
	return 0;
}

/* Read SquiD PID file and return PID */
pid_t squid_getpid(void)
{
	FILE *pid_fp = NULL;
	const char *f = SQUID_PID_FILE;
	pid_t pid = -1;
	int i;
	pid_fp = fopen(f, "r");
	if (pid_fp != NULL) {
		pid = 0;
		if (fscanf(pid_fp, "%d", &i) == 1)
			pid = (pid_t) i;
		fclose(pid_fp);
	} else {
		snprintf(temp, STR_MAX_SIZE, "can't find %s", SQUID_PID_FILE);
		logmsg(temp);
	}
/*
    if (pid<2)
    {
      logerr(_T("SQUID not running!"));
      exit_all();
    }
*/
	return pid;
}

/* write new pid file */
int PidFile_write(void)
{
	FILE *fd = NULL;
	const char *f = NULL;
	mode_t old_umask;
	char buf[32];
	if ((f = PID_FILE) == NULL)
		return 0;
	if (!strcmp(PID_FILE, "none"))
		return 0;
	old_umask = umask(022);
	fd = fopen(f, "w");
	umask(old_umask);
	if (fd == NULL) {
		snprintf(temp, STR_MAX_SIZE, "can't open %s", PID_FILE);
		logcrt(temp);
		exit_all();
	}
	snprintf(buf, 32, "%d\n", (int)getpid());
	fwrite(buf, (size_t) 1, strlen(buf), fd);
	fclose(fd);
	return 1;
}

/* Log Error message */
void _logerr(char *msg, char *FileName, int Line)
{
	if (config_insyslog)
		syslog(LOG_CRIT, "%s in %s:%d", msg, FileName, Line);
	else
		printf("ERROR: %s in %s:%d\n", msg, FileName, Line);
}

/* Log Critical message */
void _logcrit(char *msg, char *FileName, int Line)
{
	if (config_insyslog)
		syslog(LOG_CRIT, "%s in %s:%d", msg, FileName, Line);
	else
		printf("CRITICAL: %s in %s:%d\n", msg, FileName, Line);
}

/* Log notice message */
void _logmsg(char *msg, char *FileName, int Line)
{
	if (config_insyslog)
		syslog(LOG_NOTICE, "%s in %s:%d", msg, FileName, Line);
	else
		printf("NOTICE: %s in %s:%d\n", msg, FileName, Line);
}

/* full cleanups */
void exit_all()
{
#ifdef STAT
	sys_gettime(&stop_time1);
	snprintf(temp, MAXLEN, "%d hit/%d miss/%d total, time %d\n", cache_hit,
		 cache_miss, cache_lines, (int)(stop_time1 - start_time1));
	logmsg(temp);
#endif

	/* delete pid file */
	if (unlink(PID_FILE) != 0) {
		snprintf(temp, STR_MAX_SIZE, "can't remove pid file: %s",
			 PID_FILE);
		logmsg(temp);
	};
	/* log message about what we're stoped */
	snprintf(temp, STR_MAX_SIZE, "service %s (%u) stopped.", APP_NAME, PID);
	logmsg(temp);
	close(0);
	close(1);
	close(2);
	close(sys_logfile);
#ifndef STAT
	unlink(SESSION_LOG_FNAME);
#endif
	exit(ErrNumber);
}

/* exit with closing mysql connection */
void exit_mysql()
{
	/* closing connection with MySQL server */
	mysql_close(&mysql);
	exit_all();
}

/* connect to MySQL db */
void sql_connect(MYSQL * lpmysql_link)
{
	mysql_init(lpmysql_link);
	if (lpmysql_link !=
	    mysql_real_connect(lpmysql_link, DB_HOSTNAME, DB_USERNAME,
			       DB_PASSWORD, DB_DATABASE, 3306, NULL, 0)) {
		snprintf(temp, STR_MAX_SIZE,
			 "can't connect to server, MySQL Error - %d: %s\n",
			 mysql_errno(lpmysql_link), mysql_error(lpmysql_link));
		logcrt(temp);
		exit_all();
	}
	/* try to select DB */
	if (mysql_select_db(lpmysql_link, DB_DATABASE)) {
		/* log message about error */
		snprintf(temp, STR_MAX_SIZE,
			 "Unable to select db, MySQL Error - %d: %s\n",
			 mysql_errno(lpmysql_link), mysql_error(lpmysql_link));
		logcrt(temp);
		exit_all();
	}
#ifdef HAVE_MYSQL_SET_CHARACTER_SET
	if (!mysql_set_character_set(&mysql, "koi8r")) {
		printf("New client character set: %s\n",
		       mysql_character_set_name(&mysql));
	}
#endif
	return;
}

/* fork */
void _fork(void)
{
	switch (fork()) {
	case -1:
		logcrt(_T("fork() step failed"));
		exit_all();
	case 0:
#ifdef DEBUG
		logmsg(_T("fork() step ok"));
#endif
		return;
	default:
		exit(0);
	};
}

/* get file offset from database */
off_t get_offset()
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	off_t int_offset;
#ifdef DEBUG
	printf("get_offset: entered, query database\n");
#endif
	/* Getting offset from sys table */
	if (mysql_query(&mysql, "SELECT offset FROM sys_trf where id=1")) {
		/* log message about error */
		snprintf(temp, STR_MAX_SIZE, "MySQL Error - %d: %s\n",
			 mysql_errno(&mysql), mysql_error(&mysql));
		logcrt(temp);
		ErrNumber = 1;
		exit_all();
	}
#ifdef DEBUG
	printf("get_offset: result\n");
#endif
	/* get offset from result */
	result = mysql_store_result(&mysql);
	if (result == NULL) {
		logcrt(_T
		       ("get_offset: can't get work offset from table sys_trf, shutting down."));
		exit_all();
	};
#ifdef DEBUG
	printf("get_offset: fetch row\n");
#endif
	row = mysql_fetch_row(result);
	if (mysql_num_rows(result) == 0) {
		logcrt(_T
		       ("get_offset: can't get work offset from table sys_trf, shutting down."));
		exit_all();
	}
#ifdef DEBUG
	printf("get_offset: atoi\n");
#endif
	int_offset = atoll(row[0]);
#ifdef DEBUG
	printf("get_offset: offset=%llu, freeresult\n", int_offset);
#endif
	mysql_free_result(result);
#ifdef DEBUG
	printf("get_offset: return\n");
#endif
	return (int_offset);
}

// return int = YYMM
int get_lastlogrotation(MYSQL * lmysql)
{
#ifdef DEBUG
	printf("get_lastlog: entered\n");
#endif
	MYSQL_RES *result;
	MYSQL_ROW row;
	time_t int_offset;
	/* Getting date */
	if (mysql_query(lmysql, "select max(utime) from uhistory;")) {
		/* log message about error */
		snprintf(temp, STR_MAX_SIZE, "MySQL Error - %d: %s\n",
			 mysql_errno(lmysql), mysql_error(lmysql));
		logcrt(temp);
		ErrNumber = 1;
		exit_all();
	}
	/* get offset from result */
	result = mysql_store_result(lmysql);
	if (result == NULL) {
		logcrt(_T("can't get last logrotation, return..."));
		return 0;
	};
#ifdef DEBUG
	printf("get_lastlog: select, num_row=%d\n",
	       (int)mysql_num_rows(result));
#endif
	if (mysql_num_rows(result) == 0) {
		// кусок требует дополнительного тестирования
#ifdef DEBUG
		printf("get_lastlog: no records, set today\n");
#endif
		logerr(_T("can't get last logrotation, set today"));
		struct tm *timing;
		time_t tmp;
		time(&tmp);
		timing = localtime(&tmp);
		int_offset = timing->tm_year * 100 + timing->tm_mon;
		return int_offset;
		// end of block
	}
	row = mysql_fetch_row(result);
#ifdef DEBUG
	printf("get_lastlog: row[0]='%s'\n", row[0]);
#endif
	if (row[0] == NULL) {
		// кусок требует дополнительного тестирования
#ifdef DEBUG
		printf("get_lastlog: null row, set today\n");
#endif
		logerr(_T("get_lastlog: null row, set today"));
		struct tm *timing;
		time_t tmp;
		time(&tmp);
		timing = localtime(&tmp);
		int_offset = timing->tm_year * 100 + timing->tm_mon;
		return int_offset;
		// end of block
	}
	int_offset = (time_t) atol(row[0]);
	tm *atime;
	atime = localtime(&int_offset);
//    strftime(temp, MAXLEN, "%y%m", tm);
	int_offset = atime->tm_year * 100 + atime->tm_mon;
	mysql_free_result(result);
#ifdef DEBUG
	printf("get_lastlog: result=%d\n", int_offset);
#endif
	return (int_offset);
}

/* do demonization */
void sys_daemonize()
{
	snprintf(temp, STR_MAX_SIZE, "%s/sacc/var", PREFIX);
	chdir(temp);
	/* close STDIN and STDOUT */
	mode_t old_umask;
	old_umask = umask(133);
	umask(old_umask);

	if (sys_logfile == -1) {
		snprintf(temp, STR_MAX_SIZE, "can't open %s w",
			 SESSION_LOG_FNAME);
		logcrt(temp);
		exit_all();
	}
	/* Establish new open descriptors for stdin, stdout and stderr. */
//    dup2(sys_logfile, 0);
	_fork();
	setsid();		// set as group leader
	_fork();
	umask(0);		//we need complete control over the permissions of anything we write.
	/* close STDIN and STDOUT */
	close(0);		// stdin
	close(1);		// stdout
	close(2);		// stderr
	sys_logfile = open(SESSION_LOG_FNAME, O_CREAT | O_RDWR | O_TRUNC);
	dup2(sys_logfile, 1);
	dup2(sys_logfile, 2);
/* change my name */
#ifdef HAVE_SETPROCTITLE
	setproctitle("-%s", APP_NAME);
#endif
}

/* shut down signal handler */
void sig_shutdown(int)
{
#ifdef DEBUG
	printf("recieved TERM signal");
#endif
//      logmsg(_T("SIGTERM recieved, quitting."));
	sig_num = SIG_SHUTDOWN;
}

/* signal handler recount */
void sig_recount(int)
{
#ifdef DEBUG
	printf("recieved USR2 signal");
#endif
//      logmsg(_T("USR2 signal got, lets recount..."));
	sig_num = SIG_RECOUNT;
}

/* signal handler logrotate */
void sig_rotate(int)
{
#ifdef DEBUG
	printf("recieved USR1 signal");
#endif
	sig_num = SIG_ROTATE;
}

/* reconfugure signal handler */
void sig_reconf(int)
{
#ifdef DEBUG
	printf("recieved HUP signal");
#endif
//      logmsg(_T("SIGHUP - reconfiguration begin..."));
	sig_num = SIG_RECONFIG;
//    squid_reconfig();
}

int sql_exec(char *sql_filename)
{
	MYSQL mysql_link;
	sql_connect(&mysql_link);
	FILE *sqlscript;
	int len, count = 0;
#ifdef DEBUG
	printf("sql_exec: fopen script %s\n", sql_filename);
#endif
	if (NULL == (sqlscript = fopen(sql_filename, "r"))) {
		snprintf(temp, STR_MAX_SIZE,
			 "sql_exec: Can't open %s for read.", sql_filename);
		logcrt(temp);
		sig_shutdown(1);
		return 1;
	}
	while (!feof(sqlscript)) {
		count++;
		len = MAXLEN - 20;
		memset(sql_query, 0, STR_MAX_SIZE);
		fgets(sql_query, len, sqlscript);
		if (strlen(sql_query) > 5)
			if (mysql_query(&mysql_link, sql_query)) {
				/* log message about error */
				snprintf(temp, STR_MAX_SIZE,
					 "sql_exec: %s:%d MySQL Error - %d: %s\n",
					 sql_filename, count,
					 mysql_errno(&mysql_link),
					 mysql_error(&mysql_link));
				logcrt(temp);
				return 1;
			}
	}
	fclose(sqlscript);
	mysql_close(&mysql_link);
	return 0;
}

int squid_recount(MYSQL * lmysql)
{
	logmsg(_T("squid_recount: recounting"));
	offset = 0;
	squid_setlogoffset(offset, lmysql);
	//in case of error, we never reach this line...
	fseeko(fp, offset, SEEK_SET);
	snprintf(temp, STR_MAX_SIZE, "%s/sacc/etc/recount.sql", PREFIX);
	if (1 == sql_exec(temp)) {
		return 1;
	} else {
		return 0;
	};
}

// Если в конфиге сквида установлено что ротейтится внешними программами
// то дожидаться не надо, а надо ловить момент ротации.
bool squid_logconf(void)
{
// В дебиане дефолтом - внешними
#ifdef DEBIAN
	return true;
#else
	return false;
#endif

}

/* squid logrotation */
int squid_logrotate(void)
{
	int result = 0;
	pid_t squid_pid;
	struct tm *timing;
	time_t tmp;
	time(&tmp);
	timing = localtime(&tmp);
	logrotated = timing->tm_year * 100 + timing->tm_mon;
	logmsg(_T("monthly job: started"));
	squid_pid = squid_getpid();
	/* send logrotation signal to squid */
	if (squid_pid < 2) {
		logmsg(_T
		       ("monthly job: SquiD PID invalid. Silently skip logrotation..."));
		result = 1;
	} else {
		struct stat finfo;
		off_t logsize;
// buggy
		stat(squid_filename, &finfo);
		logsize = finfo.st_size;
		if (-1 == kill(squid_pid, SIGUSR1)) {
			result = 2;
			snprintf(temp, STR_MAX_SIZE,
				 "kill return error: %d (%s)", errno, "1");
			logmsg(temp);
			//sys_nerr
		} else {
			logmsg(_T
			       ("Wait for squid log rotated and file access.log is 0 size ..."));
			if (!squid_logconf()) {	// Если собрано в Debian - не ждать.
				while ((finfo.st_size != 0)
				       || (!(finfo.st_size < logsize))) {
					sleep(1);
					stat(squid_filename, &finfo);
				}	// end while - можем получить бесконечный цикл.
			}	// end !squid_logcong()
		}
	};

	/* sleep for 30 sec for rotating purposes */
	sleep(30);
	snprintf(temp, STR_MAX_SIZE, "%s/sacc/etc/monthly.sql", PREFIX);
	if (1 == sql_exec(temp)) {
		logerr("monthly job: sql execution error");
		result = 2;
	} else {
		if (squid_logconf()) {
			// Если мы в Debian - вернуть указатель в базе на место.
			squid_setlogoffset(offset, &mysql);
		} else
			offset = 0;
	}
	fseeko(fp, offset, SEEK_SET);
	logmsg(_T("monthly job: ended."));
	return result;
}

/* hints output */
static void usage()
{
	printf("SAcc counter v %s [%s]\n", VERSION, WWW_SITE);
	printf("Reasonable:\n");
	printf("\t-h: This help and exit.\n");
	printf("\t-d: No daemonize. Log messages going to stdout.\n");
	printf("\t-f: Filename to handle.\n\t    DEFAULT is: %s\n",
	       SQUID_LOGNAME);
	printf
	    ("\t-s: No offset usage. Just handle the logfile at all and quit.\n");
	printf("\t-e: Extend log messages.\n");
	printf("\t-l: Don't log with syslog, just to stdout.\n");
	printf("\t-a: Autolearn users from access.log file.\n");
	printf("\n");
	exit(1);
}

/* test for free subnet match, still unused */
/*int insubnet (char *ip, char *subnet) // true(1) if match [Greyder]
{
        u_int32_t       addr, net, mask;
        int bits;
        inet_net_pton(AF_INET, ip, &addr, sizeof(u_int32_t));
        bits = inet_net_pton(AF_INET, subnet, &net, sizeof(u_int32_t));
        mask = 0xffffffff << (32-bits);
        addr = ntohl (addr);
        net = ntohl (net);
        return (((addr&mask) == net));
} */
/* db pinging, is they alive? */
static void sql_dbping()
{
	if (mysql_ping(&mysql)) {
		logcrt(_T("Database Ping failed!"));
		exit_all();
	}
}

void squid_reconfig()
{				/* config generation and squid reconfiguration routine */
//!надо переделать с учетом колонки статус.
#ifdef IP_STAT
	return;
#endif

#ifdef DEBUG
	printf("entering reconfig\n");
#endif
	FILE *fr;
	FILE *fw, *fg;
	char *line_buffer = _V2PC(malloc(MAXLEN * SIZEOF_CHAR));
	char *out_buffer = _V2PC(malloc(MAXLEN * SIZEOF_CHAR));
	char *conv_buff = _V2PC(malloc(MAXLEN * SIZEOF_CHAR));
	int len;
	int current_record = 0;
	int rows_selected = 0, acl_rows = 0;
	pid_t squid_pid;
	//char *tmpname;
	std::string tmpname;	// Временная для работы ConvertKrbName
	std::string strconv;
#ifdef DEBUG
	printf("reconfig: query malloc\n");
#endif
	MYSQL_RES *res = NULL, *acl_res = NULL;
	MYSQL_ROW row = NULL, acl_row = NULL;
#ifdef DEBUG
	printf("reconfig: fopen %s r\n", SQUID_ORIG_CONF);
#endif
	if (NULL == (fr = fopen(SQUID_ORIG_CONF, "r"))) {
		snprintf(temp, STR_MAX_SIZE,
			 "reconfig: Can't open %s for read.", SQUID_ORIG_CONF);
		logcrt(temp);
		sig_shutdown(1);
		return;
	}
#ifdef DEBUG
	printf("reconfig: fopen %s w\n", SQUID_CONF);
#endif
	if (NULL == (fw = fopen(SQUID_CONF, "w"))) {
		snprintf(temp, STR_MAX_SIZE,
			 "Can't open %s for write, skipping reconfiguration.",
			 SQUID_CONF);
		logerr(temp);
		return;
	}
#ifdef DEBUG
	printf("reconfig: squid_orig_processing\n");
#endif
	while (!feof(fr)) {
		len = MAXLEN - 20;
		squid_loffset = ftello(fr);
		fgets(line_buffer, len, fr);
		if (errno == EINTR)
			while (errno == EINTR) {
				fseeko(fr, squid_loffset, SEEK_SET);
				fgets(line_buffer, len, fr);
			}
		if (strstr(line_buffer, "http_access deny all")) {
#ifdef DEBUG
			printf("reconfig: creating_configuration\n");
#endif
/*
*
acl group_time1900 proxy_auth "/usr/local/squid/etc/ACLs/group_time1900.user"
http_access allow group_time1900
*
*/
			if (mysql_query
			    (&mysql, "select id, sysname, data from acl")) {
				/* log message about error */
				snprintf(temp, STR_MAX_SIZE,
					 "logrotate: MySQL Error - %d: %s\n",
					 mysql_errno(&mysql),
					 mysql_error(&mysql));
				logcrt(temp);
				exit_all();
			}
			acl_res = mysql_store_result(&mysql);
			if (acl_res == NULL) {
				logcrt(_T("squid_reconfig: no result"));
				exit_all();
			};
			acl_rows = mysql_num_rows(acl_res);
			if (acl_rows == 0) {
				logcrt(_T("squid_reconfig: no acl's found"));
				exit_all();
			};
			for (int acl_current_record = 0;
			     acl_current_record < acl_rows;
			     acl_current_record++) {
				acl_row = mysql_fetch_row(acl_res);
				snprintf(out_buffer, STR_MAX_SIZE, "#acl %s\n",
					 acl_row[1]);
				fputs(out_buffer, fw);
				snprintf(out_buffer, STR_MAX_SIZE, "%s\n",
					 acl_row[2]);
				fputs(out_buffer, fw);
				snprintf(temp, STR_MAX_SIZE, "%s.%s",
					 SQUID_CONF, acl_row[1]);
//                      snprintf(out_buffer,STR_MAX_SIZE,"acl group_%s proxy_auth \"%s\"\n", acl_row[1], temp);
//                      fputs(out_buffer, fw);

				if (NULL == (fg = fopen(temp, "w"))) {
					snprintf(temp, STR_MAX_SIZE,
						 "Can't open %s.%s for write, skipping reconfiguration.",
						 SQUID_CONF, acl_row[0]);
					logerr(temp);
					return;
				}
				snprintf(sql_query, STR_MAX_SIZE,
					 "select login from users where (used<quota or quota=0) and (status=0) and aid=%s",
					 acl_row[0]);
				if (mysql_query(&mysql, sql_query)) {
					/* log message about error */
					snprintf(temp, STR_MAX_SIZE,
						 "logrotate: MySQL Error - %d: %s\n",
						 mysql_errno(&mysql),
						 mysql_error(&mysql));
					logcrt(temp);
					exit_all();
				}
				res = mysql_store_result(&mysql);
				if (res == NULL) {
					logcrt(_T("squid_reconfig: no result"));
					exit_all();
				};
				rows_selected = mysql_num_rows(res);
				if (rows_selected > 0) {
					snprintf(out_buffer, STR_MAX_SIZE,
						 "acl group_%s proxy_auth \"%s\"\n",
						 acl_row[1], temp);
					fputs(out_buffer, fw);
					snprintf(out_buffer, STR_MAX_SIZE,
						 "http_access allow group_%s %s\n",
						 acl_row[1], acl_row[1]);
					fputs(out_buffer, fw);
#if FREE_HTTPS
					snprintf(out_buffer, STR_MAX_SIZE,
						 "http_access allow group_%s CONNECT\n",
						 acl_row[1]);
					fputs(out_buffer, fw);
#endif				/* FREE_HTTPS */

				}
				// Ales:
				// Вот в этом цикле и вносятся сами логины в файл, типа 'squid.conf.fulltime'
				//char *tmpname = _V2PC(malloc(STR_MAX_SIZE));  // login from users
				for (current_record = 0;
				     current_record < rows_selected;
				     current_record++) {
					row = mysql_fetch_row(res);
					if (row == NULL) {
						logcrt(_T
						       ("squid_reconfig: no row"));
						exit_all();
					};
					snprintf(out_buffer, STR_MAX_SIZE,
						 "%s\n", row[0]);
					// его и будем конвертировать.
					snprintf(conv_buff, STR_MAX_SIZE,
						 "%s", row[0]);

					fputs(out_buffer, fg);
					// Тут вносим(добавляем!) наш измененный логин, если было 'domain\username'
					// то станет 'username@DOMAIN.TIPA.RU', т.е. проведем обратное преобразование.
					//tmpname = ConvertKrbName(out_buffer, false);
					//tmpname = ConvertKrbName(strncpy(tmpname, out_buffer, strlen(out_buffer)-1), false);
					//char *tmpname;        // = new char[MAXLEN];
					strconv.assign(conv_buff);
					const std::string & tmpname =
					    ConvertKrbName(strconv, false);
					if (!tmpname.empty()) {
						snprintf(out_buffer,
							 STR_MAX_SIZE, "%s\n",
							 tmpname.c_str());
#ifdef DEBUG
						std::clog <<
						    "Обратная обработка: "
						    << tmpname << std::endl;
#endif
						fputs(out_buffer, fg);
					}
#ifdef DEBUG
					else {
						std::clog << "Имя: " <<
						    conv_buff <<
						    " _не_стали_ обрабатывать!"
						    << std::endl;
					}
#endif
					//delete[]tmpname;      //
					/*

					   tmpname = ConvertKrbName(row[0], false);

					   if (strlen(row[0]) > 1) {
					   snprintf(tmpname, STR_MAX_SIZE,
					   ConvertKrbName(row[0],
					   false));
					   if ((NULL != tmpname) && strlen(tmpname) > 1) {
					   snprintf(out_buffer,
					   STR_MAX_SIZE,
					   "%s\n",
					   tmpname);

					   loginf(out_buffer);
					   //loginf("Output!");
					   //fputs(out_buffer, fg);
					   }
					   } */

					//fputs(out_buffer, fg);

				};
				//loginf("Afrer for delete tmpname");
				mysql_free_result(res);
				fclose(fg);

				//!
			}
		}
		fputs(line_buffer, fw);
	}
	fclose(fw);
	fclose(fr);
/* ok, let's reconfigure squid */
/* part of main.c [squid-2.5stable3]
        case 'k':
            if ((int) strlen(optarg) < 1)
                usage();
            if (!strncmp(optarg, "reconfigure", strlen(optarg)))
                opt_send_signal = SIGHUP;
            else if (!strncmp(optarg, "rotate", strlen(optarg)))
#ifdef _SQUID_LINUX_THREADS_
                opt_send_signal = SIGQUIT;
#else
                opt_send_signal = SIGUSR1;
#endif
            else if (!strncmp(optarg, "shutdown", strlen(optarg)))
                opt_send_signal = SIGTERM;
            else if (!strncmp(optarg, "interrupt", strlen(optarg)))
                opt_send_signal = SIGINT;
            else if (!strncmp(optarg, "kill", strlen(optarg)))
                opt_send_signal = SIGKILL;
*/
#ifdef DEBUG
	printf
	    ("reconfig: configuration written, let's signal squid reload it\n");
#endif
	squid_pid = squid_getpid();
	if (squid_pid < 2) {
		logmsg(_T
		       ("squid_reconfig: SquiD PID invalid. Silently skip reconfiguration..."));
	} else {
		kill(squid_pid, SIGHUP);
	};			/* reconfigure squid */
#ifdef DEBUG
	printf("reconfig: free memory, and leave...\n");
#endif
	free(line_buffer);
	free(out_buffer);
	logmsg(_T("squid_reconfig: complete."));
	return;
}

/* SR sql_update_cmd_status
 * out: 0 = success
 * 1 = error
 */

int sql_update_cmd_status(MYSQL * lmysql, int cmd_id, int status_id)
{
	time_t current_time;
	time(&current_time);
	snprintf(sql_query, STR_MAX_SIZE,
		 "update queue set complete=%d, rtime=%d where id=%d",
		 status_id, (int)current_time, cmd_id);
	if (mysql_query(lmysql, sql_query)) {
		/* log message about error */
		snprintf(temp, STR_MAX_SIZE,
			 "sql_update_cmd_status: MySQL Error - %d: %s\n",
			 mysql_errno(lmysql), mysql_error(lmysql));
		logerr(temp);
		return 1;
	}
	if (0 == mysql_affected_rows(lmysql)) {
		snprintf(temp, STR_MAX_SIZE,
			 "sql_update_cmd_status: id=%d, status=%d, query (%s)\n",
			 cmd_id, status_id, sql_query);
		logerr(temp);
		return 1;
	}
	return 0;
}

/* conditions testing, for rotating or reconfiguring */
void check_state()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int need_reconf = 0;
	struct tm *timing;
	time_t tmp;

#ifdef DEBUG
	printf("entering check state \n");
#endif
	time(&tmp);
	timing = localtime(&tmp);
#if SAFE_LOGROTATE
	if (logrotated != (timing->tm_year * 100 + timing->tm_mon))
//              if ((logrotated!=(timing->tm_year*100+timing->tm_mon))&&(1==timing->tm_mday)&&(timing->tm_hour==0))
//    if ((timing->tm_min==0)&&(timing->tm_hour==0)&&(1==timing->tm_mday)&&(logrotated!=timing->tm_mon))
#else
//    if ((1==timing->tm_mday)&&(logrotated!=timing->tm_mon))
	if (logrotated < (timing->tm_year * 100 + timing->tm_mon))
#endif
	{
#ifdef DEBUG
		printf("entering logrotation!\n");
#endif
		logrotated = timing->tm_year * 100 + timing->tm_mon;
		snprintf(temp, STR_MAX_SIZE,
			 "insert delayed into queue (command, itime) values (3, %d)",
			 (int)tmp);
		if (mysql_query(&mysql, temp)) {
			/* log message about error */
			snprintf(temp, STR_MAX_SIZE,
				 "time_check: MySQL Error - %d: %s\n",
				 mysql_errno(&mysql), mysql_error(&mysql));
			logerr(temp);
			exit_all();
		}
		//squid_logrotate(1);
	};
#ifdef DEBUG
	printf("before total \n");
#endif
/* getting total users */
	snprintf(sql_query, STR_MAX_SIZE, "select count(*) from users");
	sql_dbping();
	if (mysql_query(&mysql, sql_query)) {
		/* log message about error */
		snprintf(temp, STR_MAX_SIZE,
			 "check_state_1: MySQL Error - %d: %s\n",
			 mysql_errno(&mysql), mysql_error(&mysql));
		logerr(temp);
		exit_all();
	}
	res = mysql_store_result(&mysql);
	if (res == NULL) {
		logerr(_T("check_state_1: no result"));
		exit_all();
	};
	row = mysql_fetch_row(res);
	if (row == NULL) {
		logerr(_T("check_state_1: no row"));
		exit_all();
	};
	if (users_total != atoi(row[0])) {
		users_total = atoi(row[0]);
		need_reconf = 1;
	};
	mysql_free_result(res);
#ifdef DEBUG
	printf("after total users \n");
#endif
/* getting trusted users */
	if (mysql_query
	    (&mysql,
	     "select count(*) from users where used<quota or quota=0")) {
		/* log message about error */
		snprintf(temp, STR_MAX_SIZE,
			 "check_state_2: MySQL Error - %d: %s\n",
			 mysql_errno(&mysql), mysql_error(&mysql));
		logerr(temp);
		exit_all();
	}
	mysql_query(&mysql, sql_query);

	if (NULL == (res = mysql_store_result(&mysql))) {
		logerr(_T("check_state_2: no result"));
		exit_all();
	};
	if (NULL == (row = mysql_fetch_row(res))) {
		logerr(_T("check_state_2: no row"));
		exit_all();
	};
	if (users_positive != atoi(row[0])) {
		users_positive = atoi(row[0]);
		need_reconf = 1;
	};
	mysql_free_result(res);
#ifdef DEBUG
	printf("after trusted users\n");
#endif
/* restricted users */
	if (mysql_query
	    (&mysql,
	     "select count(*) from users where used>quota and quota<>0")) {
		/* log message about error */
		snprintf(temp, STR_MAX_SIZE,
			 "check_state_3: MySQL Error - %d: %s\n",
			 mysql_errno(&mysql), mysql_error(&mysql));
		logerr(temp);
		exit_all();
	}
//    res=mysql_store_result(&mysql);
	if (NULL == (res = mysql_store_result(&mysql))) {
		logerr(_T("check_state_3: no result"));
		exit_all();
	};
	if (NULL == (row = mysql_fetch_row(res))) {
		logerr(_T("check_state_3: no row"));
		exit_all();
	};
	if (users_negative != atoi(row[0])) {
		users_negative = atoi(row[0]);
		need_reconf = 1;
	};
#ifdef DEBUG
	printf("after negative users \n");
#endif
	mysql_free_result(res);

	if (mysql_query
	    (&mysql,
	     "select id, command from queue where complete=0 limit 1")) {
		/* log message about error */
		snprintf(temp, STR_MAX_SIZE,
			 "check_state_4: MySQL Error - %d: %s\n",
			 mysql_errno(&mysql), mysql_error(&mysql));
		logerr(temp);
		exit_all();
	}
	if (NULL == (res = mysql_store_result(&mysql))) {
#ifdef DEBUG
		printf("empty queue, skip...\n");
#endif
	} else {
		if (NULL == (row = mysql_fetch_row(res))) {
#ifdef DEBUG
			printf("empty row in queue, skip...\n");
#endif
		} else {
			/*
			 * --- commands
			 * 0 - test
			 * 1 - reconf
			 * 2 - recount
			 * 3 - month job
			 * --- status
			 * 1 = processing
			 * 2 = complete
			 * 3 = done with errors
			 * 4 = error
			 * 5 = skipped
			 */
			int cres = 0;
			int cmd = 0;
			int cid = 0;
			cmd = atoi(row[1]);
			cid = atoi(row[0]);
			switch (cmd) {
			case 0:
				cres = 2;
				break;
			case 1:
				cres = 1;
				if (1 ==
				    sql_update_cmd_status(&mysql, cid, cres)) {
					logerr
					    ("queue command status update failed.");
				};
				squid_reconfig();
				cres = 2;
				break;
			case 2:
				cres = 1;
				if (1 ==
				    sql_update_cmd_status(&mysql, cid, cres)) {
					logerr
					    ("queue command status update failed.");
				};
				(1 == squid_recount(&mysql)) ? cres = 3 : cres =
				    2;
				break;
			case 3:
				cres = 1;
				if (1 ==
				    sql_update_cmd_status(&mysql, cid, cres)) {
					logerr
					    ("queue command status update failed.");
				};
				(1 == squid_logrotate())? cres = 3 : cres = 2;
				break;
				//here
			default:
				cres = 5;
			}	/* of switch */
			if (1 == sql_update_cmd_status(&mysql, cid, cres)) {
				logerr("queue command status update failed.");
			};
		}
		mysql_free_result(res);
	}
	if (need_reconf == 1) {
#ifdef DEBUG
		printf("before reconfiguring \n");
#endif
		squid_reconfig();
		need_reconf = 0;
	};
}

/* RegEx matching */
/*int re_match (char *pattern, char *string)
{
        regex_t re;
        int res=0;

        regcomp (&re, pattern, REG_ICASE|REG_NOSUB);
        if (0==regexec (&re, string, 0, 0, 0)) res = 1;
        regfree(&re);
        return (res);
} */

bool AutoAddUser(const char *login, int &uid)
{
	static MYSQL_RES *result;
	static MYSQL_ROW row;
	std::clog << "Login test in AutoAddUser: " << login << std::endl;
	snprintf(sql_query, STR_MAX_SIZE,
		 "select id from users where login='%s';", login);
	if (mysql_query(&mysql, sql_query)) {
		// log message about error
		snprintf(temp, STR_MAX_SIZE,
			 "parser: mysql error, user: %s query: %s error %s",
			 login, sql_query, mysql_error(&mysql));
		logmsg(temp);
		errno = 5;
		return false;	//no user found
	}
	result = mysql_store_result(&mysql);
	if (0 == mysql_num_rows(result)) {	// Если юзера в базе нету.
		if (NULL != result)
			mysql_free_result(result);
		// Если режим автодобавления
		if (1 == autolearn) {
			snprintf(temp, STR_MAX_SIZE, "Add autolearn user: %s\n",
				 login);
			loginf(temp);
			snprintf(sql_query, STR_MAX_SIZE,
				 "insert delayed into users (aid, quota, dquota, login, descr) select o1.value as aid, o2.value as lim1 \
				 ,o2.value as lim2, '%s', 'autocreated' from options as o1, options as o2  \
				 where o1.name='def_timeacl' and o2.name='std_limit'",
				 login);
			if (mysql_query(&mysql, sql_query)) {
				// log message about error
				snprintf(temp, STR_MAX_SIZE,
					 "parser: autoadd fail, user: %s, query: %s, error: %s",
					 login, sql_query, mysql_error(&mysql));
				logmsg(temp);
				errno = 5;
				return false;	//no user found
			}
#ifdef DEBUG
			std::clog << "User " << login <<
			    " успешно добавлен в базу пользователей"
			    << std::endl;
#endif
			uid = mysql_insert_id(&mysql);
		} else {	// Так можно и весь лог забить(!) при неверной конфигурации squid.conf
			snprintf(temp, STR_MAX_SIZE,
				 "parser: no user: %s query: %s error %s",
				 login, sql_query, mysql_error(&mysql));
			logmsg(temp);
			errno = 6;
			return false;
		}
	} else {
		row = mysql_fetch_row(result);
		// uid будет возвращен по ссылке
		uid = atoi(row[0]);
		mysql_free_result(result);
	}
	return true;
}

/* main string parsing */
static bool parse_string(char *s, size_t len)
{
	char *out, *point = NULL, *http_query, *host = NULL;
	char **field;
	char *fields[10];
	static char login[512], site[1024], url[4096];
	static int login_size, site_size, url_size;
	int affect = 0;
	int fieldnum = 0;
	bool cached = false;
	unsigned long ip = 0;
	static int uid;
	std::string strconv;
	bool chdeni = false;	// Признак что строка содержит "DENIED"
	//1040314641.655     36 10.0.0.200 TCP_DENIED/407 1691 GET http://www.wzor.net/ - NONE/- text/html
	//[0]-time
	//[1]-conn_time
	//[2]-ip
	//[3]-code
	//[4]-size
	//[5]-method
	//[6]-query
	//[7]-user
	//[8]-mode/addr
	//[9]-content type

#ifdef DEBUG
	printf("parser: string %s len=%d\n", s, len);
#endif
	if (len > 16) {
		out = s;
		// strsep заменена макросом
		for (field = fields; (*field = strsep(&out, " \t")) != NULL;)
			if (**field != '\0')
				if (++field >= &fields[10])
					break;
		if (fields[0] != NULL)
			point = strchr(fields[0], '.');
/* first parametr MUST contain ".", very simple test... */
		if (point != NULL)
			*point = '\0';
		else {
			errno = 1;
			return false;
		};
		for (fieldnum = 0; fieldnum < 10; fieldnum++)
			if (fields[fieldnum] == NULL)
				return false;
/* advanced test, if enabled (default) - reduce perfomance */
#ifndef FAST
/* IP address must contain 3 dot's, test about one */
		if (strchr(fields[2], '.') == NULL) {
			errno = 2;
			return false;
		}
/* code [3] must contain "/" */
		if (strchr(fields[3], '/') == NULL) {
			errno = 3;
			return false;
		}
/* mode [8] must contain "/" */
		if (strchr(fields[8], '/') == NULL) {
			errno = 4;
			return false;
		}
#endif				/* FAST switch */
		point = NULL;
		point = strstr(fields[6], "//");
		if (point != NULL) {
			point = strchr(point + 2, '/');
			if (point != NULL) {
//                valid = 1;
//                point++;
				host = temp;
				http_query = point;
				strncpy(host, fields[6], point - fields[6] + 1);
				host[point - fields[6] + 1] = '\0';
			}
		} else {
			http_query = fields[6];
			host = fields[6];
		}
		if (atol(fields[4]) == 0)
			return true;
		if (strstr(fields[8], "NONE/-") != NULL) {
			if (0 == autolearn)
				return true;
		}
		if (strstr(fields[3], "NONE/") != NULL)
			return true;
#ifndef IP_STAT
		_nstrcmp(fields[7], _T("-")) return true;
#endif
		_strcmp(fields[8], _T(INT_SUBNET1)) return true;
		_strcmp(fields[8], _T(INT_SUBNET2)) return true;
		_strcmp(fields[8], _T(INT_SUBNET3)) return true;
		// При таком раскладе, если юзера в базе нет то и создаваться он не будет при autolearn=1!
		//printf("Добрались до DENIED при autolearn=%d\n", autolearn);
		if (strstr(fields[3], "DENIED") != NULL) {
			chdeni = true;
			if (0 == autolearn)	// Если нет режима автосоздания пользователя - прекращаем эту функцию.
				return true;
		}
//#ifndef CHEAT_MODE
		if (strstr(fields[3], "HIT") != NULL)
			cached = true;
//#endif /* cheat */
#ifdef DEBUG
		printf("parser: all fields looks like valid\n");
#endif
		static int uidcache;
		uidcache = 0;	//miss
#ifndef IP_STAT
#ifndef DEBIAN
		login_size =
		    mysql_real_escape_string(&mysql, login, fields[7],
					     strlen(fields[7]));
#else
		strconv.assign(fields[7]);
		std::string tmplog = ConvertKrbName(strconv, true);

		if (!tmplog.empty()) {
			login_size =
			    mysql_real_escape_string(&mysql, login,
						     tmplog.c_str(),
						     tmplog.size());
		} else {
			login_size =
			    mysql_real_escape_string(&mysql, login, fields[7],
						     strlen(fields[7]));
		}
		tmplog.erase();

#endif				// End DEBIAN
#else				// Else in IP_STAT

		login_size =
		    mysql_real_escape_string(&mysql, login, fields[2],
					     strlen(fields[2]));
#endif				// End IP_STAT

#ifdef DEBUG
		printf("field %s login(%d) %s \n", fields[7], login_size,
		       login);
#endif

		if (!chdeni) {
			for (int i = 0; (i < uccsize) && (0 == uidcache); i++) {
				_nstrcmp(login, (ucache[i].uname)) {
					uidcache = 1;
					uid = ucache[i].uid;
#ifdef DEBUG
					std::clog << "Найден uid=" << uid
					    << " для юзера: " << login
					    << std::endl;
#endif
#ifdef STAT
					cache_hit++;
#endif
					break;
				}
			}
		}		// end if(chdeni)

		if (0 == uidcache)	//cache miss, select uid from database
		{
			//static MYSQL_RES *result;
			//static MYSQL_ROW row;
#ifdef STAT
			cache_miss++;
#endif
			// Если пользователя нет и нет режима автодобавления то на выход.
			if (!AutoAddUser(login, uid)) {
				return false;
			}
			if (chdeni)	// Если ранее был обнаружен DENIED то выход
				return true;
			//save data to internal cache
			ucache[ucpointer].uid = uid;
			strncpy(ucache[ucpointer].uname, login, STR_MAX_SIZE);
			if (USER_CACHE_SIZE > ucpointer) {
				ucpointer++;
				if (USER_CACHE_SIZE == ucpointer)
					ucpointer = 0;
			};
			if ((USER_CACHE_SIZE - 1) > uccsize) {
				uccsize++;
			};
		}
		cache_lines++;
		{
#ifndef CHEAT_MODE
			if (!cached)
#endif				/* cheat */
			{
				snprintf(sql_query, STR_MAX_SIZE,
					 "update users set used=used+%s where id=%d;",
					 fields[4], uid);
#ifdef DEBUG
				printf("parser: updating users %s\n",
				       sql_query);
#endif				/* DEBUG */
				mysql_query(&mysql, sql_query);
				if (0 == mysql_affected_rows(&mysql)) {
					logerr(_T("parser: no user found\n"));
					snprintf(temp, STR_MAX_SIZE,
						 "no user found: %d query: %s\n",
						 uid, sql_query);
					logerr(temp);
					errno = 6;
					return false;
				};
				site_size =
				    mysql_real_escape_string(&mysql, site, host,
							     strlen(host));
				snprintf(sql_query, STR_MAX_SIZE,
					 "update site set size=size+%s, lutime=%s where u_id=%d and site='%s';",
					 fields[4], fields[0], uid, site);
#ifdef DEBUG
				printf
				    ("parser: affected=%d, updating site %s\n",
				     affect, sql_query);
#endif				/* DEBUG */
				mysql_query(&mysql, sql_query);
				affect = mysql_affected_rows(&mysql);
				if (0 == affect) {	/* new host, add to table */
					snprintf(sql_query, STR_MAX_SIZE,
						 "insert delayed into site (size, u_id, site, lutime, futime) values ( %s, %d,'%s', %s, %s );",
						 fields[4], uid, site,
						 fields[0], fields[0]);
#ifdef DEBUG
					printf
					    ("parser: affected=%d, inserting into site, %s\n",
					     affect, sql_query);
#endif				/* DEBUG */
					mysql_query(&mysql, sql_query);
				};
#ifdef DEBUG
				if (affect > 1) {
					printf("%d %s %s", affect, login, site);
					logmsg(_T
					       ("parser: warning, more than one items affected in table site\n"));
					logmsg(sql_query);
				};
#endif				/* DEBUG */
			}
			if (1 == detailed) {
				url_size =
				    mysql_real_escape_string(&mysql, url,
							     fields[6],
							     strlen(fields[6]));
				ip = ntohl(inet_addr(fields[2]));
				snprintf(sql_query, STR_MAX_SIZE,
					 "insert delayed into detail (utime, qtime, ip_addr, code, size, method, url, u_id, server) values (%s,%s,%lu,'%s',%s,'%s','%s',%d,'%s')",
					 fields[0], fields[1], ip, fields[3],
					 fields[4], fields[5], url, uid,
					 fields[8]);
#ifdef DEBUG
				printf("parser: inserting into detail: %s\n",
				       sql_query);
#endif				/* DEBUG */
				mysql_query(&mysql, sql_query);
#ifdef DEBUG
				if (0 == mysql_affected_rows(&mysql)) {
					logerr(_T
					       ("can't insert detailed record, look for db permissions!\n"));
				};
#endif				/* DEBUG */
			};
		};
	};
	return true;
}

/* main function */
int main(int argc, char *argv[])
{
	tmpbf = _V2PC(malloc(MAXLEN * SIZEOF_CHAR));

#ifdef STAT
	sys_gettime(&start_time1);
#endif
	for (int i = 0; i < USER_CACHE_SIZE; i++)
		ucache[i].uname = _V2PC(malloc(MAXBUFSIZE));

#ifdef DEBUG
	time_t start_time, stop_time;
#endif				/*debug */
	char *s = _V2PC(malloc(MAXBUFSIZE));
	size_t len;
	int a_char;
	struct stat finfo;
/* arguments parsing... */
	while ((a_char = getopt(argc, argv, "nahdf:slD:"
#ifdef IS_FIFO
				"x:"
#endif
		)) != -1)
		switch (a_char) {
		case 'd':
			config_nodaemon = 1;
			break;
		case 'a':
			loginf("Use autolearn");
			autolearn = 1;
			break;
		case 'f':
			squid_filename = optarg;
//                                              no_seek = 1;
			break;
		case 's':
			config_noseek = 1;
			break;
		case 'l':
			config_insyslog = false;
			break;
		case 'e':
			loglevel = 7;
			break;
		case '?':
		case 'h':
		default:
			usage();
		}		/* switch */
	argc -= optind;
	argv += optind;
	logger.log(SYSLOG_NAME, config_insyslog, loglevel);	// set 7 for debug output
	if (squid_filename == NULL)
		squid_filename = SQUID_LOGNAME;	/* default filename */
/* Checking for runned program */
	if (checkRunningPid() > 0)
		exit(1);

#ifdef IS_FIFO
	for (;;) {
		if ((opt_acfd = open(opt_acfifo, O_WRONLY)) < 0) {
			if (errno == EINTR)
				continue;
			if (errno == ENOENT && mkfifo(opt_acfifo, 0644) < 0)
				err(1, opt_acfifo);
			continue;
		}
		break;
	}
#endif
#ifdef DEBUG
	printf("ready to daemonize...");
#endif				/* DEBUG */
	if (!config_nodaemon)
		sys_daemonize();
/* set system signals handlers */
	signal(SIGHUP, &sig_reconf);
	signal(SIGTERM, &sig_shutdown);
	signal(SIGUSR1, &sig_rotate);
	signal(SIGUSR2, &sig_recount);
/* getting pid */
	PID = getpid();
/* for debug */
#ifdef DEBUG
	printf("%s process started with PID= %d\n", SYSLOG_NAME, PID);
#endif				/* DEBUG */
	if (PidFile_write() < 0)
		exit(1);
/* Log message about what we're started */
//snprintf(temp,STR_MAX_SIZE,"%s service v%s - %d was started.\n",SYSLOG_NAME,VERSION,PID);
	snprintf(temp, STR_MAX_SIZE,
		 "%s service v%s - %d was started. off_t size %d \n",
		 SYSLOG_NAME, VERSION, PID, sizeof(offset));
	logmsg(temp);
#ifdef DEBUG
	printf("main: init MySQL struct...\n");
#endif				/* DEBUG */
/* initializing MySQL structure */
	if (!mysql_init(&mysql)) {
/* log message about error */
		logerr(_T("Unable to initialize MySQL struct"));
		ErrNumber = 1;
		exit_all();
	}
/* try connect to MySQL server'n'select db */
	sql_connect(&mysql);
#ifdef DEBUG
	printf("main: connected...\n");
#endif				/* DEBUG */
	/* try to get offset from table */
	if (!config_noseek)
		offset = get_offset();
	else
		offset = 0;
#ifdef DEBUG
	printf("main: Offset from DB = %llu\n", offset);
#endif				/* DEBUG */
// get last logrotation time
	logrotated = get_lastlogrotation(&mysql);
#ifdef DEBUG
	printf("main: logrotation: %d\n", logrotated);
#endif				/* DEBUG */

	/* Going to endless cycle */
	s = _V2PC(malloc(MAXBUFSIZE * SIZEOF_CHAR));
	/* opennig access.log file */
	if (NULL == (fp = fopen(squid_filename, "rm"))) {
#ifdef DEBUG
		err(1, "logfile open0");
#endif				/* DEBUG */
		logerr(squid_filename);
		logerr(_T("Can't open logfile for read."));
		exit_all();
	}
	setvbuf(fp, tmpbf, _IOLBF, MAXBUFSIZE);
	/* seek */
	fseeko(fp, offset, SEEK_SET);

	while (1) {
//        if (got_recount==1) {offset=get_offset(); got_recount=0;}
		/* checking for termination signal */
//        if (Sig_do_shutdown>0) exit_mysql();
#ifdef DEBUG
		printf("main: cycle start\n");
#endif				/* DEBUG */
		cycle_counter++;
		if (cycle_counter > USER_TEST_DELAY) {
			sql_dbping();
			check_state();
			cycle_counter = 0;
#ifdef DEBUG
			printf("main: check signals\n");
#endif				/* DEBUG */
			switch (sig_num) {
			case SIG_RECONFIG:
				{
					fclose(fp);
#ifndef IP_STAT
					logmsg(_T("signal: reconfiguration"));
					squid_reconfig();
#endif
					if (NULL ==
					    (fp = fopen(squid_filename, "r"))) {
#ifdef DEBUG
						err(1, "logfile open2");
#endif				/* DEBUG */
						logerr(squid_filename);
						logerr(_T
						       ("Can't open logfile for read."));
						exit_all();
					}
					setvbuf(fp, tmpbf, _IOLBF, MAXBUFSIZE);
					break;
				}
			case SIG_RECOUNT:
				{
					squid_recount(&mysql);
					fclose(fp);
					logmsg(_T("signal: recount"));
					squid_reconfig();
					if (NULL ==
					    (fp = fopen(squid_filename, "r"))) {
#ifdef DEBUG
						err(1, "logfile open3");
#endif				/* DEBUG */
						logerr(squid_filename);
						logerr(_T
						       ("Can't open logfile for read."));
						exit_all();
					}
					setvbuf(fp, tmpbf, _IOLBF, MAXBUFSIZE);
//                      squid_reconfig();
					break;
				}
			case SIG_SHUTDOWN:
				logmsg(_T("signal: shutdown"));
				offset = ftello(fp);
				squid_setlogoffset(offset, &mysql);
				exit_mysql();
				break;
			case SIG_ROTATE:
				{
					squid_logrotate();
					fclose(fp);
					logmsg(_T("signal: rotate"));
					squid_reconfig();
					if (NULL ==
					    (fp = fopen(squid_filename, "r"))) {
#ifdef DEBUG
						err(1, "logfile open4");
#endif				/* DEBUG */

						logerr(squid_filename);
						logerr(_T
						       ("Can't open logfile for read."));
						exit_all();
					}
					setvbuf(fp, tmpbf, _IOLBF, MAXBUFSIZE);
					break;
				}
				//sigrotate need
			}	/* switch */
			sig_num = 0;	// reset signals
		}
		/* getting information about access.log file */
		stat(squid_filename, &finfo);
		/* checking access to access.log file */
		if (!(finfo.st_mode & S_IRUSR)) {
#ifdef DEBUG
			err(1, "logfile access");
#endif				/* DEBUG */
			logerr(squid_filename);
			logerr(_T("Can't get access for read logfile"));
			exit_all();
			break;
		}
		// Если произошло усечение лога внешними воздействиями.
		// Ales999
		if (offset > finfo.st_size) {
#ifdef DEBUG
			snprintf(temp, STR_MAX_SIZE,
				 "Size %llu, Current size: %llu ", offset,
				 finfo.st_size);
			logmsg(temp);
			logmsg(_T("- detected EXTERNAL logrotation."));
#endif
			fclose(fp);
			sleep(1);
			if (NULL == (fp = fopen(squid_filename, "r"))) {
				logerr(squid_filename);
				logerr(_T("Can't open logfile for read."));
				exit_all();	// Гм, а если еще идет работа ?
				break;
			}
			setvbuf(fp, tmpbf, _IOLBF, MAXBUFSIZE);
			offset = 0;
			squid_setlogoffset(offset, &mysql);
			//in case of error, we never reach this line...
			fseeko(fp, offset, SEEK_SET);
			squid_loffset = ftello(fp);
			usleep(SLEEP_TIME);	/* default 50 msec */
		}
		/* if we've new cheanges, we must parse log */
		// BUGGY
		if (offset != finfo.st_size) {
			/* if log file is rotated, VERY BUGGY */
//              if(finfo.st_size<offset)
//              {
//                      offset=0;
//                      snprintf(temp, 4096, "size %llu", offset);
//                      logmsg(temp);logmsg(_T("detected EXTERNAL logrotation."));
//                      fclose(fp);
//                      /* opennig access.log file */
//                      if (NULL == (fp = fopen(squid_filename, "r")))
//                      {
#ifdef DEBUG
//err(1,"logfile open1");
#endif				/* DEBUG */

//                              logerr(squid_filename);logerr(_T("Can't open logfile for read.")); exit_all();
//                      }
//                      setvbuf(fp, tmpbf, _IOLBF, MAXBUFSIZE);
			/* seek */
//                      fseeko(fp,offset,SEEK_SET);
//                      continue;
//              }
#ifdef DEBUG
			snprintf(temp, STR_MAX_SIZE, "Working with offset: %ld",
				 offset);
			logmsg(temp);
			sys_gettime(&start_time);
#endif				/* DEBUG */
			/* seek */
			fseeko(fp, offset, SEEK_SET);
			/* getting new entrys from file */
			while (!feof(fp)
			       && (cycle_counter < USER_TEST_DELAY * 10)) {
				/* getting new line from file */
				cycle_counter++;
				if (NULL != _fgets(s, STR_MAX_SIZE, fp)) {
					/* process line */
#ifdef DEBUG
					squid_loffset = ftello(fp);
//                              printf("before parsing offset %d\n",loffset);
#endif				/* debug */
					len = strlen(s);
#ifdef DEBUG
					printf
					    ("before parsing string: '%s'\nlen %d\n",
					     s, len);
#endif				/* debug */
					if (!parse_string(s, len)) {
/* send message about error to syslog */
						switch (errno) {
						case 1:
							snprintf(temp,
								 STR_MAX_SIZE,
								 "parser: string incorrect, not enought fields in line:\n");
							break;
						case 2:
							snprintf(temp,
								 STR_MAX_SIZE,
								 "parser: string incorrect, can't find '.' in timestamp. line:\n");
							break;
						case 3:
							snprintf(temp,
								 STR_MAX_SIZE,
								 "parser: string incorrect, can't find '/' in code. line:\n");
							break;
						case 4:
							snprintf(temp,
								 STR_MAX_SIZE,
								 "parser: string incorrect, can't find '/' in mode. line:\n");
							break;
						}	/* switch */
						logmsg(temp);
#ifdef DEBUG
						fseeko(fp, squid_loffset,
						       SEEK_SET);
						_fgets(s, MAXBUFSIZE - 1, fp);
						logerr(s);
						sleep(1);
#endif				/* debug */
					}
				}
/*
			if (errno==EINTR) while (errno==EINTR)
			{
				errno=0;
				fseeko(fp,loffset,SEEK_SET);
				continue;
			}
*/
#ifdef DEBUG
				squid_loffset = ftello(fp);
				printf("after offset %d\n", squid_loffset);
#endif				/* debug */
//                      if (cycle_counter>USER_TEST_DELAY*10) break; // let's process signals and saves data
			}	/*while feof */
			/* storing positon */
			offset = ftello(fp);
			squid_setlogoffset(offset, &mysql);
#ifdef DEBUG
			printf("before fclose offset %llu \n ", offset);
#endif				/* debug */
#ifdef TRANSACTION_SAFE
			/*for transaction safe compatibility */
			mysql_commit(&mysql);
#endif
			/* closing access.log file */
//              fclose(fp);
#ifdef DEBUG
			sys_gettime(&stop_time);
			snprintf(temp, STR_MAX_SIZE,
				 "\n\nNew offset is - %llu, it takes %d seconds.",
				 offset, stop_time - start_time);
			logmsg(temp);
//              printf("sleeping 10 sec\n");
//              sleep(5);
#endif				/* DEBUG */
		} /*if (offset!=finfo.st_size) */
		else {
			if (1 == config_noseek) {
				logmsg("self kill, logfile is over");
				exit_mysql();
			};
			/* start sleeping to wait new entry's in access.log file */
			sql_dbping();
			usleep(SLEEP_TIME);	/* default 50 msec */
		};
	}			/* while 1 */
}
