/* Portuguses language added by Pedro L Orso <porso@usa.net> */

#if defined(Portuguese)

char *msg01 = "Arquivo de senhas sem premiss&atilde;o para grava&ccedil;&atilde;o:";
char *msg02 = "Faltou a nova senha ou &eacute; muito curta (menor que";
char *msg03 = "caracteres).";
char *msg04 = "Nova senha muito longa (mais que ";
char *msg05 = "Por favor n&atilde;o use a mesma senha.";
char *msg06 = "Nova senha n&atilde;o coincide.";
char *msg07 = "N&atilde;o pode abrir o arquivo de senhas:";
char *msg08 = "N&atilde;o pode abrir arquivo tempor&aacute;rio.";
char *msg09 = "Senha inv&aacute;lida para o usu&aacute;rio";
char *msg10 = "Senha n&atilde;o alterada.";
char *msg11 = "Senha para o usu&aacute;rio";
char *msg12 = "foi alterada com sucesso.";
char *msg13 = "Usu&aacute;rio:";
char *msg14 = "n&atilde;o encontrado.";
char *msg15 = "N&atilde;o pode abrir o arquivo de configuracao:";
char *msg16 = "N&atilde;o pode abrir o arquivo log:";
char *msg17 = "Nova senha inv&aacute;lida.";
char *msg18 = "Por favor, use as seguintes regras para entrar com a nova senha:";
char *msg19 = "M&iacute;nimo de letras (a-z, A-Z)";
char *msg20 = "M&iacute;nimo de n&uacute;meros (0-9)";
char *msg21 = "M&iacute;nimo de caracteres especiais (teclado)";
char *msg22 = "Altera&ccedil;&atilde;o da senha de acesso ao Squid";
char *msg23 = "Arquivo de autoriza&ccedil;&atilde;o deve pertencer ao root. Por favor leia o README.";
char *msg24 = "Arquivo de autoriza&ccedil;&atilde;o deve ter como atributos 755. Por favor leia o README.";
char *msg25 = "N&atilde;o pode abrir o arquivo de autoriza&ccedil;&atilde;o.";
char *msg26 = "Voc&ecirc; n&atilde;o tem autoriza&ccedil;&atilde;o para alterar a senha.";
char *msg27 = "Arquivo de autoriza&ccedil;&atilde;o n&atilde;o encontrado";

static void UserForm(char *minlen, char *maxlen, char *minchar, char *minnum, char *minspec, char *bgcolor, char *txcolor, char *ticolor, char *rucolor, char *logo, char *width, char *height, char *header, char *bgimage)
{
  char rules[255] = "<li>Composi&ccedil;&atilde;o:";
  char wrules[255];
  int  frules=0;

  printf("Content-type: text/html\n");
  puts("\n");
  printf("<html>\n");
  printf("<head>\n");
  printf("  <title>Altera&ccedil;&atilde;o de senha do Squid</title>\n");
  printf("</head>\n");
  printf("<body bgcolor=%s text=%s background='%s'>\n",bgcolor,txcolor,bgimage);

  if(strlen(logo) > 0)
     printf("<h2><img src='%s' border=0 align=absmiddle width=%s height=%s><font color=%s>%s</font></h2>\n",logo,width,height,ticolor,header);
   else
     printf("<h2><font color=%s>%s</font></h2>\n",ticolor,header);

  printf("Por medida de seguran&ccedil;a, lembre-se das regras abaixo para escolher sua senha:\n");
  printf("<ul>\n");
  printf("<li>A senha pode conter letras mai&uacute;sculas e/ou min&uacute;sculas, n&uacute;meros ou qualquer caracter do teclado. <br>\n");
  printf("<li>Regras para a nova senha:<br>");
  printf("<ul><li>Comprimento: m&iacute;nimo <font color=%s><b>%s</b></font>, m&aacute;ximo <font color=%s><b>%s</b></font> caracteres.<br>\n",rucolor,minlen,rucolor,maxlen);

  if(strcmp(minchar,"0") != 0) {
     sprintf(wrules," letras (a-z, A-Z):<font color=%s><b> %s</b></font>",rucolor,minchar);
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
     sprintf(wrules," caracteres especiaus (teclado):<font color=%s><b> %s</b></font>",rucolor,minspec);
     strcat(rules,wrules);
     frules=1;
  }
  if(!frules){
     sprintf(wrules,"<font color=%s><b> livre escolha.</b></font>",rucolor);
     strcat(rules,wrules);
  }
  strcat(rules,"<br>");
  printf(rules);
  printf("</ul>\n");

  printf("<li>Sua nova senha ser&aacute; efetivada em torno de 15 minutos.\n");
  printf("</ul>\n");
  printf("<form action='chpasswd.cgi' method=POST>\n");
  printf("<center>\n");
  printf("<hr noshade>\n");
  printf("<table cellpadding=0 cellspacing=0>\n");
  printf("  <tr><td align=right>Usu&aacute;rio: \n");
  printf("      <td><input type=text name=user size=10>\n");
  printf("  <tr><td align=right>Senha atual:\n");
  printf("      <td><input type=password name=old_pw size=10>\n");
  printf("  <tr><td align=right>Nova senha:\n");
  printf("      <td><input type=password name=new_pw1 size=10>\n");
  printf("  <tr><td align=right>Reentre a nova senha:\n");
  printf("      <td><input type=password name=new_pw2 size=10>\n");
  printf("</table>\n");
  printf("<hr noshade>\n");
  printf("<input type=submit name=change value='Altere minha senha'>\n");
  printf("<input type=reset name=reset value='Limpa os campos'>\n");
  printf("<input type=submit name=cancel value='Cancele'>\n");
  printf("</center>\n");
  printf("</form>\n");
  printf("<a href='%s'>%s-%s</a>\n",URL,PGM,VERSION);
  printf("</body>\n");
  printf("</html>\n");

  fflush(stdout);
  return;

}
#endif
