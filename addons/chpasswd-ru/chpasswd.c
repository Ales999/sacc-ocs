/*
 * chpasswd.cgi from htpasswd
 * Mar/98 - Pedro Lineu Orso  
 * Curitiba Parana Brazil
 * porso@usa.net
 *
 */
#include "conf.h"

#define MAXLEN 1024
#define hhex(x) (((x) >= '0' && (x) <= '9') || ((x) >= 'a' && (x) <= 'f') || \
                  ((x) >= 'A' && (x) <= 'F'))
char           *tn;

static void fixpwd(str);
static int  htoi(s);
void getconf(ConfigFile,IpAuth,PwdFile,PwdMinLen,PwdMaxLen,PwdLogFile,BgColor,TxColor,TiColor,RuColor,Logo,Width,Height,Header,BgImage,SmtpUser,SmtpSubject);
void log(PwdLogFile,User,New_pw1,ShowPwd,Msg,PwdFile,SmtpUser,SmtpSubject);
static void vrfyrule(unsigned char *, unsigned char *, unsigned char *, unsigned char *);
static void vrfyauth(IpAuth,PwdLogFile,PwdFile,SmtpUser,SmtpSubject);
eperror(s);

main(argc, argv)
    int             argc;
    char           *argv[];
{

    FILE           *fpw,
                   *tmp;
    char            buf[MAXLEN];
    char            User[255];
    char            PUser[255];
    char            WUser[255];
    char            Old_pw[255];
    char            WOld_pw[255];
    char            New_pw1[255];
    char            New_pw2[255];
    char            Action[255];
    char            Msg[MAXLEN];
    char            command[255];
    char	    PwdFile[MAXLEN]="/usr/local/squid/etc/passwd";
    char	    IpAuth[MAXLEN]="";
    char	    PwdMinLen[255]="4";
    char	    PwdMaxLen[255]="10";
    char	    ShowPwd[255]="no";
    char	    PwdMinChar[255]="0";
    char	    PwdMinNum[255]="0";
    char	    PwdMinSpec[255]="0";
    char	    BgColor[255]="white";
    char	    TiColor[255]="green";
    char	    TxColor[255]="blue";
    char	    RuColor[255]="red";
    char	    Logo[MAXLEN]="";
    char	    Width[MAXLEN]="80";
    char	    Height[MAXLEN]="80";
    char	    Header[MAXLEN];
    char	    BgImage[MAXLEN]="";
    char	    SmtpUser[MAXLEN]="";
    char	    SmtpSubject[MAXLEN]="CHPASSWD EVENT";
    char            InputBuffer[1024];
    char	    ConfigFile[255];
    char	    PwdLogFile[255];
    char           *pContentLength;
    char           *str;
    char           *cpw,
                    salt[3];
    int             ContentLength;
    int             i;
    int             x,
                    ok = 0;

    tn = NULL;
    fpw = NULL;
    tmp = NULL;

    buf[0]='\0';
    User[0]='\0';
    PUser[0]='\0';
    WUser[0]='\0';
    Old_pw[0]='\0';
    WOld_pw[0]='\0';
    New_pw1[0]='\0';
    New_pw2[0]='\0';
    Action[0]='\0';
    Msg[0]='\0';
    command[0]='\0';
    PwdLogFile[0]='\0';
    strcpy(Header,msg22);

    sprintf(ConfigFile,"%s/chpasswd.conf",PREFIX);
    if (access(ConfigFile, R_OK) == 0)
       getconf(ConfigFile,IpAuth,PwdFile,PwdMinLen,PwdMaxLen,PwdMinChar,PwdMinNum,PwdMinSpec,PwdLogFile,ShowPwd,BgColor,TxColor,TiColor,RuColor,Logo,Width,Height,Header,BgImage,SmtpUser,SmtpSubject);

    if (strlen(IpAuth) > 0 ) {
       if (access(IpAuth, R_OK) != 0) {
          Hmsg(msg27);
          if(strlen(PwdLogFile) > 0)
             log(PwdLogFile,"-","","no",msg27,PwdFile,SmtpUser,SmtpSubject);
          exit(1);
       }
       vrfyauth(IpAuth,PwdLogFile,PwdFile,SmtpUser,SmtpSubject);
    }

    if (strcmp(getenv("REQUEST_METHOD"), "GET") == 0) {
	UserForm(PwdMinLen,PwdMaxLen,PwdMinChar,PwdMinNum,PwdMinSpec,BgColor,TxColor,TiColor,RuColor,Logo,Width,Height,Header,BgImage);
	return;
    }

    pContentLength = getenv("CONTENT_LENGTH");

    if (pContentLength != NULL)
	ContentLength = atoi(pContentLength);
    else
	ContentLength = 0;


    if (ContentLength > sizeof(InputBuffer) - 1) {
	ContentLength = sizeof(InputBuffer) - 1;
    }
    i = 0;

    while (i < ContentLength) {
	x = fgetc(stdin);
	if (x == EOF)
	    break;
	InputBuffer[i++] = x;
    }

    InputBuffer[i] = '\0';
    ContentLength = i;
    getword(User, InputBuffer, '=');
    getword(User, InputBuffer, '&');
    getword(Old_pw, InputBuffer, '=');
    getword(Old_pw, InputBuffer, '&');
    getword(New_pw1, InputBuffer, '=');
    getword(New_pw1, InputBuffer, '&');
    getword(New_pw2, InputBuffer, '=');
    getword(New_pw2, InputBuffer, '&');
    getword(Action, InputBuffer, '=');

    if(atoi(PwdMinChar) || atoi(PwdMinNum) || atoi(PwdMinSpec))
       vrfyrule(New_pw1,PwdMinChar,PwdMinNum,PwdMinSpec);

    fixpwd(Old_pw);

    if (strcmp(Action, "change") == 0) {
        if (strlen(New_pw1) < atoi(PwdMinLen)) {
            sprintf(Msg, "%s %s %s",msg02, PwdMinLen, msg03);
 	    if(strlen(PwdLogFile) > 0)
  	       log(PwdLogFile,User,New_pw1,ShowPwd,Msg,PwdFile,SmtpUser,SmtpSubject);
	    Herror(Msg, fpw, tmp);
	    return;
        }

	if (strlen(New_pw1) > atoi(PwdMaxLen)) {
	    sprintf(Msg, "%s %s %s",msg04, PwdMaxLen, msg03);
            if(strlen(PwdLogFile) > 0)
               log(PwdLogFile,User,New_pw1,ShowPwd,Msg,PwdFile,SmtpUser,SmtpSubject);
	    Herror(Msg, fpw, tmp);
	    return;
	}

	if (strcmp(Old_pw, New_pw1) == 0) {
	    sprintf(Msg, "%s",msg05);
            if(strlen(PwdLogFile) > 0)
               log(PwdLogFile,User,New_pw1,ShowPwd,Msg,PwdFile,SmtpUser,SmtpSubject);
	    Herror(Msg, fpw, tmp);
	    return;
	}
	if (strcmp(New_pw1, New_pw2) != 0) {
	    sprintf(Msg, "%s",msg06);
            if(strlen(PwdLogFile) > 0)
               log(PwdLogFile,User,New_pw1,ShowPwd,Msg,PwdFile,SmtpUser,SmtpSubject);
	    Herror(Msg, fpw, tmp);
	    return;
	}
	if ((fpw = fopen(PwdFile, "r")) == NULL) {
	    sprintf(Msg, "%s %s",msg07, PwdFile);
	    Herror(Msg, fpw, tmp);
	    return;
	}
	tn = tmpnam(NULL);
	if ((tmp = fopen(tn, "w")) == NULL) {
	    sprintf(Msg, "%s",msg08);
	    Herror(Msg, fpw, tmp);
	    return;
	}
	while (fgets(buf, MAXLEN, fpw) != NULL) {

	    if (!ok) {
                strcpy(PUser,User);
                strcat(PUser,":");

                if ((str = (char *) strstr(buf, PUser)) != (char *) NULL ) {
		    getword(WUser, buf, ':');
		    getword(WOld_pw, buf, '\n');

		    if (strcmp(WOld_pw, crypt(Old_pw, WOld_pw)) != 0) {
			sprintf(Msg, "%s %s",msg09, User);
            		if(strlen(PwdLogFile) > 0)
               		   log(PwdLogFile,User,New_pw1,ShowPwd,Msg,PwdFile,SmtpUser,SmtpSubject);
			Herror(Msg, fpw, tmp);
			return;
		    }
                    fixpwd(New_pw1);
		    (void) srand((int) time((time_t *) NULL));
		    to64(&salt[0], rand(), 2);
		    cpw = crypt(New_pw1, salt);	
		    sprintf(buf, "%s:%s\n", User, cpw);
		    ok++;

		}
	    }
	    putline(tmp, buf);
	}

	fclose(fpw);
	fclose(tmp);

	sprintf(command, "cp %s %s", tn, PwdFile);
	system(command);
	unlink(tn);

	if (ok)
	    Changed(User, PwdLogFile, New_pw1, ShowPwd, PwdFile, SmtpUser, SmtpSubject);
	else
	    NotFound(User, PwdLogFile, New_pw1, ShowPwd, PwdFile, SmtpUser, SmtpSubject);
    } else {
        if (strcmp(Action, "cancel") == 0) {
   	   sprintf(Msg, "%s",msg10);
           if(strlen(PwdLogFile) > 0)
              log(PwdLogFile,User,New_pw1,ShowPwd,Msg,PwdFile,SmtpUser,SmtpSubject);
	   Herror(Msg, fpw, tmp);
	   return;
	}
    }
    return;
}


