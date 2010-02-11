/* English language added by Pedro L Orso <porso@usa.net> */

#if defined(English)

char *msg01 = "No write permission on password file:";
char *msg02 = "New password missing or too short (less than";
char *msg03 = "characters).";
char *msg04 = "New password too long (more than";
char *msg05 = "Please, don't use the same password";
char *msg06 = "New password don't match.";
char *msg07 = "Could not open passwd file:";
char *msg08 = "Could not open temp file.";
char *msg09 = "Wrong password for user:";
char *msg10 = "Password not changed.";
char *msg11 = "Password changed for user";
char *msg12 = "";
char *msg13 = "User:";
char *msg14 = "not found.";
char *msg15 = "Could not open config file:";
char *msg16 = "Could not open log file:";
char *msg17 = "Invalid new password.";
char *msg18 = "Please, use the following rules to enter a new password:";
char *msg19 = "Minimum of characters (a-z, A-Z)";
char *msg20 = "Minimum of nummbers (0-9)";
char *msg21 = "Minimum of special characters (keyboard)";
char *msg22 = "Change your Squid Account Password";
char *msg23 = "Authorization file must be owned by root. Please README.";
char *msg24 = "Authorization file with bad mode (must be 755). Please README.";
char *msg25 = "Could not open auth file.";
char *msg26 = "You are not authorized to change password.";
char *msg27 = "Authorization file not found.";

static void UserForm(char *minlen, char *maxlen, char *minchar, char *minnum, char *minspec, char *bgcolor, char *txcolor, char *ticolor, char *rucolor, char *logo, char *width, char *height, char *header, char *bgimage)
{

  char rules[255] = "<li>Composition:";
  char wrules[255];
  int  frules=0;

  printf("Content-type: text/html\n");
  puts("\n");
  printf("<html>\n");
  printf("<head>\n");
  printf("  <title>Change Squid Password</title>\n");
  printf("</head>\n");
  printf("<body bgcolor=%s text=%s background='%s'>\n",bgcolor,txcolor,bgimage);

  if(strlen(logo) > 0)
     printf("<h2><img src='%s' border=0 align=absmiddle width=%s height=%s><font color=%s>%s</font></h2>\n",logo,width,height,ticolor,header);
   else
     printf("<h2><font color=%s>%s</font></h2>\n",ticolor,header);

  printf("For security reasons, remember these rules about your new password:\n");
  printf("<ul>\n");
  printf("<li>The password is letter-case sensitive, meaning an 'A' is not the same as an 'a'.<br>\n");
  printf("<li>You may use letters, numbers, and other special characters on your keyboard.<br>\n");
  printf("<li>New password rules:<br>");
  printf("<ul><li>Length: minimum <font color=%s><b>%s</b></font>, maximum <font color=%s><b>%s</b></font> characters.<br>\n",rucolor,minlen,rucolor,maxlen);

  if(strcmp(minchar,"0") != 0) {
     sprintf(wrules," characters (a-z, A-Z):<font color=%s><b> %s</b></font>",rucolor,minchar);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minnum,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," numbers (0-9):<font color=%s><b> %s</b></font>",rucolor,minnum);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minspec,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," special characters (keyboard):<font color=%s><b> %s</b></font>",rucolor,minspec);
     strcat(rules,wrules);
     frules=1;
  }
  if(!frules){
     sprintf(wrules,"<font color=%s><b> free choice</b></font>",rucolor);
     strcat(rules,wrules);
  }
  strcat(rules,"<br>");
  printf(rules);
  printf("</ul>\n");

  printf("<li>Your new password will be effective within about 15 minutes.\n");
  printf("</ul>\n");
  printf("<form action='chpasswd.cgi' method=POST>\n");
  printf("<center>\n");
  printf("<hr noshade>\n");
  printf("<table cellpadding=0 cellspacing=0>\n");
  printf("  <tr><td align=right>Your Account Name:\n");
  printf("      <td><input type=text name=user size=10>\n");
  printf("  <tr><td align=right>Your Current Password:\n");
  printf("      <td><input type=password name=old_pw size=%s>\n",maxlen);
  printf("  <tr><td align=right>New Password:\n");
  printf("      <td><input type=password name=new_pw1 size=%s>\n",maxlen);
  printf("  <tr><td align=right>Retype New Password:\n");
  printf("      <td><input type=password name=new_pw2 size=%s>\n",maxlen);
  printf("</table>\n");
  printf("<hr noshade>\n");
  printf("<input type=submit name=change value='Change My Password'>\n");
  printf("<input type=reset name=reset value='Clear fields'>\n");
  printf("<input type=submit name=cancel value='Cancel'>\n");
  printf("</center>\n");
  printf("</form>\n");
  printf("<a href='%s'>%s-%s</a>\n",URL,PGM,VERSION);
  printf("</body>\n");
  printf("</html>\n");

  fflush(stdout);
  return;

}
#endif
