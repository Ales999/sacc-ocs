<?php
//
//    SQUID Acconting                                            [SAcc system] 
//    Copyright (C) 2003  the.Virus
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

// russian messages config / конфиг русского языка
$admin_title="Администрирование доступа пользователей в Интернет";
$admin_auth_realm="SAcc administration";
$try_again="Попробовать снова";
$user_blank="Поле \"Пользователь\" не должно быть пустым.";
$login_blank="Поле \"login\" должно быть заполнено";
$user_exist="Такой пользователь уже существует.";
$passwd_noteq="Пароли несовпадают.";
$add_user_fail="Внимание! Ошибка: изменения не внесены! Очень похоже что нет разрешений на INSERT!";
$back_tomain="Вернуться на главную";
$crt_newuser="Создать нового пользователя";
$clear_form="Восстановить форму";
$new_user_creation_header="Создание нового пользователя";
$fill_columns="Заполните эти поля";
$like_that="Например так";
$descr_like="Иванов И. И. (программист)";
$login_like="ivanov_i_i";
$passwd_like_1="пароль (отобразятся звёздочки)";
$passwd_like_2="повтор пароля для верности (снова отобразятся звёздочки)";
$limit_like="Обычно ноль ;))";
$time_acl_unlim="неограничено";
$time_acl_1700="с 17:00 до 09:00";
$time_acl_1900="с 19:00 до 09:00";
$time_acl_2000="с 20:00 до 09:00";
$time_acl_2100="с 21:00 до 09:00";
$time_acl_icq="только ICQ";
$edit_user="Редактирование пользователя";
$edit_save="Сохранить";
$edit_delete="Удалить пользователя";
$view_user="Пользователь";
$view_login="Логин (ник):";
$view_traf="Трафик (байт)";
$view_quota="Лимит (байт)";
$place_money="Положить на счёт. (руб)";
$view_email="Эл. почта";
$work_time="Время работы";
$web_admin_descr="Фамилия И. О. (отдел)";
$web_admin_traf="Трафик,<BR>байт";
$web_admin_quota="Лимит,<BR>байт";
$web_admin_tacl="Время доступа";
$web_admin_login="Логин";
$web_admin_mailc="Почта,<br>байт";
$web_status="Статус";
$web_status_negative="Отключен";
$web_admin_total="Итого на ";
$web_admin_logfree_space="Места на разделе для логов";
$web_admin_header_influences="Доступные административные воздействия";
$web_admin_header_adduser="Добавление пользователя";
// client side (user level)
$web_client_welcome="Вас приветствует модуль отображения трафика пользователя SAcc v.";
$web_client_heap_unauth="SAcc отображает:<BR>
 &#151;&nbsp;лимит пользователя (в байтах);<BR>
 &#151;&nbsp;израсходованное и оставшееся в распоряжении пользователя количество байт;<BR>
 &#151;&nbsp;посещенные сайты и число байт, скачанное с каждого из них.<BR><BR>
Кроме того, SAcc может представлять данные в отсортированном по размеру или имени 
сайта виде (в прямом и обратном порядке).
</P><HR NOSHADE COLOR=#000000 SIZE=1><P>
После успешного 
прохождения авторизации (вам будет предложено ввести свой логин и пароль) SAcc
отобразит интересующую вас информацию.
</P>";
$web_client_header="Статистика по трафику";
$web_client_auth_realm="яРЮРХЯРХЙЮ ХМРЕПМЕР ДНЯРСОЮ, бБЕДХРЕ ЯБНИ КНЦХМ Х ОЮПНКЭ"; // must be cp1251
$web_client_your_stat="Ваша статистика по трафику";
$web_client_auth_wrong="Извините, вы неверно ввели логин и/или пароль. Попробуйте войти еще раз.";
$web_client_nouser="Вы не зарегестрированы как пользователь прокси сервера<br> Поэтому доступ к серверу вам запрещён. <bR> Если вы считаете это неправильным, обратитесь к администратору ";
$web_client_mbcost="Текущая цена мегабайта (т.е. 1048576 байт) составляет";
$web_client_account_total="Итого на счету";
$web_client_unlim="без ограничений";
$word_byte="байт";
$word_warning="Внимание";
$word_time="Время";
$web_client_used="Израсходовано";
$web_client_in_mail_trf="входящий почтовый трафик";
$web_client_order_desc="Отсортировано в обратном порядке";
$web_client_order_asc="Отсортировано в прямом порядке";
$web_client_downloaded="Скачано байт";
$web_client_site="Сайт";
$word_total="Итого";
$web_client_downloaded_from="скачано с";
$web_client_sites_wrd="сайтов";
$web_client_inmail="Входящая почта";
$web_client_mailfrom="От кого";
$web_client_noaccess="Доступ запрещен";
$web_client_query="Запрос";
$web_client_src_ip="Источник";
$web_client_server_rply="Ответ";
$web_client_downloaded_with="скачано с помощью";
$web_client_query="запросов";
$web_client_enry_user="Статистика доступа в интернет";
$web_client_entry_admin="для администраторов";
$sa_title="Управление системой SAcc";
$sa_auth_realm="SAcc administration";
$new_sa_creation_header="Создание нового администратора";
$web_admin_header_managesa="Управление учётными записями Администраторов";
$web_admin_header_viewlog="Просмотр журнала событий";
$Reset_password="Сбросить пароль";
$web_admin_perf="Производительность и эффективность";
$web_admin_header_options="Настройки системы";
$web_client_futime="первое обращение";
$web_client_lutime="последнее обращение";
$web_admin_header_viewrsh="Просмотр статистики Cisco IP accounting";
$web_admin_daystat="Статистика по дням";
$web_admin_queue="Управление очередью событий";
$web_admin_history="Срезы за предыдущие периоды";
$web_admin_acl="Списки контроля доступа";
$web_admin_name="Имя";
$web_admin_hit_percent="Процент обращений попадания в кэш";
$web_admin_bytes_percent="Процент в байтах";
$web_admin_count="Количество";
$web_admin_total="Всего";
$web_admin_number="Номер";
$web_admin_date="Дата";
$web_admin_description="Описание";





?>