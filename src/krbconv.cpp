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
#include <mysql/mysql.h>
using namespace std;

#define _V2PC(data) (char*)(data)
#define _T(txt) (char*)(txt)

#ifndef UTEST

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif				// End HAVE_CONFIG_H

#include "mylog.h"
//#define MAXLEN 10*1024*SIZEOF_CHAR
#define QUERY_BUFSIZE   (MAXLEN-1)
#define MAXBUFSIZE QUERY_BUFSIZE
#define STR_MAX_SIZE  QUERY_BUFSIZE-1

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
#define SIZEOF_CHAR 1
#define MAXLEN 10*1024*SIZEOF_CHAR
#define QUERY_BUFSIZE   (MAXLEN-1)
#define MAXBUFSIZE QUERY_BUFSIZE
#define STR_MAX_SIZE  QUERY_BUFSIZE-1

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

char *mysql_kerbname(char *user_check, bool obratka)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	char login[STR_MAX_SIZE + 1];
	int login_size;

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
		 "select krbname, ntlmname from krbconv where %s like '%s'",
		 (obratka ? "krbname" : "ntlmname"), login);
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
#ifdef DEBUG
	clog << "row[0] " << row[0] << endl;
	clog << "row[1] " << row[1] << endl;
	clog << "sizof: " << (int)(sizeof(user_check) + 1) << endl;
#endif
	snprintf(user_check, MAXLEN,	/*sizeof(user_check) + 1, */
		 (obratka ? row[1] : row[0]));
#ifdef DEBUG
	clog << "Результат mysql_kerbname(): " << user_check << endl;
#endif
	row = NULL;
	mysql_free_result(result);
	return user_check;
}

// -----------------------------------------------------------------------------------------
char *ConvertKrbName(char *k_name, bool preob = true)
{
	char *pch = strchr(k_name, '@');
	int posd = (int)(pch - k_name);	// posd - будет указывать на позицию _перед_ разделителем '@'
	int klen = strlen(k_name);
#ifdef DEBUG
	clog << "Size of " << k_name << " is: " << klen << endl;
#endif
	// возвращаемая строка.
	char *ret = new char[MAXLEN];

	// Если не найден в имени символ '@' - значит оставим без изменений - вероятно(!) это NTLM
	if (NULL == pch) {
#ifdef DEBUG
		clog << "Found NTLM name: " << k_name << endl;
#endif
		if (preob) {
			strncpy(ret, k_name, klen);
			ret[klen] = '\0';
#ifdef DEBUG
			clog << "1: " << ret << endl;
#endif
			return ret;
		} else {	// Требуется обратное преобразование, т.е. из ntlm -> kerberos name
			char *pch = strchr(k_name, '\\');
			if (NULL != pch) {
				//char domain[MAXLEN];
				char *dom = new char[MAXLEN];
				int poso = (int)(pch - k_name);
#ifdef DEBUG
				clog << "Poso: " << poso << endl;	//temp
#endif
				strncpy(dom, k_name, (int)(poso));
				dom[poso] = '\0';
#ifdef DEBUG
				clog << "Domain: " << dom << " pch: " << pch + 1 << endl;	//temp
#endif
				char *tmp = mysql_kerbname(dom, preob);
				snprintf(ret, STR_MAX_SIZE, "%s%s", pch + 1,
					 tmp);
#ifdef DEBUG
				clog << "NTLM обратка: " << ret << endl;
#endif
				delete[]dom;
				return ret;
			} else {
				//free(dom);
				snprintf(ret, STR_MAX_SIZE, "#");
				return ret;	//snprintf(k_name, 1, "");
			}
		}
	} else {
#ifdef DEBUG
		clog << "Found Kerberos name: " << k_name <<
		    " in pos: " << posd + 1 << endl;
#endif
		char login[MAXLEN];	// = NULL;
		// Выделим из полного имени только само имя, без домена
		strncpy(login, k_name, posd);
		login[posd] = '\0';
		char *tmp = mysql_kerbname(pch, preob);
		if (NULL != tmp) {
			snprintf(ret, STR_MAX_SIZE, "%s\\%s", tmp, login);
#ifdef DEBUG
			clog << "Converted to NTLM name: " << ret << endl;
#endif
			return ret;
		}
	}
	//free(dom);
	ret[0] = '#';
	ret[1] = '\0';
	return ret;
}				// End ConvertName(...)

#ifdef UTEST
/*
    ---=== MAIN ===---
*/
int main(void)
{
	//char *tstconv = _V2PC(malloc(MAXLEN));
	char qq[512], *ss = NULL;
	sql_connect(&mysql);
	char *tempconv;		// Local !
	// simple test
	snprintf(temp, STR_MAX_SIZE, "testuser@KRBD.DOMEN.RU");
	tempconv = ConvertKrbName(temp);
	clog << "Original: " << temp << " and converted name: " << tempconv << endl;	//ConvertKrbName(tstconv) << endl;
	delete[]tempconv;
	// Test 2
	tempconv = ConvertKrbName("auser@NONE.RU");
	clog << "Original: " << "auser@NONE.RU" <<
	    " and converted name: " << tempconv << endl;
	delete[]tempconv;
	// Test 3
	tempconv = ConvertKrbName("tst@KRBD.DOMEN.RU");
	clog << "Original: " << "tst@KRBD.DOMEN.RU" << " and converted name: "
	    << tempconv << endl;
	delete[]tempconv;
	// Test 4
	clog << "---------------------------------------" << endl;
	ss = strncpy(qq, "otherdom", 18);
	ss[8] = '\\';
	strcat(ss, "nextuser");
	snprintf(temp, STR_MAX_SIZE, _T(ss));
	//snprintf(tstconv, STR_MAX_SIZE, temp);
	tempconv = ConvertKrbName(_T(ss), false);
	clog << "Original: " << temp <<
	    " and converted name: " << tempconv << endl;
	delete[]tempconv;
	// Test 5
	snprintf(temp, STR_MAX_SIZE, "ales");
	tempconv = ConvertKrbName("ales", false);
	//snprintf(tstconv, STR_MAX_SIZE, temp);
	clog << "Original: " << temp <<
	    " and converted name: " << tempconv << endl;
	delete[]tempconv;
	// Test 6
	snprintf(temp, STR_MAX_SIZE, "bigusername");
	//snprintf(tstconv, STR_MAX_SIZE, temp);
	tempconv = ConvertKrbName("bigusername", false);
	clog << "Original: " << "bigusername" <<
	    " and converted name: " << tempconv << endl;
	delete[]tempconv;
	// ----------------
	mysql_close(&mysql);
	//free(tstconv);
	return EXIT_SUCCESS;
}

#endif
