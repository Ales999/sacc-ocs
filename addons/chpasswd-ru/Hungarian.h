/* Hungarian language re-added by Domonkos Sandor <domonkos@npp.hu> 
   Hungarian language added by Robert Szabo <rob@freemail.c3.hu> */

#if defined(Hungarian)

char *msg01 = "Nincs �r�si jog a jelsz�file-hoz:";
char *msg02 = "Hi�nyzik az �j jelsz� vagy t�l r�vid (kevesebb, mint";
char *msg03 = "karakter).";
char *msg04 = "Az �j jelsz� t�l hossz� (t�bb, mint";
char *msg05 = "Ne haszn�ld ugyanazt a jelsz�t!";
char *msg06 = "Az �j jelsz� nem egyezik.";
char *msg07 = "Nem tudom megnyitni a jelsz�file-t:";
char *msg08 = "Nem tudom megnyitni az �tmeneti file-t.";
char *msg09 = "Az al�bbi felhaszn�l� jelszav�t hib�san adtad meg:";
char *msg10 = "A jelsz� nem v�ltozott meg.";
char *msg11 = "Az al�bbi felhaszn�l� jelszava m�dos�t�sra ker�lt:";
char *msg12 = "";
char *msg13 = "Felhaszn�l�:";
char *msg14 = "nem tal�lom.";
char *msg15 = "Nem tudom megnyitni a config file-t:";
char *msg16 = "Nem tudom megnyitni a log file-t:";
char *msg17 = "�rv�nytelen �j jelsz�.";
char *msg18 = "Haszn�ld az al�bbi szab�lyokat az �j jelsz�hoz:";
char *msg19 = "Legal�bb az al�bbi bet�ket (a-z, A-Z)";
char *msg20 = "legal�bb az al�bbi sz�mokat (0-9)";
char *msg21 = "Legal�bb az al�bbi speci�lis jeleket (billenty�zet)";
char *msg22 = "Internet hozz�f�r�si jogosults�g m�dos�t�sa";
char *msg23 = "Az autentik�ci�s file-nak a root tulajdon�ban kell lennie. Olvasd el README.";
char *msg24 = "Autentik�ci�s file hib�s jogosults�ggal (755-nek kell lenni). Olvasd el README.";
char *msg25 = "Nem tudom megnyitni az autentik�ci�s file-t.";
char *msg26 = "Nincs jogod a jelsz� m�dos�t�s�ra.";
char *msg27 = "Az autentik�ci�s file-t nem tal�lom.";

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

  printf("Biztons�gi okok miatt eml�kezz az al�bbi szab�lyokra az �j jelsz� megad�sakor:\n");
  printf("<ul>\n");
  printf("<li>A jelsz�ban a kis �s nagy bet� k�l�nb�zik, vagyis az 'A' �s 'a' nem azonos.<br>\n");
  printf("<li>Haszn�lhatsz bet�ket, sz�mokat �s speci�lis karakterek a billenty�zetr�l.<br>\n");
  printf("<li>�j jelsz� szab�lyai:<br>");
  printf("<ul><li>Hossza: minimum <font color=%s><b>%s</b></font>, maximum <font color=%s><b>%s</b></font> karakter.<br>\n",rucolor,minlen,rucolor,maxlen);

  if(strcmp(minchar,"0") != 0) {
     sprintf(wrules," bet�k (a-z, A-Z):<font color=%s><b> %s</b></font>",rucolor,minchar);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minnum,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," sz�mok (0-9):<font color=%s><b> %s</b></font>",rucolor,minnum);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minspec,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," speci�lis karakterek (billenty�zetr�l):<font color=%s><b> %s</b></font>",rucolor,minspec);
     strcat(rules,wrules);
     frules=1;
  }
  if(!frules){
     sprintf(wrules,"<font color=%s><b> szabadon v�lasztva</b></font>",rucolor);
     strcat(rules,wrules);
  }
  strcat(rules,"<br>");
  printf(rules);
  printf("</ul>\n");

  printf("<li>Az �j jelszavad kb. 15 percen bel�l �rv�nyes lesz.\n");
  printf("</ul>\n");
  printf("<form action='chpasswd.cgi' method=POST>\n");
  printf("<center>\n");
  printf("<hr noshade>\n");
  printf("<table cellpadding=0 cellspacing=0>\n");
  printf("  <tr><td align=right>Felhaszn�l�i neved:\n");
  printf("      <td><input type=text name=user size=10>\n");
  printf("  <tr><td align=right>Jelenlegi jelszavad:\n");
  printf("      <td><input type=password name=old_pw size=%s>\n",maxlen);
  printf("  <tr><td align=right>�j jelszavad:\n");
  printf("      <td><input type=password name=new_pw1 size=%s>\n",maxlen);
  printf("  <tr><td align=right>�j jelszavad meger�s�t�se:\n");
  printf("      <td><input type=password name=new_pw2 size=%s>\n",maxlen);
  printf("</table>\n");
  printf("<hr noshade>\n");
  printf("<input type=submit name=change value='Jelszavam m�dos�t�sa'>\n");
  printf("<input type=reset name=reset value='Mez�k t�rl�se'>\n");
  printf("<input type=submit name=cancel value='Megszak�t�s'>\n");
  printf("</center>\n");
  printf("</form>\n");
  printf("<a href='%s'>%s-%s</a>\n",URL,PGM,VERSION);
  printf("</body>\n");
  printf("</html>\n");

  fflush(stdout);
  return;

}
#endif