void
getword(char *word, char *line, char stop)
{
    int             x = 0,
                    y;

    for (x = 0; ((line[x]) && (line[x] != stop)); x++)
	word[x] = line[x];

    word[x] = '\0';
    if (line[x])
	++x;
    y = 0;

    while ((line[y++] = line[x++]));
}


void
Herror(char *msg, FILE * fpw, FILE * tmp)
{
    printf("Content-type: text/html\n");
    puts("\n");
    printf("<html>\n");
    printf("<head>\n");
    printf("<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=koi8-r\">");
    printf("  <title>Изменение пароля доступа к Squid</title>\n");
    printf("</head>\n");
	    
    printf("<font color=red size=+2>%s\n", msg);
    if (fpw)
	fclose(fpw);
    if (tmp) {
	fclose(tmp);
	unlink(tmp);
    }
    return;
}


void
Hmsg(char *msg)
{
    printf("Content-type: text/html\n");
    puts("\n");
    printf("<html>\n");
  printf("<head>\n");
    printf("<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=koi8-r\">");
      printf("  <title>Изменение пароля доступа к Squid</title>\n");
        printf("</head>\n");
    printf("<font color=red size=+2>%s\n", msg);
    return;
}


void
putline(FILE * f, char *l)
{
    int             x;

    for (x = 0; l[x]; x++)
	fputc(l[x], f);
// fputc('\n',f);
}


