-- phpMyAdmin SQL Dump
-- version 3.2.5deb2
-- http://www.phpmyadmin.net
--
-- Хост: localhost
-- Время создания: Мар 04 2010 г., 11:19
-- Версия сервера: 5.1.41
-- Версия PHP: 5.3.1-5

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT=0;
START TRANSACTION;


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- База данных: `sacc`
--

-- --------------------------------------------------------

--
-- Структура таблицы `krbconv`
--

DROP TABLE IF EXISTS `krbconv`;
CREATE TABLE IF NOT EXISTS `krbconv` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'id',
  `krbname` varchar(254) CHARACTER SET ascii NOT NULL COMMENT 'Шаблон в Kerberos нотации',
  `ntlmname` varchar(254) CHARACTER SET ascii NOT NULL COMMENT 'Шаблон в NTLM нотации',
  PRIMARY KEY (`id`),
  UNIQUE KEY `krbname` (`krbname`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COMMENT='Таблица преобразований имен пользователей в acees.log из Ker' AUTO_INCREMENT=3 ;

--
-- MIME-ТИПЫ ТАБЛИЦЫ `krbconv`:
--   `id`
--       `text_plain`
--   `krbname`
--       `text_plain`
--   `ntlmname`
--       `text_plain`
--

--
-- Дамп данных таблицы `krbconv`
--

INSERT INTO `krbconv` (`id`, `krbname`, `ntlmname`) VALUES
(1, '@krbd.domen.ru', 'krbd'),
(2, '@other.local', 'otherdom');
COMMIT;