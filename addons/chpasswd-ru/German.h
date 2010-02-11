/* German language added by Juergen Scheiderer <jsch@odin.de> */

#if defined(German)

char *msg01 = "Sie haben kein Schreibrecht f&uuml;r die Passwortdatei:";
char *msg02 = "Fehlendes oder zu kurzes Passwort (weniger als";
char *msg03 = "Zeichen).";
char *msg04 = "Neues Passwort zu lang (mehr als";
char *msg05 = "Altes und neues Passwort d&uuml;rfen nicht gleich sein.";
char *msg06 = "Die zwei Eingaben des neuen Passwortes sind nicht gleich.";
char *msg07 = "Die Passwortdatei kann nicht ge&ouml;ffnet werden:";
char *msg08 = "Die Temp-Datei kann nicht ge&ouml;ffnet werden.";
char *msg09 = "Falsches Passwort f&uuml;r Benutzer:";
char *msg10 = "Passwort nicht ge&auml;ndert.";
char *msg11 = "Passwort ge&auml;ndert f&uuml;r Benutzer:";
char *msg12 = "";
char *msg13 = "Benutzer:";
char *msg14 = "nicht gefunden.";
char *msg15 = "Konfigurationsdatei konnte nicht ge&ouml;ffnet werden:";
char *msg16 = "Protokolldatei konnte nicht ge&ouml;ffnet werden:";
char *msg17 = "Ung&uuml;ltiges neues Passwort.";
char *msg18 = "Bitte beachten Sie die folgenden Regeln bei der Passworteingabe:";
char *msg19 = "Zeichen (a-z, A-Z)";
char *msg20 = "Ziffern (0-9)";
char *msg21 = "Sonderzeichen (Tastatur)";
char *msg22 = "Aenderung Ihres Zugriffspasswortes";
char *msg23 = "Authorisations File mit falschem Besitzer.";
char *msg24 = "Authorisations File mit falschen Zugriffsrechten.";
char *msg25 = "Konnte Auth-File nicht &ouml;ffnen.";
char *msg26 = "Sie haben keine Berechtigung, Ihr Passwort zu &auml;ndern.";
char *msg27 = "Authorisations File nicht gefunden.";

static void UserForm(char *minlen, char *maxlen, char *minchar, char *minnum, char *minspec, char *bgcolor, char *txcolor, char *ticolor, char *rucolor, char *logo, char *width, char *height, char *header, char *bgimage)
{

  char rules[255] = "<li>Zusammensetzung:";
  char wrules[255];
  int  frules=0;

  printf("Content-type: text/html\n");
  puts("\n");
  printf("<html>\n");
  printf("<head>\n");
  printf("  <title>Aendern des Zugriffspasswortes</title>\n");
  printf("</head>\n");
  printf("<body bgcolor=%s text=%s background='%s'>\n",bgcolor,txcolor,bgimage);

  if(strlen(logo) > 0)
     printf("<h2><img src='%s' border=0 align=absmiddle width=%s height=%s><font color=%s>%s</font></h2>\n",logo,width,height,ticolor,header);
   else
     printf("<h2><font color=%s>%s</font></h2>\n",ticolor,header);

  printf("Aus Sicherheitsgr&uuml;nden sollten Sie sich bei Eingabe ihres neuen Passworts an folgenden Regeln orientieren:\n");
  printf("<ul>\n");
  printf("<li>Im Passwort wird zwischen Gross- und Kleinbuchstaben unterschieden. 'A' ist also nicht gleich 'a'.<br>\n");
  printf("<li>Sie k&ouml;nnen Buchstaben, Ziffern und andere Zeichen auf Ihrer Tastatur verwenden.<br>\n");


  printf("<li>Regeln f&uuml;r das neue Passwort:<br>");
  printf("<ul><li>L&auml;nge: mindestens <font color=%s><b>%s</b></font>, h&ouml;chstens <font color=%s><b>%s</b></font> Zeichen.<br>\n",rucolor,minlen,rucolor,maxlen);

  if(strcmp(minchar,"0") != 0) {
     sprintf(wrules," Buchstaben (a-z, A-Z):<font color=%s><b> %s</b></font>",rucolor,minchar);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minnum,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," Ziffern (0-9):<font color=%s><b> %s</b></font>",rucolor,minnum);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minspec,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," Sonderzeichen (Tastatur):<font color=%s><b> %s</b></font>",rucolor,minspec);
     strcat(rules,wrules);
     frules=1;
  }
  if(!frules){
     sprintf(wrules,"<font color=%s><b> freie Zeichenwahl</b></font>",rucolor);
     strcat(rules,wrules);
  }
  strcat(rules,"<br>");
  printf(rules);
  printf("</ul>\n");

  printf("<li>Ihr neues Passwort wird innerhalb der n&auml;chsten 15 Minuten aktiviert.\n");
  printf("</ul>\n");
  printf("<form action='chpasswd.cgi' method=POST>\n");
  printf("<center>\n");
  printf("<hr noshade>\n");
  printf("<table cellpadding=0 cellspacing=0>\n");
  printf("  <tr><td align=right>Benutzername:\n");
  printf("      <td><input type=text name=user size=10>\n");
  printf("  <tr><td align=right>aktuelles Passwort:\n");
  printf("      <td><input type=password name=old_pw size=10>\n");
  printf("  <tr><td align=right>neues Passwort:\n");
  printf("      <td><input type=password name=new_pw1 size=10>\n");
  printf("  <tr><td align=right>neues Passwort wiederholen:\n");
  printf("      <td><input type=password name=new_pw2 size=10>\n");
  printf("</table>\n");
  printf("<hr noshade>\n");
  printf("<input type=submit name=change value='Passwort &auml;ndern'>\n");
  printf("<input type=reset name=reset value='Eingaben l&ouml;schen'>\n");
  printf("<input type=submit name=cancel value='Passwort doch nicht &auml;ndern'>\n");
  printf("</center>\n");
  printf("</form>\n");
  printf("<a href='%s'>%s-%s</a>\n",URL,PGM,VERSION);
  printf("</body>\n");
  printf("</html>\n");

  fflush(stdout);
  return;

}
#endif