static unsigned char itoa64[] =	/*
				 * 0 ... 63 => ascii - 64 
				 */
"./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

to64(s, v, n)
    register char  *s;
    register long   v;
    register int    n;
{
    while (--n >= 0) {
	*s++ = itoa64[v & 0x3f];
	v >>= 6;
    }
}


void
Changed(char *user, char *PwdLogFile, char *newpwd, char *showpwd, char *PwdFile, char *SmtpUser, char *SmtpSubject)
{
    char Msg[MAXLEN];

    printf("Content-type: text/html\n");
    puts("\n");
    printf("<html>\n");
  printf("<head>\n");
    printf("<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=koi8-r\">");
      printf("  <title>Изменение пароля доступа к Squid</title>\n");
        printf("</head>\n");
	    
    printf("<font color=blue size=+2>");
    sprintf(Msg, "%s %s %s",msg11, user, msg12);
    if(strlen(PwdLogFile) > 0)
       log(PwdLogFile,user,newpwd,showpwd,Msg,PwdFile,SmtpUser,SmtpSubject);
    strcat(Msg,"</font>");
    printf(Msg);

    return;
}


void
NotFound(char *user, char *PwdLogFile, char *newpwd, char *showpwd, char *PwdFile, char *SmtpUser, char *SmtpSubject)
{
    char Msg[MAXLEN];

    printf("Content-type: text/html\n");
    puts("\n");
    printf("<html>\n");
  printf("<head>\n");
    printf("<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=koi8-r\">");
      printf("  <title>Изменение пароля доступа к Squid</title>\n");
        printf("</head>\n");
	    
    printf("<font color=red size=+2>");
    sprintf(Msg, "%s %s %s",msg13, user, msg14);
    printf(Msg);
    printf("</font>\n");
    
    if(strlen(PwdLogFile) > 0)
       log(PwdLogFile,user,newpwd,showpwd,Msg,PwdFile,SmtpUser,SmtpSubject);
    return;
}


