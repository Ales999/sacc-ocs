/********************************************************************************
#           LogSystem                                            [SAcc system] 
#			   Copyright (C) 2003-2006  Vyacheslav Nikitin
#		$Author: slavik $ $Date: 2008-01-16 14:25:35 +0500 (Срд, 16 Янв 2008) $
#		$Id: mylog.cpp 17 2008-01-16 09:25:35Z slavik $
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
#include "mylog.h"
#include <pthread.h>

mylog::~mylog()
	{
	if (connected) 
		{
		closelog();
		connected=false;
		free(log_buffer);
		}
	}
void mylog::log(char* app_name, bool insyslog, int mask)
	{
	openlog(app_name, 0, LOG_USER);
	connected=true;
	logmask=mask;
	log_buffer=(char*)malloc(MAXLEN);
	syslg=insyslog;
	}
/* Log message */
void mylog::msg(char* FileName, int Line, int severity, const char *fmt, ...)
{
	va_list ap;		
	if (severity>logmask) return;
	if (NULL==fmt) return;
	va_start(ap, fmt);	
    if (syslg)
    {
    //char* FileName, int Line
    	if (7==logmask) {
    		syslog(severity, "in %s:%d", FileName, Line);
    		};
        vsyslog(severity, fmt, ap);
    }
    else
    {
      	if (7==logmask) {
      		printf("(%u) in %s:%d ",pthread_self(), FileName, Line);
      		};
		vprintf(fmt, ap);
		printf("\n");
    };
	va_end(ap);
};
// Альтернативная версия msg
void mylog::msg(std::string FileName, int Line, int severity, const std::string fmt, ...)
{
    va_list ap;
    if (severity>logmask) return;
    if (fmt.empty()) return;
    //if (NULL==fmt) return;
    va_start(ap, fmt);
    vsyslog(severity, "CPP Version", ap);
    if(syslg)
    {
	if(7==logmask) 
	    syslog(severity, "in %s:%d", FileName.c_str(), Line);
	vsyslog(severity, fmt.c_str(), ap);
    } else
    {
	if(7==logmask)
	    printf("(%u) in %s:%d ",pthread_self(), FileName.c_str(), Line);
	vprintf(fmt.c_str(), ap);
	printf("\n");
    }
    va_end(ap);
};
