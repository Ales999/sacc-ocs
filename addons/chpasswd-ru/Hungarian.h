/* Hungarian language re-added by Domonkos Sandor <domonkos@npp.hu> 
   Hungarian language added by Robert Szabo <rob@freemail.c3.hu> */

#if defined(Hungarian)

char *msg01 = "Nincs írási jog a jelszófile-hoz:";
char *msg02 = "Hiányzik az új jelszó vagy túl rövid (kevesebb, mint";
char *msg03 = "karakter).";
char *msg04 = "Az új jelszó túl hosszú (több, mint";
char *msg05 = "Ne használd ugyanazt a jelszót!";
char *msg06 = "Az új jelszó nem egyezik.";
char *msg07 = "Nem tudom megnyitni a jelszófile-t:";
char *msg08 = "Nem tudom megnyitni az átmeneti file-t.";
char *msg09 = "Az alábbi felhasználó jelszavát hibásan adtad meg:";
char *msg10 = "A jelszó nem változott meg.";
char *msg11 = "Az alábbi felhasználó jelszava módosításra került:";
char *msg12 = "";
char *msg13 = "Felhasználó:";
char *msg14 = "nem találom.";
char *msg15 = "Nem tudom megnyitni a config file-t:";
char *msg16 = "Nem tudom megnyitni a log file-t:";
char *msg17 = "Érvénytelen új jelszó.";
char *msg18 = "Használd az alábbi szabályokat az új jelszóhoz:";
char *msg19 = "Legalább az alábbi betüket (a-z, A-Z)";
char *msg20 = "legalább az alábbi számokat (0-9)";
char *msg21 = "Legalább az alábbi speciális jeleket (billentyüzet)";
char *msg22 = "Internet hozzáférési jogosultság módosítása";
char *msg23 = "Az autentikációs file-nak a root tulajdonában kell lennie. Olvasd el README.";
char *msg24 = "Autentikációs file hibás jogosultsággal (755-nek kell lenni). Olvasd el README.";
char *msg25 = "Nem tudom megnyitni az autentikációs file-t.";
char *msg26 = "Nincs jogod a jelszó módosítására.";
char *msg27 = "Az autentikációs file-t nem találom.";

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

  printf("Biztonsági okok miatt emlékezz az alábbi szabályokra az új jelszó megadásakor:\n");
  printf("<ul>\n");
  printf("<li>A jelszóban a kis és nagy betü különbözik, vagyis az 'A' és 'a' nem azonos.<br>\n");
  printf("<li>Használhatsz betüket, számokat és speciális karakterek a billentyüzetröl.<br>\n");
  printf("<li>Új jelszó szabályai:<br>");
  printf("<ul><li>Hossza: minimum <font color=%s><b>%s</b></font>, maximum <font color=%s><b>%s</b></font> karakter.<br>\n",rucolor,minlen,rucolor,maxlen);

  if(strcmp(minchar,"0") != 0) {
     sprintf(wrules," betük (a-z, A-Z):<font color=%s><b> %s</b></font>",rucolor,minchar);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minnum,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," számok (0-9):<font color=%s><b> %s</b></font>",rucolor,minnum);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minspec,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," speciális karakterek (billentyüzetröl):<font color=%s><b> %s</b></font>",rucolor,minspec);
     strcat(rules,wrules);
     frules=1;
  }
  if(!frules){
     sprintf(wrules,"<font color=%s><b> szabadon választva</b></font>",rucolor);
     strcat(rules,wrules);
  }
  strcat(rules,"<br>");
  printf(rules);
  printf("</ul>\n");

  printf("<li>Az új jelszavad kb. 15 percen belül érvényes lesz.\n");
  printf("</ul>\n");
  printf("<form action='chpasswd.cgi' method=POST>\n");
  printf("<center>\n");
  printf("<hr noshade>\n");
  printf("<table cellpadding=0 cellspacing=0>\n");
  printf("  <tr><td align=right>Felhasználói neved:\n");
  printf("      <td><input type=text name=user size=10>\n");
  printf("  <tr><td align=right>Jelenlegi jelszavad:\n");
  printf("      <td><input type=password name=old_pw size=%s>\n",maxlen);
  printf("  <tr><td align=right>Új jelszavad:\n");
  printf("      <td><input type=password name=new_pw1 size=%s>\n",maxlen);
  printf("  <tr><td align=right>Új jelszavad megerösítése:\n");
  printf("      <td><input type=password name=new_pw2 size=%s>\n",maxlen);
  printf("</table>\n");
  printf("<hr noshade>\n");
  printf("<input type=submit name=change value='Jelszavam módosítása'>\n");
  printf("<input type=reset name=reset value='Mezök törlése'>\n");
  printf("<input type=submit name=cancel value='Megszakítás'>\n");
  printf("</center>\n");
  printf("</form>\n");
  printf("<a href='%s'>%s-%s</a>\n",URL,PGM,VERSION);
  printf("</body>\n");
  printf("</html>\n");

  fflush(stdout);
  return;

}
#endif
