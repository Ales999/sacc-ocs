/* French language added by Alexandre Allien <aallien@teaser.fr> */

#if defined(French)

char *msg01 = "Vous n'avez pas le droit d'ecriture dans le fichier des mots de passe:";
char *msg02 = "Le nouveau mot de passe est maquant ou trop court (moins de";
char *msg03 = "caractères).";
char *msg04 = "Le nouveau mot de passe est trop long (plus de";
char *msg05 = "SVP, n'utilisez pas le même mot de passe";
char *msg06 = "Les nouveaux mots de passe sont différent.";
char *msg07 = "Ouverture du fichier mot de passe impossible:";
char *msg08 = "Impossible d'ouvrir le fichier temporaire.";
char *msg09 = "Mot de passe incorrect pour l'utilisateur:";
char *msg10 = "Le mot de passe n'a pas été change.";
char *msg11 = "Le mot de passe a été changé pour l'utilisateur ";
char *msg12 = "";
char *msg13 = "Utilisateur:";
char *msg14 = "non trouvé.";
char *msg15 = "Impossible d'ouvrir le fichier de configuration:";
char *msg16 = "Impossible d'ouvrir le fichier de log:";
char *msg17 = "Le nouveau mot de passe est invalide.";
char *msg18 = "SVP, utilisez les règles suivantes pour saisir un nouveau mot de passe:";
char *msg19 = "Un minimun de caractères (a-z, A-Z)";
char *msg20 = "Un minimun de chiffres (0-9)";
char *msg21 = "Un minimun de caractères spéciaux"; 
char *msg22 = "Mise à jour du mot de passe d'accès à Squid";
char *msg23 = "Le fichier d'Authentification doit être possedé par l'utilisateur root. Lisez README.";
char *msg24 = "Le fichier d'Authentification ne possède pas les bons droits (il doit etre en 755). Lisez README.";
char *msg25 = "Impossible d'ouvrir le fichier d'authentification.";
char *msg26 = "Vous n'étes pas autorisé a changer votre mot de passe.";
char *msg27 = "Impossible de trouver le fichier d'authentification.";

static void UserForm(char *minlen, char *maxlen, char *minchar, char *minnum, char *minspec, char *bgcolor, char *txcolor, char *ticolor, char *rucolor, char *logo, char *width, char *height, char *header, char *bgimage)
{

  char rules[255] = "<li>Composition:";
  char wrules[255];
  int  frules=0;

  printf("Content-type: text/html\n");
  puts("\n");
  printf("<html>\n");
  printf("<head>\n");
  printf("  <title>Mise à jour du mot de passe Squid</title>\n");
  printf("</head>\n");
  printf("<body bgcolor=%s text=%s background='%s'>\n",bgcolor,txcolor,bgimage);

  if(strlen(logo) > 0)
     printf("<h2><img src='%s' border=0 align=absmiddle width=%s height=%s><font color=%s>%s</font></h2>\n",logo,width,height,ticolor,header);
   else
     printf("<h2><font color=%s>%s</font></h2>\n",ticolor,header);

  printf("Pour des raisons de sécurité, rappelez-vous les règles pour votre nouveau mot de passe:\n");
  printf("<ul>\n");
  printf("<li>Le mot de passe tient compte de la casse, cela signifie qu'un 'A' est différent d'un 'a'.<br>\n");
  printf("<li>Il est possible d'utiliser des lettres, des chiffres et des caractères spéciaux (disponible sur le clavier).<br>\n");
  printf("<li>Règles du nouveau mot de passe:<br>");
  printf("<ul><li>Longueur: minimum <font color=%s><b>%s</b></font>, maximum <font color=%s><b>%s</b></font> caractères.<br>\n",rucolor,minlen,rucolor,maxlen);

  if(strcmp(minchar,"0") != 0) {
     sprintf(wrules," caractères (a-z, A-Z):<font color=%s><b> %s</b></font>",rucolor,minchar);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minnum,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," chiffres (0-9):<font color=%s><b> %s</b></font>",rucolor,minnum);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minspec,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," caractères spéciaux (clavier):<font color=%s><b> %s</b></font>",rucolor,minspec);
     strcat(rules,wrules);
     frules=1;
  }
  if(!frules){
     sprintf(wrules,"<font color=%s><b> libre</b></font>",rucolor);
     strcat(rules,wrules);
  }
  strcat(rules,"<br>");
  printf(rules);
  printf("</ul>\n");

  printf("<li>Votre nouveau mot de passe deviendra actif dans 15 minutes environs.\n");
  printf("</ul>\n");
  printf("<form action='chpasswd.cgi' method=POST>\n");
  printf("<center>\n");
  printf("<hr noshade>\n");
  printf("<table cellpadding=0 cellspacing=0>\n");
  printf("  <tr><td align=right>Votre nom d'utilisateur:\n");
  printf("      <td><input type=text name=user size=10>\n");
  printf("  <tr><td align=right>Votre mot de passe actuel:\n");
  printf("      <td><input type=password name=old_pw size=%s>\n",maxlen);
  printf("  <tr><td align=right>Nouveau mot de passe:\n");
  printf("      <td><input type=password name=new_pw1 size=%s>\n",maxlen);
  printf("  <tr><td align=right>Resaisissez votre nouveau mot de passe:\n");
  printf("      <td><input type=password name=new_pw2 size=%s>\n",maxlen);
  printf("</table>\n");
  printf("<hr noshade>\n");
  printf("<input type=submit name=change value='Changer le mot de passe'>\n");
  printf("<input type=reset name=reset value='Effacer les champs'>\n");
  printf("<input type=submit name=cancel value='Annuler'>\n");
  printf("</center>\n");
  printf("</form>\n");
  printf("<a href='%s'>%s-%s</a>\n",URL,PGM,VERSION);
  printf("</body>\n");
  printf("</html>\n");

  fflush(stdout);
  return;

}
#endif
