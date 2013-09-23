/* Russian language added by Andrew <andrew@extrim.ru> */

#if defined(Russian)

char *msg01 = "Нет прав для записи в файл паролей:";
char *msg02 = "Новый пароль не задан или менее";
char *msg03 = "символов.";
char *msg04 = "Новый пароль более";
char *msg05 = "Пожалуйста, не используйте тот же самый пароль. Пароль не изменен.";
char *msg06 = "Новый пароль и подтверждение не совпадают.";
char *msg07 = "Невозможно открыть файл паролей:";
char *msg08 = "Невозможно открыть временный файл.";
char *msg09 = "Неверный пароль пользователя:";
char *msg10 = "Пароль не был изменен.";
char *msg11 = "Успешно изменен пароль пользователя";
char *msg12 = "";
char *msg13 = "Пользователь:";
char *msg14 = "не найден.";
char *msg15 = "Невозможно открыть файл конфигурации:";
char *msg16 = "Невозможно открыть log-файл:";
char *msg17 = "Неверный новый пароль.";
char *msg18 = "Пожалуйста, пользуйтесь следующими правилами для изменения пароля:";
char *msg19 = "Минимальное количество букв (a-z, A-Z)";
char *msg20 = "Минимальное количество цифр (0-9)";
char *msg21 = "Минимальное количество спецсимволов (с клавиатуры)";
char *msg22 = "Изменение пароля учетной записи прокси сервера Squid";
char *msg23 = "Владельцем файла авторизации должет быть root. Пожалуйста, читайте README.";
char *msg24 = "Маска доступа к файлу авторизации должна быть 755. Пожалуйста, читайте README.";
char *msg25 = "Невозможно открыть файл авторизации.";
char *msg26 = "У Вас нет прав для изменения пароля.";
char *msg27 = "Файл авторизации не найден.";

static void UserForm(char *minlen, char *maxlen, char *minchar, char *minnum, char *minspec, char *bgcolor, char *txcolor, char *ticolor, char *rucolor, char *logo, char *width, char *height, char *header, char *bgimage)
{

  char rules[255] = "<li>Ограничение на символы:";
  char wrules[255];
  int  frules=0;

  printf("Content-type: text/html\n");
  puts("\n");
  printf("<html>\n");
  printf("<head>\n");
  printf("<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=utf-8\">");
  printf("  <title>Изменение пароля доступа к Squid</title>\n");
  printf("</head>\n");
  printf("<body bgcolor=%s text=%s background='%s'>\n",bgcolor,txcolor,bgimage);

  if(strlen(logo) > 0)
     printf("<h2><img src='%s' border=0 align=absmiddle width=%s height=%s><font color=%s>%s</font></h2>\n",logo,width,height,ticolor,header);
   else
     printf("<h2><font color=%s>%s</font></h2>\n",ticolor,header);

  printf("Из соображений безопасности, запомните следующие правила изменения пароля:\n");
  printf("<ul>\n");
  printf("<li>При вводе нового пароля различаются маленькие и большие буквы, т.е. 'A' не то же самое, что 'a'.<br>\n");
  printf("<li>Вы можете использовать буквы, цифры и другие специальные символы на вашей клавиатуре.<br>\n");
  printf("<li>Правила для нового пароля:<br>");
  printf("<ul><li>Длина: минимум <font color=%s><b>%s</b></font>, максимум <font color=%s><b>%s</b></font> символов.<br>\n",rucolor,minlen,rucolor,maxlen);

  if(strcmp(minchar,"0") != 0) {
     sprintf(wrules," буквы (a-z, A-Z):<font color=%s><b> %s</b></font>",rucolor,minchar);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minnum,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," цифры (0-9):<font color=%s><b> %s</b></font>",rucolor,minnum);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minspec,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," специальные символы (с клавиатуры):<font color=%s><b> %s</b></font>",rucolor,minspec);
     strcat(rules,wrules);
     frules=1;
  }
  if(!frules){
     sprintf(wrules,"<font color=%s><b> свободный выбор</b></font>",rucolor);
     strcat(rules,wrules);
  }
  strcat(rules,"<br>");
  printf(rules);
  printf("</ul>\n");

  printf("<li>Ваш новый пароль вступит в силу не позднее, чем через 15 минут.\n");
  printf("</ul>\n");
  printf("<form action='chpasswd.cgi' method=POST>\n");
  printf("<center>\n");
  printf("<hr noshade>\n");
  printf("<table cellpadding=0 cellspacing=0>\n");
  printf("  <tr><td align=right>Имя Вашей учетной записи\n");
  printf("      <td><input type=text name=user size=10>\n");
  printf("  <tr><td align=right>Ваш текуший пароль:\n");
  printf("      <td><input type=password name=old_pw size=%s>\n",maxlen);
  printf("  <tr><td align=right>Новый пароль:\n");
  printf("      <td><input type=password name=new_pw1 size=%s>\n",maxlen);
  printf("  <tr><td align=right>Подтвердите новый пароль:\n");
  printf("      <td><input type=password name=new_pw2 size=%s>\n",maxlen);
  printf("</table>\n");
  printf("<hr noshade>\n");
  printf("<input type=submit name=change value='Изменить мой пароль'>\n");
  printf("<input type=reset name=reset value='Очистить все поля'>\n");
  printf("<input type=submit name=cancel value='Отмена'>\n");
  printf("</center>\n");
  printf("</form>\n");
  printf("<a href='%s'>%s-%s</a>\n",URL,PGM,VERSION);
  printf("</body>\n");
  printf("</html>\n");

  fflush(stdout);
  return;

}
#endif
