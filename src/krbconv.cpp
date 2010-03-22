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
#include <cstdio>
#include <cstdlib>
#include <mysql/mysql.h>

#include "namepair.h"
NamePairList npl;

using namespace std;

//#define _V2PC(data) (char*)(data)
//#define _T(txt) (char*)(txt)

#ifndef UTEST

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif				// End HAVE_CONFIG_H

#define MAXLEN 10*1024*SIZEOF_CHAR
#define QUERY_BUFSIZE   (MAXLEN-1)
#define MAXBUFSIZE QUERY_BUFSIZE
#define STR_MAX_SIZE  QUERY_BUFSIZE-1

extern MYSQL mysql;
extern char *temp;
extern char *sql_query;
extern void exit_all(void);
extern void sql_connect(MYSQL * lpmysql_link);

//extern void logcrt(const char *msg);
#include "mylog.h"
extern mylog logger;
#define logcrt(mesg) logger.msg(__FILE__, __LINE__, 4, mesg)

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

//char *temp = _V2PC(malloc(MAXLEN));
char *temp = new char[MAXLEN];
//char *sql_query = _V2PC(malloc(STR_MAX_SIZE));
char *sql_query = new char[STR_MAX_SIZE];

void exit_all(void)
{
	mysql_close(&mysql);
	delete[]temp;
	delete[]sql_query;
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

std::string mysql_kerbname(const string & user_check, bool obratka)
{
	//char *ret = new char[MAXLEN];
	//std::string * ret = new std::string[MAXLEN];
	string ret("");		// объявление и инициализация.
	size_t inlen = user_check.length();
	char *login = new char[inlen * 2 + 1];
	if (!login) {
		snprintf(temp, STR_MAX_SIZE, "No memory to allocate login");
		logcrt(temp);
		exit_all();
	}
	MYSQL_RES *result;
	MYSQL_ROW row;
	//char login[STR_MAX_SIZE + 1]; // High(!): fixed size local buffer
	size_t login_size;

#ifdef DEBUG
	clog << "Проверка домена " << user_check <<
	    " по базе krbconv" << endl;
#endif
	// Check user exist in DB
	login_size =
	    mysql_real_escape_string(&mysql, login, user_check.c_str(), inlen);
#ifdef DEBUG
	printf("In field %s: login(%d) %s\n", user_check.c_str(), login_size,
	       login);
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
		logcrt(temp);
		delete[]login;
		return ret;	//NULL;
	}

	delete[]login;		// более он нам более не нужен.
	result = mysql_store_result(&mysql);
	if (result == NULL) {
#ifdef DEBUG
		cerr << "Нет соединения с базой! " << endl;
#endif
		//delete[]ret;
		//return NULL;
		return ret;
	}

	if (mysql_num_rows(result) < 1) {
#ifdef DEBUG
		cerr << "Нет данных в таблице" << endl;
#endif
		mysql_free_result(result);
		//delete[]ret;
		return ret;
	}

	row = mysql_fetch_row(result);
	if (row == NULL) {
#ifdef DEBUG
		cerr <<
		    "Данные найдены, но не могу прочитать."
		    << endl;
#endif
		mysql_free_result(result);
		//return NULL;
		return ret;
	}
#ifdef DEBUG
	clog << "row[0] " << row[0] << endl;
	clog << "row[1] " << row[1] << endl;
	clog << "Len of input: " << inlen << endl;
#endif
	/*
	   snprintf(ret, MAXLEN,        //sizeof(user_check) + 1,
	   (obratka ? row[1] : row[0]));
	 */
//      string *ret = new string[MAXLEN];
//      (*ret) = obratka ? row[1] : row[0];
	ret.assign(obratka ? row[1] : row[0]);
#ifdef DEBUG
	clog << "Результат mysql_kerbname(): " << user_check <<
	    " ==--> " << ret << endl;
#endif
	mysql_free_result(result);
	//return (char *)ret;
	return ret;
}

