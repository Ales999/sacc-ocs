/* Russian language added by Andrew <andrew@extrim.ru> */

#if defined(Russian)

char *msg01 = "��� ���� ��� ������ � ���� �������:";
char *msg02 = "����� ������ �� ����� ��� �����";
char *msg03 = "��������.";
char *msg04 = "����� ������ �����";
char *msg05 = "����������, �� ����������� ��� �� ����� ������. ������ �� �������.";
char *msg06 = "����� ������ � ������������� �� ���������.";
char *msg07 = "���������� ������� ���� �������:";
char *msg08 = "���������� ������� ��������� ����.";
char *msg09 = "�������� ������ ������������:";
char *msg10 = "������ �� ��� �������.";
char *msg11 = "������� ������� ������ ������������";
char *msg12 = "";
char *msg13 = "������������:";
char *msg14 = "�� ������.";
char *msg15 = "���������� ������� ���� ������������:";
char *msg16 = "���������� ������� log-����:";
char *msg17 = "�������� ����� ������.";
char *msg18 = "����������, ����������� ���������� ��������� ��� ��������� ������:";
char *msg19 = "����������� ���������� ���� (a-z, A-Z)";
char *msg20 = "����������� ���������� ���� (0-9)";
char *msg21 = "����������� ���������� ������������ (� ����������)";
char *msg22 = "��������� ������ ������� ������ ������ ������� Squid";
char *msg23 = "���������� ����� ����������� ������ ���� root. ����������, ������� README.";
char *msg24 = "����� ������� � ����� ����������� ������ ���� 755. ����������, ������� README.";
char *msg25 = "���������� ������� ���� �����������.";
char *msg26 = "� ��� ��� ���� ��� ��������� ������.";
char *msg27 = "���� ����������� �� ������.";

static void UserForm(char *minlen, char *maxlen, char *minchar, char *minnum, char *minspec, char *bgcolor, char *txcolor, char *ticolor, char *rucolor, char *logo, char *width, char *height, char *header, char *bgimage)
{

  char rules[255] = "<li>����������� �� �������:";
  char wrules[255];
  int  frules=0;

  printf("Content-type: text/html\n");
  puts("\n");
  printf("<html>\n");
  printf("<head>\n");
  printf("<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=koi8-r\">");
  printf("  <title>��������� ������ ������� � Squid</title>\n");
  printf("</head>\n");
  printf("<body bgcolor=%s text=%s background='%s'>\n",bgcolor,txcolor,bgimage);

  if(strlen(logo) > 0)
     printf("<h2><img src='%s' border=0 align=absmiddle width=%s height=%s><font color=%s>%s</font></h2>\n",logo,width,height,ticolor,header);
   else
     printf("<h2><font color=%s>%s</font></h2>\n",ticolor,header);

  printf("�� ����������� ������������, ��������� ��������� ������� ��������� ������:\n");
  printf("<ul>\n");
  printf("<li>��� ����� ������ ������ ����������� ��������� � ������� �����, �.�. 'A' �� �� �� �����, ��� 'a'.<br>\n");
  printf("<li>�� ������ ������������ �����, ����� � ������ ����������� ������� �� ����� ����������.<br>\n");
  printf("<li>������� ��� ������ ������:<br>");
  printf("<ul><li>�����: ������� <font color=%s><b>%s</b></font>, �������� <font color=%s><b>%s</b></font> ��������.<br>\n",rucolor,minlen,rucolor,maxlen);

  if(strcmp(minchar,"0") != 0) {
     sprintf(wrules," ����� (a-z, A-Z):<font color=%s><b> %s</b></font>",rucolor,minchar);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minnum,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," ����� (0-9):<font color=%s><b> %s</b></font>",rucolor,minnum);
     strcat(rules,wrules);
     frules=1;
  }
  if(strcmp(minspec,"0") != 0) {
     if(frules)
        strcat(rules,", ");
     sprintf(wrules," ����������� ������� (� ����������):<font color=%s><b> %s</b></font>",rucolor,minspec);
     strcat(rules,wrules);
     frules=1;
  }
  if(!frules){
     sprintf(wrules,"<font color=%s><b> ��������� �����</b></font>",rucolor);
     strcat(rules,wrules);
  }
  strcat(rules,"<br>");
  printf(rules);
  printf("</ul>\n");

  printf("<li>��� ����� ������ ������� � ���� �� �������, ��� ����� 15 �����.\n");
  printf("</ul>\n");
  printf("<form action='chpasswd.cgi' method=POST>\n");
  printf("<center>\n");
  printf("<hr noshade>\n");
  printf("<table cellpadding=0 cellspacing=0>\n");
  printf("  <tr><td align=right>��� ����� ������� ������\n");
  printf("      <td><input type=text name=user size=10>\n");
  printf("  <tr><td align=right>��� ������� ������:\n");
  printf("      <td><input type=password name=old_pw size=%s>\n",maxlen);
  printf("  <tr><td align=right>����� ������:\n");
  printf("      <td><input type=password name=new_pw1 size=%s>\n",maxlen);
  printf("  <tr><td align=right>����������� ����� ������:\n");
  printf("      <td><input type=password name=new_pw2 size=%s>\n",maxlen);
  printf("</table>\n");
  printf("<hr noshade>\n");
  printf("<input type=submit name=change value='�������� ��� ������'>\n");
  printf("<input type=reset name=reset value='�������� ��� ����'>\n");
  printf("<input type=submit name=cancel value='������'>\n");
  printf("</center>\n");
  printf("</form>\n");
  printf("<a href='%s'>%s-%s</a>\n",URL,PGM,VERSION);
  printf("</body>\n");
  printf("</html>\n");

  fflush(stdout);
  return;

}
#endif