static void
fixpwd(str)
        unsigned char   *str;
{
        unsigned char   *dest = str;

        while (str[0])
        {
                if (str[0] == '+')
                        dest[0] = ' ';
                else if (str[0] == '%' && hhex(str[1]) && hhex(str[2]))
                {
                        dest[0] = (unsigned char) htoi(str + 1);
                        str += 2;
                }
                else
                        dest[0] = str[0];

                str++;
                dest++;
        }

        dest[0] = '\0';
	return;

}


static int
htoi(s)
        unsigned char   *s;
{
        int     value;
        char    c;

        c = s[0];
        if (isupper(c))
                c = tolower(c);
        value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

        c = s[1];
        if (isupper(c))
                c = tolower(c);
        value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

        return (value);
}


void getconf(ConfigFile, IpAuth, PwdFile, PwdMinLen, PwdMaxLen, PwdMinChar, PwdMinNum, PwdMinSpec, PwdLogFile, ShowPwd, BgColor, TxColor, TiColor, RuColor, Logo, Width, Height, Header, BgImage, SmtpUser, SmtpSubject)
	unsigned char *ConfigFile;
	unsigned char *PwdFile;
	unsigned char *IpAuth;
	unsigned char *PwdLogFile;
        unsigned char *ShowPwd;
	unsigned char *PwdMinLen;
	unsigned char *PwdMaxLen;
	unsigned char *PwdMinChar;
	unsigned char *PwdMinNum;
	unsigned char *PwdMinSpec;
	unsigned char *BgColor;
	unsigned char *TxColor;
	unsigned char *TiColor;
	unsigned char *RuColor;
	unsigned char *Logo;
	unsigned char *Width;
	unsigned char *Height;
	unsigned char *Header;
	unsigned char *BgImage;
	unsigned char *SmtpUser;
	unsigned char *SmtpSubject;
{
        
        FILE *fp_in;
        char buf[255];
        char wbuf[255];
	char Msg[255];
	char *str;

        if ((fp_in = fopen(ConfigFile, "r")) == NULL) {
            sprintf(Msg, "%s %s", msg15, ConfigFile);
	    Hmsg(Msg);
	    exit(1);
	}

	while (fgets(buf, MAXLEN, fp_in) != NULL) {
	   if((str=(char *) strstr(buf,"#")) != (char *) NULL) 
	      continue;

 	   if(strstr(buf,"password_file") != 0) {
	      getword(wbuf,buf,' ');
	      getword(PwdFile,buf,' ');
	      PwdFile[strlen(PwdFile)-1]='\0';
	   }

 	   if(strstr(buf,"ip_auth") != 0) {
	      getword(wbuf,buf,' ');
	      getword(IpAuth,buf,' ');
	      IpAuth[strlen(IpAuth)-1]='\0';
	   }

 	   if(strstr(buf,"minimum_length") != 0) {
	      getword(wbuf,buf,' ');
	      getword(PwdMinLen,buf,' ');
	      PwdMinLen[strlen(PwdMinLen)-1]='\0';
	   }

 	   if(strstr(buf,"maximum_length") != 0) {
	      getword(wbuf,buf,' ');
	      getword(PwdMaxLen,buf,' ');
	      PwdMaxLen[strlen(PwdMaxLen)-1]='\0';
	   }

 	   if(strstr(buf,"minimum_char") != 0) {
	      getword(wbuf,buf,' ');
	      getword(PwdMinChar,buf,' ');
	      PwdMinChar[strlen(PwdMinChar)-1]='\0';
	   }

 	   if(strstr(buf,"minimum_num") != 0) {
	      getword(wbuf,buf,' ');
	      getword(PwdMinNum,buf,' ');
	      PwdMinNum[strlen(PwdMinNum)-1]='\0';
	   }

 	   if(strstr(buf,"minimum_spec") != 0) {
	      getword(wbuf,buf,' ');
	      getword(PwdMinSpec,buf,' ');
	      PwdMinSpec[strlen(PwdMinSpec)-1]='\0';
	   }

 	   if(strstr(buf,"enable_log") != 0) {
	      getword(wbuf,buf,' ');
	      getword(PwdLogFile,buf,'\0');
	      PwdLogFile[strlen(PwdLogFile)-1]='\0';
	   }

 	   if(strstr(buf,"show_pwd") != 0) {
	      getword(wbuf,buf,' ');
	      getword(ShowPwd,buf,'\0');
	      ShowPwd[strlen(ShowPwd)-1]='\0';
	   }

 	   if(strstr(buf,"background_color") != 0) {
	      getword(wbuf,buf,' ');
              getword(BgColor,buf,'\0');
	      BgColor[strlen(BgColor)-1]='\0';
	   }

 	   if(strstr(buf,"text_color") != 0) {
	      getword(wbuf,buf,' ');
	      getword(TxColor,buf,'\0');
	      TxColor[strlen(TxColor)-1]='\0';
	   }

 	   if(strstr(buf,"title_color") != 0) {
	      getword(wbuf,buf,' ');
	      getword(TiColor,buf,'\0');
	      TiColor[strlen(TiColor)-1]='\0';
	   }

 	   if(strstr(buf,"rules_color") != 0) {
	      getword(wbuf,buf,' ');
	      getword(RuColor,buf,'\0');
	      RuColor[strlen(RuColor)-1]='\0';
	   }

 	   if(strstr(buf,"logo_image") != 0) {
	      getword(wbuf,buf,' ');
	      getword(Logo,buf,'\0');
	      Logo[strlen(Logo)-1]='\0';
	   }

 	   if(strstr(buf,"background_image") != 0) {
	      getword(wbuf,buf,' ');
	      getword(BgImage,buf,'\0');
	      BgImage[strlen(BgImage)-1]='\0';
	   }

 	   if(strstr(buf,"alert_mail_user") != 0) {
	      getword(wbuf,buf,' ');
	      getword(SmtpUser,buf,'\0');
	      SmtpUser[strlen(SmtpUser)-1]='\0';
	   }

 	   if(strstr(buf,"alert_mail_subject") != 0) {
	      getword(wbuf,buf,'"');
	      getword(SmtpSubject,buf,'"');
	   }

 	   if(strstr(buf,"image_size") != 0) {
	      getword(wbuf,buf,' ');
	      getword(Width,buf,' ');
	      getword(Height,buf,'\0');
	      Height[strlen(Height)-1]='\0';
	   }

 	   if(strstr(buf,"header") != 0) {
	      getword(wbuf,buf,'"');
	      getword(Header,buf,'"');
	   }

	}
	
        fclose(fp_in);
	return;
}