// -----------------------------------------------------------------------------------------
string ConvertKrbName(const string k_name, bool preob = true)
{
	const static char delim = '\\';
#ifdef DEBUG
	clog << endl;
	clog << "=--->ConvertKrbName(" << k_name << "," << (preob ? "True" :
							    "False")
	    << ")<---=" << endl;
#endif
	// возвращаемая строка. В вызывающей ф. удаляем как: delete [] temp;
	//char *ret = new char[MAXLEN + 1];     // High(!): fixed size local buffer
	string myret;		// Возвращаемая строка.
	string ltmp = "";
	if (npl.GetValue(k_name.c_str(), &ltmp)) {
#ifdef DEBUG
		clog << "-------Найдено в кеше: " << ltmp <<
		    " -------------" << endl;
#endif
		//snprintf(ret, MAXLEN, "%s", ltmp.c_str());
		return myret.assign(ltmp);	//ret;
	}

	size_t klen = k_name.length();
	string login, domain;
	// Выполняем поиск.
	size_t found = k_name.find('@');

#ifdef DEBUG
	clog << "Size of " << k_name << " is: " << klen << endl;
	//clog << "Size of " << ptr << " is: " << strlen(ptr) << endl;
#endif

	// Если _не_найден_ в имени символ '@' - значит оставим без изменений - вероятно(!) это NTLM
	if (found == string::npos) {	// npos - NO_POSITION!
#ifdef DEBUG
		clog << "Found NTLM name: " << k_name << endl;
#endif
		if (preob) {

			//strncpy(ret, k_name.c_str(), klen);
			//ret[klen] = '\0';
#ifdef DEBUG
			clog << "Оставим как есть: " << k_name <<
			    endl;
#endif
			return myret.assign(k_name);	//ret;
		} else {	// Требуется обратное преобразование, т.е. из ntlm -> kerberos name
			// В k_name будет типа 'ntlmdom\username'
			found = k_name.find('\\');
			if (found != string::npos) {
				domain = k_name.substr(0, found);
				login =
				    k_name.substr(found + 1,
						  (klen - found - 1));
#ifdef DEBUG
				clog << "Found Domain: " << domain << " in position: " << int (found) << " and login: " << login << endl;	//temp
#endif
				const string & tmp =
				    mysql_kerbname(domain, preob);
				if (tmp.size() != 0) {
					//snprintf(ret, STR_MAX_SIZE, "%s%s", login.c_str(), tmp.c_str());
					login += tmp;
					//snprintf(ret, MAXLEN, "%s", login.c_str());
					myret.assign(login);

#ifdef DEBUG
					clog << "NTLM обратка: " << myret
					    << endl;
#endif
					// Добавляем найденное в наш кеш NamePairList.
					// (BUG?) npl.AddPair(k_name, myret.c_str());
					return myret;
				}
				//delete[]tmp;
			} else {
				//snprintf(ret, STR_MAX_SIZE, "#");
				myret.assign("");
				return myret;	//ret;    //snprintf(k_name, 1, "");
			}

		}
	} else {

#ifdef DEBUG
		clog << "Found Kerberos name: " << k_name << " in position: " << found << endl;	//<< (unsigned int)(posd + 1) << endl;
#endif
		// В k_name будет типа 'username@KERBDOM.MY.RU'
		// Выделим из полного имени только само имя, без домена
		login = k_name.substr(0, found);
		domain = k_name.substr(found, k_name.length() - found);
#ifdef DEBUG
		clog << "Kerberos Domain: " <<
		    domain << " and login: " << login << endl;
#endif
		const string & tmp = mysql_kerbname(domain, preob);
		if (tmp.size() != 0) {
			//snprintf(ret, STR_MAX_SIZE, "%s\\%s", tmp.c_str(), login.c_str());
			myret.assign(tmp);
			myret += delim;
			myret += login;
#ifdef DEBUG
			clog << "Converted to NTLM name: " << myret << endl;
#endif
			// Сохраняем пару в кеше, хотя возможно это и излишне т.к. эти данные
			// нужны тольео при формировании списков ACL (типа squid.conf.fulltime)
			npl.AddPair(k_name, myret.c_str());
			return myret;	//ret;
		}
	}
	//ret[0] = '#';
	//ret[1] = '\0';
	myret.assign("");
	return myret;		// ret;
}				// End ConvertName(...)

#ifdef UTEST
/*
    ---=== MAIN ===---
*/
int main(void)
{
	//char qq[512], *ss = NULL;
	sql_connect(&mysql);
	//char *tempconv;               // Local !
	string tempconv;
	// Test 1
	string str = "testuser@KRBD.DOMEN.RU";
	tempconv = ConvertKrbName(str);
	clog << "Original: " << str <<
	    " and converted name: " << tempconv << endl;
	//delete[]tempconv;
	// Test 1-1
	tempconv = ConvertKrbName(str, false);
	clog << "Original: " << str <<
	    " and converted name: " << tempconv << endl;
	//delete[]tempconv;
	// Test 2
	str.assign("tst@OTHERDOM.TEST.MY");
	tempconv = ConvertKrbName(str);
	clog << "Original: " << str <<
	    " and converted name: " << tempconv << endl;
	//delete[]tempconv;
	// Test 2-1
	tempconv = ConvertKrbName(str, false);
	clog << "Original: " << str <<
	    " and converted name: " << tempconv << endl;
	//delete[]tempconv;
	// Test 2-2
	str.erase();
	str.assign("tst@OTHERDOM.TEST.MY");
	tempconv = ConvertKrbName(str);
	clog << "Original: " << str <<
	    " and converted name: " << tempconv << endl;
	//delete[]tempconv;
	// Test 3
	clog << "---------------------------------------" << endl;
	str.assign("otherdom");
	str += '\\';
	str += "nextuser";
	tempconv = ConvertKrbName(str);
	clog << "Original: " << str <<
	    " and converted name: " << tempconv << endl;
	//delete[]tempconv;
	// Test 3-1
	tempconv = ConvertKrbName(str, false);
	clog << "Original: " << str <<
	    " and converted name: " << tempconv << endl;
	//delete[]tempconv;
	// Test 3-2
	tempconv = ConvertKrbName(str, false);
	clog << "Original: " << str <<
	    " and converted name: " << tempconv << endl;
	//delete[]tempconv;
	// Test 5
	str.assign("test_true");
	tempconv = ConvertKrbName(str);
	clog << "Original: " << str <<
	    " and converted name: " << tempconv << endl;
	//delete[]tempconv;
	// Test 6
	str.assign("test_false");
	tempconv = ConvertKrbName(str, false);
	clog << "Original: " << str <<
	    " and converted name: " << tempconv << endl;
	//delete[]tempconv;
	// ----------------
	mysql_close(&mysql);
	//free(tstconv);
	delete[]temp;
	delete[]sql_query;
	return EXIT_SUCCESS;
}
#endif
