/* Italian language added by Luigi Gangitano <lgangitano@netscape.net> */

#if defined(Italian)

char *msg01 = "Diritti insufficienti per scrivere il file delle password:";
char *msg02 = "Manca la nuova password o e' troppo corta (meno di";
char *msg03 = "caratteri).";
char *msg04 = "La nuova password e' troppo lunga (piu' di";
char *msg05 = "Per favore, non usare la stessa password";
char *msg06 = "La nuova password non combacia.";
char *msg07 = "Non e' possibile aprire il file delle password:";
char *msg08 = "Non e' possibile aprire il file temporaneo.";
char *msg09 = "Password errata per l'utente:";
char *msg10 = "La password no e' stata cambiata.";
char *msg11 = "E' stata cambiata la password dell'utente";
char *msg12 = "";
char *msg13 = "Utente:";
char *msg14 = "non trovato.";
char *msg15 = "Non e' possibile aprire il file di configurazione:";
char *msg16 = "Non e' possibile aprire il file di log:";
char *msg17 = "La nuova password non e' valida.";
char *msg18 = "Per favore, usare le seguenti regole per inserire la nuova passowrd:";
char *msg19 = "Numero di lettere (minimo) (a-z, A-Z)";
char *msg20 = "Numero di cifre (minimo9 (0-9)";
char *msg21 = "Numero di caratteri speciali (minimo) (eg: %,&,*,ecc.)";
char *msg22 = "Cambia la tua Password di Accesso al server Squid";
char *msg23 = "Il file di autorizzazione deve essere intestato al root. Leggere il file README.";
char *msg24 = "Il file di autorizzazione ha permessi insufficienti (deve essere 755). Leggere il file README.";
char *msg25 = "Impossibile aprire il file di autorizzazione.";
char *msg26 = "Non sei autorizzato a cambiare la password.";
char *msg27 = "Il file di autorizzazione non e' stato trovato.";

static void UserForm(char *minlen, char *maxlen, char *minchar, char *minnum, char *minspec, char *bgcolor, char *txcolor, char *ticolor, char *rucolor, char *logo, char *width, char *height, char *header, char *bgimage)
{

  char rules[255] = "<li>Composizione:";
  char wrules[255];
  int  frules=0;

  printf("Content-type: text/html\n");
  puts("\n");
  printf("<html>\n");
  printf("<head>\n");
  printf("  <title>Cambia la Password del server Squid</title>\n");
  printf("</head>\n");
  printf("<body bgcolor=%s text=%s background='%s'>\n",bgcolor,txcolor,bgimage);

  if(strlen(logo) > 0)
     printf("<h2><img src='%s' border=0 align=absmiddle width=%s height=%s><font color=%s>%s</font></h2>\n",logo,width,height,ticolor,header);
   else
     printf("<h2><font color=%s>%s</font></h2>\n",ticolor,header);

  printf("Per ragioni di sicurezza, ricorda queste avvertenze riguardo alla tua password:\n");
  printf("<ul>\n");
  printf("<li>La password e' case-sensitive, il che vuol dire che 'A' non e' la stessa cosa di 'a'.<br>\n");
  printf("<li>Puoi usare lettere, numeri o caratteri speciali della tua tastiera.<br>\n");
  printf("<li>Regole riguardanti la nuova password:<br>");
  printf("<ul><li>Lunghezza: minimo <font color=%s><b>%s</b></font>, massimo <font color=%s><b>%s</b></font> caratteri.<br>\n",rucolor,minlen,rucolor,maxlen);

  if(strcmp(minchar,"0") != 0) {
     sprintf(wrules," lettere (a-z, A-Z):<font color=%s><b> %s</b></font>",rucolor,minchar);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minnum,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," numeri (0-9):<font color=%s><b> %s</b></font>",rucolor,minnum);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minspec,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," caratteri speciali (eg: %,&,*,ecc.):<font color=%s><b> %s</b></font>",rucolor,minspec);
     strcat(rules,wrules);
     frules=1;
  }
  if(!frules){
     sprintf(wrules,"<font color=%s><b> Scelta libera</b></font>",rucolor);
     strcat(rules,wrules);
  }
  strcat(rules,"<br>");
  printf(rules);
  printf("</ul>\n");

  printf("<li>La tua nuova password sara' attivata entro 15 minuti.\n");
  printf("</ul>\n");
  printf("<form action='chpasswd.cgi' method=POST>\n");
  printf("<center>\n");
  printf("<hr noshade>\n");
  printf("<table cellpadding=0 cellspacing=0>\n");
  printf("  <tr><td align=right>Il tuo Account:\n");
  printf("      <td><input type=text name=user size=10>\n");
  printf("  <tr><td align=right>La tua Password attuale:\n");
  printf("      <td><input type=password name=old_pw size=%s>\n",maxlen);
  printf("  <tr><td align=right>La nuova Password:\n");
  printf("      <td><input type=password name=new_pw1 size=%s>\n",maxlen);
  printf("  <tr><td align=right>Riscrivi la nuova Password:\n");
  printf("      <td><input type=password name=new_pw2 size=%s>\n",maxlen);
  printf("</table>\n");
  printf("<hr noshade>\n");
  printf("<input type=submit name=change value='Cambia la mia password'>\n");
  printf("<input type=reset name=reset value='Pulisci i campi'>\n");
  printf("<input type=submit name=cancel value='Cancella'>\n");
  printf("</center>\n");
  printf("</form>\n");
  printf("<a href='%s'>%s-%s</a>\n",URL,PGM,VERSION);
  printf("</body>\n");
  printf("</html>\n");

  fflush(stdout);
  return;

}
#endif