void log(pwdlogfile, user, newpwd, showpwd, msg, pwdfile, smtpuser, smtpsubject)
	unsigned char *pwdlogfile;
	unsigned char *user;
	unsigned char *newpwd;
	unsigned char *showpwd;
	unsigned char *msg;
	unsigned char *pwdfile;
	unsigned char *smtpuser;
	unsigned char *smtpsubject;
{
	FILE *fp_ou;
	char Msg[MAXLEN];
	time_t tm;
	struct tm *t;
	char time_string[128];
	char pwd[MAXLEN];
	
	tm = time(NULL);
	t = localtime(&tm);
	strftime(time_string, 127, "%Y/%m/%d %H:%M:%S", t);

	if ((fp_ou = fopen(pwdlogfile, "a")) == NULL) {
            sprintf(Msg, "%s %s",msg16, pwdlogfile);
	    Hmsg(Msg);
	    exit(1);
        }

        Msg[0]='\0';
        strcpy(pwd,"-");
	
        if(strcmp(showpwd,"yes") == 0){
           if(strlen(newpwd) > 0)
	      strcpy(pwd,newpwd);
	}

        if(strlen(user) == 0)
           strcpy(user,"-");

        sprintf(Msg, "%s %s %s %s %s pwdfile=%s\n",getenv("REMOTE_ADDR"),time_string,user,pwd,msg,pwdfile);
        Msg[strlen(Msg)-1]='\n';
	strip_latin(Msg);
	fputs(Msg,fp_ou);
	fclose(fp_ou);

        if(strlen(smtpuser) > 0)
           sendmail(smtpuser,smtpsubject,Msg);

	return;

}


