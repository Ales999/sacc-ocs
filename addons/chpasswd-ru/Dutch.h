/* Dutch language added by Richard Baselier <baselier@baselier.demon.nl> */

#if defined(Dutch)

char *msg01 = "Geen schrijfpermissie op paswoord file:";
char *msg02 = "Nieuw paswoord leeg of te kort (minder dan";
char *msg03 = "karakters).";
char *msg04 = "Nieuw paswoord te lang (meer dan";
char *msg05 = "Aub, niet hetzelfde paswoord gebruiken";
char *msg06 = "Nieuwe paswoorden zijn niet gelijk.";
char *msg07 = "Kan niet openen paswoord file:";
char *msg08 = "Kan temp file niet openen.";
char *msg09 = "Verkeerd paswoord voor gebruiker:";
char *msg10 = "Paswoord niet gewijzigd.";
char *msg11 = "Password gewijzigd voor gebruiker";
char *msg12 = "";
char *msg13 = "Gebruiker:";
char *msg14 = "niet gevonden.";
char *msg15 = "Kan niet openen config file:";
char *msg16 = "Kan niet openen log file:";
char *msg17 = "Invalide nieuw paswoord.";
char *msg18 = "Aub., gebruik de volgende regels voor een nieuw paswoord:";
char *msg19 = "Minimum aantal karakters (a-z, A-Z)";
char *msg20 = "Minimum aantal nummers (0-9)";
char *msg21 = "Minimum aantal speciale karakters (toetsenbord)";
char *msg22 = "Wijzig uw Squid Account Paswoord";
char *msg23 = "Authorisatie file moet eigenaar root hebben. Aub. README.";
char *msg24 = "Authorisatie file heeft verkeerde mode (moet 755 zijn). Aub. README.";
char *msg25 = "Kan authorisatie file niet openen."; 
char *msg26 = "U bent niet bevoegd het paswoord te wijzigen.";
char *msg27 = "Authorisatie file niet gevonden.";

static void UserForm(char *minlen, char *maxlen, char *minchar, char *minnum, char *minspec, char *bgcolor, char *txcolor, char *ticolor, char *rucolor, char *logo, char *width, char *height, char *header, char *bgimage)
{

  char rules[255] = "<li>Compositie:";
  char wrules[255];
  int  frules=0;

  printf("Content-type: text/html\n");
  puts("\n");
  printf("<html>\n");
  printf("<head>\n");
  printf("  <title>Wijzig Squid paswoord</title>\n");
  printf("</head>\n");
  printf("<body bgcolor=%s text=%s background='%s'>\n",bgcolor,txcolor,bgimage);

  if(strlen(logo) > 0)
     printf("<h2><img src='%s' border=0 align=absmiddle width=%s height=%s><font color=%s>%s</font></h2>\n",logo,width,height,ticolor,header);
   else
     printf("<h2><font color=%s>%s</font></h2>\n",ticolor,header);

  printf("Voor beveiligings redenen, pas deze regels toe op uw nieuwe paswoord :\n");
  printf("<ul>\n");
  printf("<li>Het paswoord is hoofdletter gevoelig, dit betekent dat 'A' niet gelijk is aan 'a'.<br>\n");
  printf("<li>U mag letters, nummers, en andere speciale karakters op uw toetsenbord gebruiken.<br>\n");
  printf("<li>Nieuw paswoord regels:<br>");
  printf("<ul><li>Lengte: minimum <font color=%s><b>%s</b></font>, maximum <font color=%s><b>%s</b></font> characters.<br>\n",rucolor,minlen,rucolor,maxlen);

  if(strcmp(minchar,"0") != 0) {
     sprintf(wrules," karakters (a-z, A-Z):<font color=%s><b> %s</b></font>",rucolor,minchar);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minnum,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," nummers (0-9):<font color=%s><b> %s</b></font>",rucolor,minnum);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minspec,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," speciale karakters (toetsenbord):<font color=%s><b> %s</b></font>",rucolor,minspec);
     strcat(rules,wrules);
     frules=1;
  }
  if(!frules){
     sprintf(wrules,"<font color=%s><b> vrije keus</b></font>",rucolor);
     strcat(rules,wrules);
  }
  strcat(rules,"<br>");
  printf(rules);
  printf("</ul>\n");

  printf("<li>Uw nieuw paswoord is effectief binnen ongeveer 15 minuten.\n");
  printf("</ul>\n");
  printf("<form action='chpasswd.cgi' method=POST>\n");
  printf("<center>\n");
  printf("<hr noshade>\n");
  printf("<table cellpadding=0 cellspacing=0>\n");
  printf("  <tr><td align=right>Uw account naam:\n");
  printf("      <td><input type=text name=user size=10>\n");
  printf("  <tr><td align=right>Uw huidige paswoord:\n");
  printf("      <td><input type=password name=old_pw size=%s>\n",maxlen);
  printf("  <tr><td align=right>Nieuw paswoord:\n");
  printf("      <td><input type=password name=new_pw1 size=%s>\n",maxlen);
  printf("  <tr><td align=right>Nogmaals nieuw paswoord:\n");
  printf("      <td><input type=password name=new_pw2 size=%s>\n",maxlen);
  printf("</table>\n");
  printf("<hr noshade>\n");
  printf("<input type=submit name=change value='Wijzig mijn paswoord'>\n");
  printf("<input type=reset name=reset value='Maak velden leeg'>\n");
  printf("<input type=submit name=cancel value='Afbreken'>\n");
  printf("</center>\n");
  printf("</form>\n");
  printf("<a href='%s'>%s-%s</a>\n",URL,PGM,VERSION);
  printf("</body>\n");
  printf("</html>\n");

  fflush(stdout);
  return;

}
#endif
