-- $Id: monthly.sql 4 2006-03-26 20:20:41Z slavik $
-- add users history records
insert into uhistory (login, descr, quota, used, utime) select login, descr, quota, used, UNIX_TIMESTAMP() as utime from users;
-- add sites history records
insert into shistory (site, size, uh_id) select s.site, s.size, (select max(uh.id) from uhistory AS uh where uh.login = (select login from users where id=s.u_id)) as uh_id from site as s;
-- cleanup
update users set used=0;
truncate table site;
truncate table mail;
truncate table detail;
update users set quota=dquota;
update sys_trf set offset=0;
update users set status=0 where status=2;
delete from users where status=3;