static void vrfyrule(unsigned char *str, unsigned char *minchar, unsigned char *minnum, unsigned char *minspec)
{
    int tnum=0, talfa=0, tspec=0;
    char *ostr = str;

    while(str[0]) {
       if (str[0] == '%' && hhex(str[1]) && hhex(str[2])) {
          tspec++;
	  str=str+3;
	  continue;
       }
       if (str[0] == '.') {
          tspec++;
	  str++;
	  continue;
       }
       if ((str[0] >= 'a' && str[0] <= 'z') || (str[0] >='A' && str[0] <= 'Z')) 
          talfa++;
       if (str[0] >= '0' && str[0] <= '9')
          tnum++;

       str++;
    }

    if(talfa < atoi(minchar))
       rulefail(minchar,minnum,minspec);

    if(tnum < atoi(minnum))
       rulefail(minchar,minnum,minspec);

    if(tspec < atoi(minspec))
       rulefail(minchar,minnum,minspec);

    return;
}


void
rulefail(char *minchar, char *minnum, char *minspec)
{
    char Msg[MAXLEN];

    printf("Content-type: text/html\n");
    puts("\n");
    printf("<html>\n");
  printf("<head>\n");
    printf("<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=koi8-r\">");
      printf("  <title>Изменение пароля доступа к Squid</title>\n");
        printf("</head>\n");
    printf("<font color=red size=+2>");
    sprintf(Msg,"%s</font><br><br>",msg17);
    printf(Msg);
    sprintf(Msg,"%s<br><br>\n",msg18);
    printf(Msg);

    if(strcmp(minchar,"0") != 0)
       printf("<table><tr><td align=right>%s</td><td align=left><b>%s</b></td></tr>\n",msg19,minchar);
      else
       printf("<table><tr><td align=right>%s</td><td align=left><b></b></td></tr>\n",msg19);

    if(strcmp(minnum,"0") != 0)
       printf("<tr><td align=right>%s</td><td align=left><b>%s</b></td></tr>\n",msg20,minnum);
     else
       printf("<tr><td align=right>%s</td><td align=left><b></b></td></tr>\n",msg20);

    if(strcmp(minspec,"0") != 0)
       printf("<tr><td align=right>%s</td><td align=left><b>%s</b></td></tr>\n",msg21,minspec);
     else
       printf("<tr><td align=right>%s</td><td align=left><b></b></td></tr>\n",msg21);

    printf("</table>\n");

    exit(1);
}


static void strip_latin(char *line)
{
   char buf[255];
   char warea[255];

   while(strstr(line,"&") != 0){
      getword(warea,line,'&');
      strncat(warea,line,1);
      getword(buf,line,';');
      strcat(warea,line);
      strcpy(line,warea);
   }
   
   return;

}


