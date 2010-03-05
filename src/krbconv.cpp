/*
  $Author: Alexey Mekhanoshin (Мутатор) <ales999@gmail.com> for SACC project.
  Need expand database sacc from addons/krbconv/krbconv.sql
            -----------------------------------------------------
#                       This file is part of SAcc system.
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
*/
#include <iostream>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <mysql/my_config.h>
#include <mysql/mysql.h>
using namespace std;

#define MAXLEN 10*1024*SIZEOF_CHAR
#define QUERY_BUFSIZE   (MAXLEN-1)
#define MAXBUFSIZE QUERY_BUFSIZE
#define STR_MAX_SIZE  QUERY_BUFSIZE-1
#define _V2PC(data) (char*)(data)
#define _T(txt) (char*)(txt)

#ifndef UTEST

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif				// End HAVE_CONFIG_H

extern MYSQL mysql;
extern char *temp;
extern char *sql_query;
extern void exit_all(void);
extern void sql_connect(MYSQL * lpmysql_link);

#else				// Else UTEST

// For internal test:
//#define DEBUG
#define DB_HOSTNAME "localhost"
#define DB_DATABASE "sacc"
#define DB_USERNAME "sacc"
#define DB_PASSWORD "sacc"

MYSQL mysql;

char *temp = _V2PC(malloc(MAXLEN));
char *sql_query = _V2PC(malloc(STR_MAX_SIZE));
void exit_all(void)
{
	mysql_close(&mysql);
	exit(EXIT_FAILURE);
}

void logcrt(string str)
{
	cerr << str << endl;
}

/* connect to MySQL db */
void sql_connect(MYSQL * lpmysql_link)
{
	mysql_init(lpmysql_link);
	if (!(mysql_real_connect(lpmysql_link, DB_HOSTNAME, DB_USERNAME,
				 DB_PASSWORD, DB_DATABASE, 3306, NULL, 0))) {
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

#endif				// End UTEST

char *mysql_kerbname(char *user_check)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	static char login[MAXLEN];
	static int login_size;

#ifdef DEBUG
	clog << "Проверка домена " << user_check <<
	    " по базе krbconv" << endl;
#endif
	// Check user exist in DB
	login_size =
	    mysql_real_escape_string(&mysql, login, user_check,
				     strlen(user_check));
#ifdef DEBUG
	printf("In field %s: login(%d) %s\n", user_check, login_size, login);
#endif
	snprintf(sql_query, STR_MAX_SIZE,
		 "select krbname, ntlmname from krbconv where krbname like '%s'",
		 login);
#ifdef DEBUG
	printf("Sql: %s\n", sql_query);
#endif
	if (mysql_query(&mysql, sql_query)) {
		snprintf(temp, STR_MAX_SIZE,
			 "MySQL Error - %d: %s\n",
			 mysql_errno(&mysql), mysql_error(&mysql));
#ifdef DEBUG
		cerr << temp << endl;
#endif
		return NULL;
	}
	result = mysql_store_result(&mysql);
	if (result == NULL) {
#ifdef DEBUG
		cerr << "Нет соединения с базой! " << endl;
#endif
		return NULL;
	}

	if (mysql_num_rows(result) < 1) {
#ifdef DEBUG
		cerr << "Нет данных в таблице" << endl;
#endif
		mysql_free_result(result);
		return NULL;
	}

	row = mysql_fetch_row(result);
	if (row == NULL) {
#ifdef DEBUG
		cerr <<
		    "Данные найдены, но не могу прочитать."
		    << endl;
#endif
		mysql_free_result(result);
		return NULL;
	}
	snprintf(user_check, sizeof(user_check) + 1, row[1]);
#ifdef DEBUG
	clog << "Результат mysql_kerbname(): " << user_check << endl;
#endif
	row = NULL;
	mysql_free_result(result);
	return user_check;
}

char *ConvertKrbName(char *k_name)
{
	char *pch = strchr(k_name, '@');
	int posd = (int)(pch - k_name);	// posd - будет указывать на позицию _перед_ разделителем '@'
	// Если не найден в имени символ '@' - значит оставим без изменений - вероятно(!) это NTLM
	if (NULL == pch) {
#ifdef DEBUG
		clog << "Found NTLM name: " << k_name << endl;
#endif
		return k_name;
	} else {
#ifdef DEBUG
		clog << "Found Kerberos name: " << k_name << " in pos: " << posd
		    + 1 << endl;
#endif
		char login[MAXLEN];	// = NULL;
		// Выделим из полного имени только само имя, без домена 
		strncpy(login, k_name, posd);
		login[posd] = '\0';
		char *tmp = mysql_kerbname(pch);
		if (NULL != tmp) {
			snprintf(k_name, STR_MAX_SIZE, "%s\\%s", tmp, login);
#ifdef DEBUG
			clog << "Converted to NTLM name: " << k_name << endl;
#endif
		}
	}
	return k_name;
}				// End ConvertName(...)

#ifdef UTEST
/*
    ---=== MAIN ===---
*/
int main(void)
{
	char *tstconv = _V2PC(malloc(MAXLEN));
	char qq[512], *ss = NULL;
	sql_connect(&mysql);
	// simple test
	snprintf(temp, STR_MAX_SIZE, "testuser@KRBD.DOMEN.RU");
	snprintf(tstconv, STR_MAX_SIZE, temp);
	clog << "Original: " << temp << " and converted name: " <<
	    ConvertKrbName(tstconv) << endl;

	// Test 2
	snprintf(temp, STR_MAX_SIZE, "auser@NONE.RU");
	snprintf(tstconv, STR_MAX_SIZE, temp);
	clog << "Original: " << temp << " and converted name: " <<
	    ConvertKrbName(tstconv) << endl;

	// Test 3
	snprintf(temp, STR_MAX_SIZE, "tst@KRBD.DOMEN.RU");
	snprintf(tstconv, STR_MAX_SIZE, temp);
	clog << "Original: " << temp << " and converted name: " <<
	    ConvertKrbName(tstconv) << endl;

	// Test 4
	ss = strncpy(qq, "otherdom", 9);
	ss[4] = '\\';
	strcat(ss, "nextuser");
	snprintf(temp, STR_MAX_SIZE, _T(ss));
	snprintf(tstconv, STR_MAX_SIZE, temp);
	clog << "Original: " << temp << " and converted name: " <<
	    ConvertKrbName(tstconv) << endl;

	mysql_close(&mysql);
	return EXIT_SUCCESS;
}

#endif
