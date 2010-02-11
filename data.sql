-- MySQL dump 10.10
--$Author: slavik $ $Date: 2006-03-27 23:51:44 +0600 (Пнд, 27 Мар 2006) $
--$Id: data.sql 6 2006-03-27 17:51:44Z slavik $
-- Host: localhost    Database: sacc
-- ------------------------------------------------------
-- Server version	5.0.15

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES koi8r */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `options`
--

DROP TABLE IF EXISTS `options`;
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
-- Dumping data for table `options`
--


/*!40000 ALTER TABLE `options` DISABLE KEYS */;
LOCK TABLES `options` WRITE;
INSERT INTO `options` VALUES (1,'language','1','���� �������','0 - �������, 1 - ����������.'),(2,'megabyte_cost','0','��������� ��������� ��������','���� 0 �� ����� ��� �ţ �� ������.'),(3,'admin_mail','s@econ.psu.ru','����� ��������������',NULL),(4,'domain','prognoz','�������� ���',NULL),(5,'detailed','1','���������������� ����������','0 -���, 1 - ��.'),(6,'delimiter',' ','����������� ��������.',NULL),(7,'def_timeacl','2','����� ������� ��  ���������.',NULL),(8,'std_limit','20000000','����� �� ���������.',NULL),(9,'create_todo','0','�������� ��� ��������','0 - ������� � � ��������������, 1 - ������� � �� �������, 3 - ������� � ����� �� ��������.'),(10,'order_main','0','main frame sort order','0-6 sort order'),(11,'order_uhist','1','history sort order','0-6 sort order'),(12,'main_ch','1','use color highlight in user manager','on/off'),(13,'uhist_ch','1','use color highlight in user history','on/off'),(14,'origin','SAcc-180rc1','webinterface header','=)'),(15,'pagelen','10','length of page','numeric'),(16,'timezone','5','delta from UTC','time offset from UTC');
UNLOCK TABLES;
/*!40000 ALTER TABLE `options` ENABLE KEYS */;

--
-- Table structure for table `acl`
--

DROP TABLE IF EXISTS `acl`;
CREATE TABLE `acl` (
  `id` int(11) NOT NULL auto_increment,
  `sysname` text,
  `vname` text,
  `data` text,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=koi8r;

--
-- Dumping data for table `acl`
--


/*!40000 ALTER TABLE `acl` DISABLE KEYS */;
LOCK TABLES `acl` WRITE;
INSERT INTO `acl` VALUES (1,'time1700','� 17:00 �� 9:00','acl time1700 time MTWHF 17:00-23:59\r\nacl time1700 time SA 0:00-23:59\r\nacl time1700 time MTWHF 0:00-8:59'),(2,'time1900','� 19:00 �� 9:00','acl time1900 time MTWHF 19:00-23:59\r\nacl time1900 time MTWHF 0:00-8:59\r\nacl time1900 time SA 0:00-23:59'),(3,'CONNECT','������ HTTPS','#acl CONNECT method CONNECT'),(4,'time2000','� 20:00 �� 00:00','acl time2000 time MTWHF 20:00-23:59'),(5,'time2100','� 21:00 �� 00:00','acl time2100 time MTWHF 21:00-23:59'),(0,'fulltime','��������������','acl fulltime time SMTWHFA 00:00-23:59');
UNLOCK TABLES;
/*!40000 ALTER TABLE `acl` ENABLE KEYS */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

