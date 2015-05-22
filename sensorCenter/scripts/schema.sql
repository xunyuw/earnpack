DROP TABLE IF EXISTS `entries`;
CREATE TABLE IF NOT EXISTS `entries` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Serial number',
  `title` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '' COMMENT '',
  `text` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '' COMMENT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Saves entries';


CREATE TABLE IF NOT EXISTS `users` (
    `UserID` char(16) NOT NULL PRIMARY KEY,
    `pwd` varchar(32),
    `name` varchar(32) CHARACTER SET utf8, 
    `sms` varchar(256) CHARACTER SET utf8 DEFAULT NULL 
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='user info';

CREATE TABLE IF NOT EXISTS `sensors` (
    `TagID`  char(16) NOT NULL PRIMARY KEY,
    `UserID` char(16)
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='sensor own info';

CREATE TABLE IF NOT EXISTS `sensorDatum` (
    `TagID` varchar(16) NOT NULL,
    `RSSI`  varchar(16) DEFAULT '', 
    `vList` varchar(16) DEFAULT '', 
    `Bat`  smallint DEFAULT '0', 
    `oV`   smallint DEFAULT '0', 
    `tV`   smallint DEFAULT '0', 
    `hV`   smallint DEFAULT '0', 
    `pV`   smallint DEFAULT '0', 
    `fV`   smallint DEFAULT '0', 
    `rT`   TIMESTAMP not null
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='user info';
