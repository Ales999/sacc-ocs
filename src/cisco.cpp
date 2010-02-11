/********************************************************************************
#           SAccounting                                            [SAcc system] 
#			   Copyright (C) 2003-2006  the.Virus
#		$Author: slavik $ $Date: 2006-05-26 01:00:16 +0600 (п÷я┌п╫, 26 п°п╟п╧ 2006) $
#		$Id: cisco.cpp 13 2006-05-25 19:00:16Z slavik $
#           -----------------------------------------------------
#   			This file is part of SAcc system.  
#           -----------------------------------------------------
#        ----------    сделано с особым цинизмом.   ----------------
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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
/* includes */
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
#ifdef HAVE_STDARG_H /* gcc 3 */
#include <stdarg.h>
#else /* gcc 2 */
#include <varargs.h>
#endif /* HAVE_STDARG_H */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <regex.h>
#include <errno.h>
/* configuartion */
#define MAXLEN 8*1024
#define DB_DATABASE_CISCO "cisco"
//#define DEBUG 1 /* define to 1 for debugging */
#define QUERY_BUFSIZE   8*1024
#define PING_TIMEOUT    60*5
/* counter sleep time msec */
#define SLEEP_TIME 50
/* user testing delay, max = 1 sec, or 20 strings */
#define USER_TEST_DELAY 20
#define MAXBUFSIZE 4*1024
#define APP_TITLE "SAcc-cisco"
#define APP_NAME "SAcc cisco RSH statistic"
#define _T(txt) (char*)(txt)
#define _V2PC(data) (char*)(data)
#define CISCO_LOGNAME "stat.rsh"
//#define _nstrcmp(str1, str2) if (*str1==*str2) if (strstr( str1, str2)!=NULL)
#define _strcmp(str1, str2) if (strstr(str1, str2)!=NULL)
#ifndef HAVE_STRSEP
#define strsep(str1, str2) getToken(str1,str2)
#endif
/* command var's */
int config_noseek = 0;
long offset=0;
int detailed = 1;
#ifndef DEBUG
int config_nodaemon = 1;
int config_nosyslog = 1;
#else
int config_nodaemon = 1;
int config_nosyslog = 1;
#endif
char *filename = NULL;
int exit_imm = 0;
int ErrNumber = 0;
struct tm *timer;
time_t cur_time;
int logrotated=0;
char temp[MAXLEN];
time_t ping_time;
/* System signals */
int Sig_do_reconfigure = 0;
int Sig_do_shutdown = 0;
int got_recount=0;
/* mysql stuff */
MYSQL mysql;
//    MYSQL_RES *result;
//    MYSQL_ROW my_row;
void exit_mysql();
void exit_all();
void logerr(char *msg);
void logmsg(char *msg);
int sys_logfile;
int loffset=0;
/* code itself */
/*
 * This is a replacement for strsep which is not portable (missing on Solaris).
 */
