/********************************************************************************
#           SAccounting                                            [SAcc system] 
#			   Copyright (C) 2003-2005  the.Virus
#		$Author: slavik $ $Date: 2008-01-16 14:25:35 +0500 (п║я─п╢, 16 п╞п╫п╡ 2008) $
#		$Id: mylog.h 17 2008-01-16 09:25:35Z slavik $
#           -----------------------------------------------------
#   			This file is part of SAcc system.  
#           -----------------------------------------------------
#        ----------    сделано с особым цинизмом.   ----------------
#                    [http://sacc.cybersec.ru/
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
#ifndef HAVE_MYLOG_H
#define HAVE_MYLOG_H 1
#define SIZEOF_CHAR 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <syslog.h>
#define MAXLEN 10*1024*SIZEOF_CHAR
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#define logerr(mesg) logger.msg(__FILE__, __LINE__, 3, mesg)
#define logcrt(mesg) logger.msg(__FILE__, __LINE__, 4, mesg)
#define logmsg(mesg) logger.msg(__FILE__, __LINE__, 5, mesg)
#define loginf(mesg) logger.msg(__FILE__, __LINE__, 6, mesg)
#define logdbg(mesg) logger.msg(__FILE__, __LINE__, 7, mesg)

class mylog 
{
	bool connected;
	int logmask;
	char* log_buffer;
	bool syslg;
public:
	void msg(char* FileName, int Line, int severity, const char *fmt, ...);
	virtual ~mylog(void);
	void log(char* app_name, bool insyslog, int mask);
};
#endif
