/* Spanish language added by Horacio Diaz Requejo <horacio@edg.net.mx> */

#if defined(Spanish)

char *msg01 = "El archivo de contrase&ntilde;as no tiene permisos para ser modificado:";
char *msg02 = "La nueva contrase&ntilde;a es muy peque&ntilde;a (tiene menos de";
char *msg03 = "caracteres).";
char *msg04 = "La nueva contrase&ntilde;a es muy grande (tiene mas de";
char *msg05 = "Porfavor, no use la misma contrase&ntilde;a que tenia anteriormente";
char *msg06 = "La nueva contrase&ntilde;a no coincide en ambos campos.";
char *msg07 = "No pudo abrir el archivo de contrase&ntilde;as:";
char *msg08 = "No se pudo crear un archivo temporal.";
char *msg09 = "La contrase&ntilde;a del usuario es inv&aacute;lida:";
char *msg10 = "La contrase&ntilde;a no fue cambiada.";
char *msg11 = "La contrase&ntilde;a ha sido cambiada con exito";
char *msg12 = "";
char *msg13 = "El Usuario:";
char *msg14 = "no existe.";
char *msg15 = "No pudo abrir el archivo de configuraci&oacute;n:";
char *msg16 = "No se pudo abrir el archivo de logs:";
char *msg17 = "La nueva contrasena es inv&aacute;lida.";
char *msg18 = "Porfavor, utilize las siguientes reglas para contruir su nueva contrase&ntilde;a:";
char *msg19 = "Utilize cualquiera de los siguientes caracteres (a-z, A-Z)";
char *msg20 = "Utilize n&uacute;meros (0-9)";
char *msg21 = "Utilize caracteres especiales (/,-,$,%,^,etc)";
char *msg22 = "Cambiando la contrase&ntilde;a de navegaci&oacute;n en Internet";
char *msg23 = "El archivo de autorizaci&oacute;n debe pertenecer a root. Porfavor lea el README.";
char *msg24 = "Los permisos del archivo de autorizaci&oacute;n son incorrectos (deben ser 755). Porfavor lea el README.";
char *msg25 = "No se pudo abrir el archivo de autorizaci&oacute;n.";
char *msg26 = "Usted no esta autorizado a cambiar contrase&ntilde;as.";
char *msg27 = "No se encontro el archivo de autorizaci&oacute;n.";

static void UserForm(char *minlen, char *maxlen, char *minchar, char *minnum, char *minspec, char *bgcolor, char *txcolor, char *ticolor, char *rucolor, char *logo, char *width, char *height, char *header, char *bgimage)
{

  char rules[255] = "<li>Sintaxis del password:";
  char wrules[255];
  int  frules=0;

  printf("Content-type: text/html\n");
  puts("\n");
  printf("<html>\n");
  printf("<head>\n");
  printf("  <title>Cambio de contrase&ntilde;a para navegar en internet</title>\n");
  printf("</head>\n");
  printf("<body bgcolor=%s text=%s background='%s'>\n",bgcolor,txcolor,bgimage);

  if(strlen(logo) > 0)
     printf("<h2><img src='%s' border=0 align=absmiddle width=%s height=%s><font color=%s>%s</font></h2>\n",logo,width,height,ticolor,header);
   else
     printf("<h2><font color=%s>%s</font></h2>\n",ticolor,header);

  printf("Por razones de seguridad, recuerde utilizar las siguientes reglas al momento de elegir su nueva contrase&ntilde;a:\n");
  printf("<ul>\n");
  printf("<li>La contrase&ntilde;a es sensitiva a las may&uacute;sculas y min&uacute;sculas, es decir, una  'A' no es igual a una 'a'.<br>\n");
  printf("<li>Al seleccionar su nueva contrase&ntilde;a debe utilizar letras, n&uacute;meros y caractares especiales.<br>\n");
  printf("<li>Reglas para crear su nueva contrase&ntilde;a:<br>");
  printf("<ul><li>Longitud de la contrase&ntilde;a: minimo <font color=%s><b>%s</b></font>, maximo <font color=%s><b>%s</b></font> de caracteres.<br>\n",rucolor,minlen,rucolor,maxlen);

  if(strcmp(minchar,"0") != 0) {
     sprintf(wrules," caracteres (a-z, A-Z):<font color=%s><b> %s</b></font>",rucolor,minchar);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minnum,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," n&uacute;meros (0-9):<font color=%s><b> %s</b></font>",rucolor,minnum);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minspec,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," caracteres especiales (#,/,-,etc):<font color=%s><b> %s</b></font>",rucolor,minspec);
     strcat(rules,wrules);
     frules=1;
  }
  if(!frules){
     sprintf(wrules,"<font color=%s><b> A elecci&oacute;n libre</b></font>",rucolor);
     strcat(rules,wrules);
  }
  strcat(rules,"<br>");
  printf(rules);
  printf("</ul>\n");

  printf("<li>El nuevo password comenzar&aacute; a funcionar aproximadamente en 15 minutos.\n");
  printf("</ul>\n");
  printf("<form action='chpasswd.cgi' method=POST>\n");
  printf("<center>\n");
  printf("<hr noshade>\n");
  printf("<table cellpadding=0 cellspacing=0>\n");
  printf("  <tr><td align=right>Cuenta de acceso a Internet:\n");
  printf("      <td><input type=text name=user size=10>\n");
  printf("  <tr><td align=right>Su contrase&ntilde;a actual:\n");
  printf("      <td><input type=password name=old_pw size=%s>\n",maxlen);
  printf("  <tr><td align=right>Su nueva contrase&ntilde;a:\n");
  printf("      <td><input type=password name=new_pw1 size=%s>\n",maxlen);
  printf("  <tr><td align=right>Reescriba su nueva contrase&ntilde;a:\n");
  printf("      <td><input type=password name=new_pw2 size=%s>\n",maxlen);
  printf("</table>\n");
  printf("<hr noshade>\n");
  printf("<input type=submit name=change value='Cambiar la contrase&ntilde;a'>\n");
  printf("<input type=reset name=reset value='Borrar todo'>\n");
  printf("<input type=submit name=cancel value='Cancelar'>\n");
  printf("</center>\n");
  printf("</form>\n");
  printf("<a href='%s'>%s-%s</a>\n",URL,PGM,VERSION);
  printf("</body>\n");
  printf("</html>\n");

  fflush(stdout);
  return;

}
#endif
