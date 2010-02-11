-- $Id: recount.sql 4 2006-03-26 20:20:41Z slavik $
update users set used=0;
truncate table site;
truncate table detail;
update sys_trf set offset=0;
