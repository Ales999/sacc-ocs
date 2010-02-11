-- MySQL dump 9.10
--
-- Host: localhost    Database: cisco
-- ------------------------------------------------------
-- Server version	4.0.17

--
-- Table structure for table `traffic`
--

CREATE TABLE traffic (
  id int(10) unsigned NOT NULL auto_increment,
  tm int(10) unsigned default NULL,
  src int(11) unsigned default NULL,
  dst int(11) unsigned default NULL,
  pckts bigint(20) unsigned default '0',
  bytes bigint(20) unsigned default '0',
  PRIMARY KEY  (id),
  UNIQUE KEY id (id),
  KEY tm (tm),
  KEY dst (dst),
  KEY src (src)
) TYPE=MyISAM;

--
-- Dumping data for table `traffic`
--


