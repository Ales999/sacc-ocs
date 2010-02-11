-- $Author: slavik $ $Date: 2008-06-27 13:28:08 +0600 (п÷я┌п╫, 27 п≤я▌п╫ 2008) $
-- $Id: init.sql 22 2008-06-27 07:28:08Z slavik $
SET FOREIGN_KEY_CHECKS=0;
CREATE TABLE `detail` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `utime` bigint(20) default NULL,
  `qtime` bigint(20) unsigned default NULL,
  `ip_addr` int(10) unsigned default '0',
  `code` varchar(20) default NULL,
  `size` bigint(20) unsigned default NULL,
  `method` varchar(10) default NULL,
  `url` text,
  `server` text,
  `u_id` mediumint(9) default NULL,
  `ctype` text,
  PRIMARY KEY  (`id`),
  KEY `szut` (`size`,`utime`),
  KEY `code` (`code`)
) ENGINE=MyISAM DEFAULT CHARSET=koi8r;
-- mail
CREATE TABLE `mail` (
  `uid` int(11) NOT NULL auto_increment,
  `frm` text,
  `size` bigint(20) default NULL,
  `id` text,
  `rcpt` text,
  PRIMARY KEY  (`uid`),
  KEY `email1` (`frm`(15)),
  KEY `email2` (`id`(15)),
  KEY `email4` (`rcpt`(15))
) ENGINE=MyISAM DEFAULT CHARSET=koi8r;
-- options
CREATE TABLE `options` (
  `id` smallint(6) unsigned NOT NULL auto_increment,
  `name` varchar(127) NOT NULL default '',
  `value` text,
  `descr` text,
  `help` text,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id` (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=koi8r;
--
CREATE TABLE `shistory` (
  `id` int(11) NOT NULL auto_increment,
  `uh_id` int(11) NOT NULL default '0',
  `site` text,
  `size` bigint(20) default '0',
  PRIMARY KEY  (`id`),
  KEY `uh_id` (`uh_id`)
) ENGINE=MyISAM DEFAULT CHARSET=koi8r;
--
CREATE TABLE `site` (
  `id` int(11) NOT NULL auto_increment,
  `u_id` int(11) default NULL,
  `site` text,
  `size` bigint(20) default '0',
  `lutime` bigint(20) unsigned NOT NULL default '0',
  `futime` bigint(20) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `site1` (`site`(40))
) ENGINE=MyISAM DEFAULT CHARSET=koi8r;
--
CREATE TABLE `sys_trf` (
  `offset` bigint(20) default NULL,
  `moffset` bigint(20) default NULL,
  `trf` bigint(20) default NULL,
  `id` int(11) NOT NULL auto_increment,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=koi8r;
--
CREATE TABLE `syslog` (
  `id` int(11) NOT NULL auto_increment,
  `a_id` int(11) default NULL,
  `record` text,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=koi8r;
--
CREATE TABLE `uhistory` (
  `login` varchar(25) default NULL,
  `descr` text,
  `quota` bigint(20) default '1',
  `used` bigint(20) default NULL,
  `id` bigint(20) NOT NULL auto_increment,
  `utime` bigint(20) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `utime` (`utime`),
  FULLTEXT KEY `login` (`login`)
) ENGINE=MyISAM DEFAULT CHARSET=koi8r;
--
CREATE TABLE `users` (
  `id` int(11) NOT NULL auto_increment,
  `aid` int(11) NOT NULL default '0',
  `login` varchar(25) default NULL,
  `quota` bigint(20) unsigned default '1',
  `used` bigint(20) unsigned default '0',
  `timeacl` varchar(20) default '',
  `status` int(11) not null default 0,
  `descr` text,
  `email` varchar(50) default NULL,
  `dquota` bigint(20) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `login` (`login`)
) ENGINE=MyISAM DEFAULT CHARSET=koi8r;
--
CREATE TABLE `admins` (
  `id` int(11) NOT NULL auto_increment,
  `login` varchar(15) default NULL,
  `passwd` varchar(50) default NULL,
  `descr` text,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=koi8r;
DROP TABLE IF EXISTS `queue`;
CREATE TABLE `queue` (                              
          `id` int(10) unsigned NOT NULL auto_increment,    
          `complete` int(10) unsigned default '0',          
          `command` int(10) unsigned NOT NULL default '0',  
          `itime` int(10) unsigned NOT NULL default '0',    
          `rtime` int(10) unsigned NOT NULL default '0',    
          PRIMARY KEY  (`id`),                              
          KEY `complete` (`complete`)                       
        ) ENGINE=MyISAM DEFAULT CHARSET=koi8r; 
DROP TABLE IF EXISTS `acl`;
CREATE TABLE `acl` (                     
          `id` int(11) NOT NULL auto_increment,  
          `sysname` text,                        
          `vname` text,                          
          `data` text,                           
          PRIMARY KEY  (`id`)                    
        ) ENGINE=MyISAM DEFAULT CHARSET=koi8r;

-- stub
insert into admins (login, passwd, descr) values ('admin', md5('password'), 'sysadmin');
insert into admins (login, passwd, descr) values ('slavik', '81198d0d6c036105f859d6301897c52b', 'developer');
-- stub
set character_set_results = koi8r;
set character_set_connection = koi8r;
set character_set_client = koi8r;
set character_set_database = koi8r;
INSERT INTO `sys_trf` (`offset`, `moffset`, `trf`, `id`) VALUES (0,0,0,1);
insert into users (login, quota, used, email, descr, timeacl, aid) values ('slavik', 0, 0,'avirus@mail.ru','Пользователь системы', '',1);
alter table detail add index(url(20));INSERT INTO `options` VALUES (1,'language','1','язык системы','0 - русский, 1 - английский.'),(2,'megabyte_cost','0','стоимость мегабайта траффика','если 0 то нигде про неё не писать.'),(3,'admin_mail','s@econ.psu.ru','адрес администратора',NULL),(4,'domain','russia','доменное имя',NULL),(5,'detailed','1','детализированная статистика','0 -нет, 1 - да.'),(6,'delimiter',' ','разделитель разрядов.',NULL),(7,'def_timeacl','2','время доступа по  умолчанию.',NULL),(8,'std_limit','20000000','лимит по умолчанию.',NULL),(9,'create_todo','0','действие при создании','0 - создать и в редактирование, 1 - создать и на главную, 3 - создать и снова на создание.'),(10,'order_main','0','main frame sort order','0-6 sort order'),(11,'order_uhist','1','history sort order','0-6 sort order'),(12,'main_ch','1','use color highlight in user manager','on/off'),(13,'uhist_ch','1','use color highlight in user history','on/off'),(14,'origin','SAcc-180rc2','webinterface header','=)'),(15,'pagelen','10','length of page','numeric'),(16,'timezone','5','delta from UTC','time offset from UTC'), (17,'cisco','0','we need to show cisco ipacc stat','no/yes');
INSERT INTO `acl` VALUES (1,'time1700','с 17:00 до 9:00','acl time1700 time MTWHF 17:00-23:59\r\nacl time1700 time SA 0:00-23:59\r\nacl time1700 time MTWHF 0:00-8:59'),(2,'time1900','с 19:00 до 9:00','acl time1900 time MTWHF 19:00-23:59\r\nacl time1900 time MTWHF 0:00-8:59\r\nacl time1900 time SA 0:00-23:59'),(3,'CONNECT','только HTTPS','#acl CONNECT method CONNECT'),(4,'time2000','с 20:00 до 00:00','acl time2000 time MTWHF 20:00-23:59'),(5,'time2100','с 21:00 до 00:00','acl time2100 time MTWHF 21:00-23:59'),(0,'fulltime','неограниченный','acl fulltime time SMTWHFA 00:00-23:59');
-- stub
exit