static void vrfyauth(char *authfile, char *PwdLogFile, char *PwdFile, char *SmtpUser, char *SmtpSubject)
{

   FILE *auth;
   char IP[30];
   char buf[MAXLEN];
   int  authbit=0;
   struct stat statb;

   if(stat(authfile,&statb)) eperror(authfile);
   if(statb.st_uid != 0) {
      Hmsg(msg23);
      if(strlen(PwdLogFile) > 0)
         log(PwdLogFile,"-","","no",msg23,PwdFile,SmtpUser,SmtpSubject);
      exit(1);
   }
   if(statb.st_mode & 022) {
      Hmsg(msg24);
      if(strlen(PwdLogFile) > 0)
         log(PwdLogFile,"-","","no",msg24,PwdFile,SmtpUser,SmtpSubject);
      exit(1);
   }

   if((auth=fopen(authfile,"r"))==NULL) {
      Hmsg(msg25);
      if(strlen(PwdLogFile) > 0)
         log(PwdLogFile,"-","","no",msg25,PwdFile,SmtpUser,SmtpSubject);
      exit(1);
   }

   if(getenv("HTTP_X_FORWARDED_FOR"))
      sprintf(IP,"%s",getenv("HTTP_X_FORWARDED_FOR"));
    else sprintf(IP,"%s",getenv("REMOTE_ADDR"));

   while(fgets(buf,MAXLEN,auth)!=NULL) {
      if(!authbit) {
         if(strstr(buf,"#") != 0)
            continue;
          else  authbit=vauth(buf,IP);
      }
   }

   if(!authbit) {
      Hmsg(msg26);
      if(strlen(PwdLogFile) > 0)
         log(PwdLogFile,"-","","no",msg26,PwdFile,SmtpUser,SmtpSubject);
      exit(1);
   }
   
   return;

}


int vauth(char *buf, char *ip)
{
   char a1[4],a2[4],a3[4],a4[4];
   char o1[4],o2[4],o3[4],o4[4];
   char ip2[255];
   int ok=1;

   strcpy(ip2,ip);

   buf[strlen(buf)-1]='\0';

   if(strcmp(buf,"0.0.0.0") == 0)
      return(1);
   if(strcmp(buf,ip2) == 0)
      return(1);

   getword(a1,buf,'.');
   getword(a2,buf,'.');
   getword(a3,buf,'.');
   getword(a4,buf,' ');

   getword(o1,ip2,'.');
   getword(o2,ip2,'.');
   getword(o3,ip2,'.');
   getword(o4,ip2,' ');

   if(strcmp(a1,"0") != 0){
      if(strcmp(a1,o1) != 0)
         ok=0;
   }
   if(strcmp(a2,"0") != 0){
      if(strcmp(a2,o2) != 0)
         ok=0;
   }
   if(strcmp(a3,"0") != 0){
      if(strcmp(a3,o3) != 0)
         ok=0;
   }
   if(strcmp(a4,"0") != 0){
      if(strcmp(a4,o4) != 0)
         ok=0;
   }

   return(ok);
}


eperror(s)
register char *s;
{
   perror("chpasswd.cgi");
   exit(1);

}


void sendmail(char *user, char *subj, char *msg)
{

   FILE *mail;
   char cmd[MAXLEN];

   sprintf(cmd,"mailx -s '%s' %s",subj,user);

   if (access("/bin/mailx", R_OK) == 0)
      sprintf(cmd,"/bin/mailx -s '%s' %s",subj,user);
   if (access("/usr/bin/mailx", R_OK) == 0)
      sprintf(cmd,"/bin/mailx -s '%s' %s",subj,user);

   if((mail =popen(cmd, "w")) != NULL ) {
      fputs(msg,mail);
      pclose(mail);
   }


//   FILE *mail = popen(MAILPROG " -s 'Test Message' root", "w");
//   if (!mail) {
//      perror("popen");
//      exit(1);
//   }
//     
//   fprintf(mail, "This is a test.\n");
//     
//         if (pclose(mail))
//         {
//             fprintf(stderr, "mail failed!\n");
//             exit(1);
//         }
//     }
   return;
}