#ifndef HAVE_STRSEP 
static char* getToken(char** str, const char* delims)
{
    char* token;

    if (*str==NULL) {
        /* No more tokens */
        return NULL;
    }

    token=*str;
    while (**str!='\0') {
        if (strchr(delims,**str)!=NULL) {
            **str='\0';
            (*str)++;
            return token;
        }
        (*str)++;
    }
    /* There is no other token */
    *str=NULL;
    return token;
}
#endif
/* Get time ticket */
void gettime(time_t *t)
{
    time_t tmp;
    time(&tmp);
    *t=tmp;
}
const char *LogTime(time_t t, char *fmt)
{
    struct tm *tm;
    static char buf[128];
    static time_t last_t = 0;
    if (t != last_t) {
        tm = localtime(&t);
        strftime(buf, 127, fmt, tm);
        last_t = t;
    }
    return buf;
}
/* Log Error message */
void logerr(char *msg)
{
    if (!config_nosyslog)
        syslog(LOG_CRIT, msg);
    else
        printf("CRITICAL: %s\n",msg);
}
/* Log notice message */
void logmsg(char *msg)
{
    if (!config_nosyslog)
        syslog(LOG_NOTICE, msg);
    else
        printf("NOTICE: %s\n", msg);
}
/* full cleanups */
void exit_all()
{
    /* close syslog connection */
    if (!config_nosyslog) {closelog();};
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
static int db_connect ()
{
        mysql_init (&mysql);
        if (&mysql != mysql_real_connect (&mysql, DB_HOSTNAME, DB_USERNAME, DB_PASSWORD, DB_DATABASE_CISCO, 3306, NULL, 0))
        {
            sprintf(temp,"db_connect: can't connect to mysql! MySQL Error - %d: %s\n",mysql_errno(&mysql), mysql_error(&mysql));
            logerr(temp);
            exit_all();
        }
    /* try to select DB */
    if(mysql_select_db(&mysql,DB_DATABASE_CISCO))
    {
        /* log message about error */
        sprintf(temp,"Unable to select db, MySQL Error - %d: %s\n",mysql_errno(&mysql), mysql_error(&mysql));
        logerr(temp);
        exit_all();
    }
    return (1);
}
/* shut down signal handler */
void shut_down(int)
{
    Sig_do_shutdown = 1;
}
/* hints output */
static void usage ()
{
    printf ("%s v %s [http://sacc.westural.ru]\n",APP_NAME,VERSION);
    printf ("Reasonable:\n");
    printf ("\t-h: This help and exit.\n");
    printf ("\t-d: No daemonize.\n");
    printf ("\t-f: Filename to handle.\n\t    DEFAULT is: %s\n", CISCO_LOGNAME);
    printf ("\t-l: Don't log with syslog, just to stdout.\n");
    printf ("\n");
    exit(1);
};
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
static void parse_string (char *s, size_t len)
{
    char *out,*tmp = NULL, *point = NULL;
    char **field;
    char *fields[10];
    char *query = _V2PC(malloc(QUERY_BUFSIZE * SIZEOF_CHAR));
    int invalid=0;
    int valid = 1;
    int affect=0;
    unsigned long ip_src, ip_dst;
// 192.36.148.17    80.237.79.147                    1                 237
//[1]-from
//[2]-to
//[3]-packets
//[4]-bytes
    if (len > 10)
    {
        out = _V2PC(malloc((len+1) *SIZEOF_CHAR));
        strncpy (out, s, len);
        out[len] = '\0';
        tmp = out;
        for (field = fields; (*field = strsep(&out, " \t")) != NULL;)
        if (**field != '\0') if (++field >= &fields[10]) break;
        point = strchr(fields[1], '.');
// first parametr MUST contain ".", very simple test...
        if (point==NULL) invalid=1;
// advanced test, if enabled (default) - reduce perfomance
#ifndef FAST
// IP address must contain 3 dot's, test about one
        if (strchr(fields[1],'.')==NULL) invalid=1;
#ifdef DEBUG
        printf("string %s field=%d invalid=%d\n",fields[1],(int)field, invalid);
#endif /* debug */
// source [0] must contain "."
        if (strchr(fields[0],'.')==NULL) invalid=1;
#ifdef DEBUG
        printf("string %s valid=%u invalid=%d\n",fields[0],valid, invalid);
#endif /* debug */
//        invalid=(atol(fields[2])==0)?invalid++:invalid;
        if (atol(fields[2])==0) invalid=1;
#ifdef DEBUG
        printf("string %s valid=%u invalid=%d\n",fields[2],valid, invalid);
#endif /* debug */
        if (atol(fields[3])==0) invalid=1;
#endif /* FAST switch */
#ifdef DEBUG
        printf("string %s valid=%d invalid=%d\n",fields[3],valid, invalid);
#endif /* debug */
// check that's invalid, and not end of file
        if (invalid==1)
        {
                valid=0;
// send message about error to syslog
//                logerr(_T("parser: incorrect string\n"));
                if ((strstr(s,"Accounting data age is")==NULL)&&(strstr(s,"Source")==NULL))
                {
                        sprintf(temp, "!parser string: %s\n", s);
                        logerr(temp);
                }
        }
        if (valid)
        {
/*              *** db: CISCO table: traffic
id      |       int ai pk
tm      |       int (unix time)
src     |       int        
dst     |       int
pckts   |       int
bytes   |       int
*/
        ip_src=ntohl(inet_addr(fields[0])); // convert source ip to int
        ip_dst=ntohl(inet_addr(fields[1])); // convert destination ip to int
//        size_pckts=atol(fielsd[2]);         // convert packets counter to int
//        size_bytes=atol(fielsd[3]);         // convert packets counter to int
            sprintf(query,"update traffic set bytes=bytes+%s, pckts=pckts+%s where src=%lu and dst=%lu;",fields[3], fields[2], ip_src, ip_dst);
            mysql_query(&mysql, query);
            affect=mysql_affected_rows(&mysql);
#ifdef DEBUG
            printf("%d updating traffic... %s %s\n",affect,query, mysql_error(&mysql));
#endif  /* DEBUG */
            if (affect == -1) { sprintf(temp, "critical mysql error %s ", mysql_error(&mysql));logerr(temp);exit_all(); }
            if (affect == 0)
            { /* new entry, add to table */
                sprintf(query,"insert into traffic (src, dst, pckts, bytes) values(%lu,%lu,%s,%s);",ip_src, ip_dst, fields[2], fields[3]);
#ifdef DEBUG
                printf("%d inserting into traffic, %s\n", affect, query);
#endif  /* DEBUG */
                mysql_query(&mysql, query);
            };
            if (affect>1)
            {
                printf("parser: more than one items affected in table traffic: %lu %lu %s", ip_src, ip_dst, fields[3]);
//                logmsg(_T("parser: more than one items affected in table traffic\n"));
                logmsg(query);
            };
        };
    };
    out=tmp;
    free(out);
    free(query);
    return;
};
/* main function */
int main (int argc, char *argv[])
{
#ifdef DEBUG  
    time_t start_time, stop_time;
#endif /*debug*/        
    char *s = _V2PC(malloc(MAXBUFSIZE *SIZEOF_CHAR));
    size_t len;
    int a_char;
    struct stat finfo;
    register FILE* fp;
/* arguments parsing... */
    while ((a_char = getopt(argc, argv, "hf:l:")) != -1)
        switch (a_char) {
                case 'f':
                        filename = optarg;
                        config_noseek = 1;
                        exit_imm = 1;
                        break;
                case 'l':
                        config_nosyslog = 1;
                        break;
                case '?':
                case 'h':
                default:
                        usage();
    }
    argc -= optind;
    argv += optind;
    if (filename == NULL)
                filename = CISCO_LOGNAME;  /* default filename */
    if (!config_nosyslog) {openlog(APP_NAME, 0, LOG_USER);}; /* open syslog */
//change my name
#ifdef HAVE_SETPROCTITLE
setproctitle("-%s",APP_TITLE);
#endif
    /* set system signals handlers, bkoz no daemonization! */
//    signal(SIGHUP,&reconfigure);
    signal(SIGTERM,&shut_down);
//    signal(SIGUSR1,&logrotate);
    /* for debug */
#ifdef DEBUG
        printf("%s process started\n",APP_TITLE);
    /* Log message about what we're started */
    sprintf(temp,"%s v%s - started.\n",APP_TITLE,VERSION);
    logmsg(temp);
        printf("init MySQL struct...\n");
#endif  /* DEBUG */
    /* initializing MySQL structure */
    if(!mysql_init(&mysql))
    {
        /* log message about error */
        logerr(_T("Unable to initialize MySQL struct"));
        ErrNumber=1;
        exit_all();
    }
    /* try connect to MySQL server'n'select db */
    db_connect();
#ifdef DEBUG
    printf("connected...\n");
#endif  /* DEBUG */
    /* try to get offset from table */
    offset=0;
        /* checking for termination signal */
    if (Sig_do_shutdown>0) exit_mysql();
        /* getting information about access.log file */
    stat(filename, &finfo);
        /* checking access to access.log file */
    if (!(finfo.st_mode & S_IRUSR))
    {
            logerr(_T("Can't get access for read")); exit_all();
    }
#ifdef DEBUG
    sprintf(temp,"Working with offset: %lu",offset);
    logmsg(temp);
    gettime(&start_time);
#endif  /* DEBUG */
            /* opennig rsh.log file */
    if (NULL == (fp = fopen(filename, "r")))
    {
          logerr(_T("Can't open log for read.")); exit_all();
    }
        /* getting new entrys from file */
    while(!feof(fp))
    {
            /* getting new line from file */
		loffset = ftell(fp);
                len = MAXBUFSIZE-20;
                fgets(s, len, fp);
                if (EINTR==errno) while (EINTR==errno)
                {
			fseek(fp,loffset,SEEK_SET);
	                fgets(s, len, fp);
                }
                len = strlen(s);
            /* process line */
#ifdef DEBUG
                loffset = ftell(fp);
                printf("before parsing offset %u \"%s\"\n",loffset, s);
#endif /* debug */
                if ((*s!=0)&&(len>15)) parse_string (s, len);
                free(s);
                s = _V2PC(malloc(MAXBUFSIZE *SIZEOF_CHAR));
                memset(s, 0, 10);
#ifdef DEBUG
                loffset = ftell(fp);
                printf("after offset %d\n",loffset);
#endif /* debug */
#ifdef DEBUG
                loffset = ftell(fp);
                gettime(&stop_time);
                sprintf(temp,"work offset is - %d, it takes %d seconds. strlen:%d\n",loffset,stop_time-start_time,len);
                logmsg(temp);
#endif  /* DEBUG */
    }
        /* storing positon */
#ifdef DEBUG
    printf("feof offset %lu \n",offset);
#endif /* debug */
        /*for mysql 4.1 compatibility*/
//      mysql_commit(&mysql);
        /* closing access.log file */
    fclose(fp);
    exit_mysql();
}

