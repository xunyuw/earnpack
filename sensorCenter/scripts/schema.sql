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

CREATE TABLE IF NOT EXISTS `latestRecord` (
    `TagID` varchar(16) NOT NULL PRIMARY KEY,
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

CREATE TABLE IF NOT EXISTS `sensorDatum0` (
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

CREATE TABLE IF NOT EXISTS `sensorDatum1` (
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

CREATE TABLE IF NOT EXISTS `sensorDatum2` (
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

CREATE TABLE IF NOT EXISTS `sensorDatum3` (
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

CREATE TABLE IF NOT EXISTS `sensorDatum4` (
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

CREATE TABLE IF NOT EXISTS `sensorDatum5` (
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

CREATE TABLE IF NOT EXISTS `sensorDatum6` (
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

CREATE TABLE IF NOT EXISTS `sensorDatum7` (
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

CREATE TABLE IF NOT EXISTS `sensorDatum8` (
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

CREATE TABLE IF NOT EXISTS `sensorDatum9` (
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

CREATE TABLE IF NOT EXISTS `sensorDatumA` (
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

CREATE TABLE IF NOT EXISTS `sensorDatumB` (
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

CREATE TABLE IF NOT EXISTS `sensorDatumC` (
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

CREATE TABLE IF NOT EXISTS `sensorDatumD` (
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

CREATE TABLE IF NOT EXISTS `sensorDatumE` (
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

CREATE TABLE IF NOT EXISTS `sensorDatumF` (
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

CREATE TABLE IF NOT EXISTS `irInfo` (
    `UserID` char(16),
    `TagID`  char(16),
    `KeyID`  tinyint unsigned DEFAULT '0', 
    `SeqNo`  smallint unsigned DEFAULT '0', 
    `pktNum` tinyint unsigned DEFAULT '0',
    `pktIdx` tinyint unsigned DEFAULT '0',
    `data`   tinyblob
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='ir info';    
