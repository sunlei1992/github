create database test_gdbank_data;

use test_gdbank_data;

CREATE TABLE gdbank_log
(
jybs varchar(30) ,

jym varchar(18),

jymc varchar(30),

khbs varchar(50),

khfzxx varchar(20),

khmc varchar(74),

jyrq date,

jysj time,

jyje int,

jycgbs int,


jycgbsxq varchar(10),

yjylsh varchar(30),

jyzt int,


jyztxq varchar(10),

fhxx varchar(40),

qybm varchar(30),

qymc varchar(40),

jyqdbm varchar(22),

jyqdmc varchar(40),

jyjg varchar(18),

jyjgmc varchar(40),

czyh varchar(18),

sfdwlbbm varchar(22),

sfdwlbmc varchar(40),

sfdwyjjgbm varchar(20),

sfdwyjjgmc varchar(40),

sfdwejjgbm varchar(20),

sfdwejjgmc varchar(40),

scbjw int,


scbjwxq varchar(10),


xgbjw int,


xgbjwxq varchar(40),


dzjg int,


dzjgxq varchar(40),


beizhu varchar(100),

baoliu1 varchar(40),

baoliu2 varchar(40),

baoliu3 varchar(40),



primary key (jybs)
);



CREATE TABLE gdbank_log_delete_backup
(
jybs varchar(30) ,

jym varchar(18),

jymc varchar(30),

khbs varchar(50),

khfzxx varchar(20),

khmc varchar(74),

jyrq date,

jysj time,

jyje int,

jycgbs int,

jycgbsxq varchar(10),

yjylsh varchar(30),

jyzt int,

jyztxq varchar(10),

fhxx varchar(40),

qybm varchar(30),

qymc varchar(40),

jyqdbm varchar(22),

jyqdmc varchar(40),

jyjg varchar(18),

jyjgmc varchar(40),

czyh varchar(18),

sfdwlbbm varchar(22),

sfdwlbmc varchar(40),

sfdwyjjgbm varchar(20),

sfdwyjjgmc varchar(40),

sfdwejjgbm varchar(20),

sfdwejjgmc varchar(40),

scbjw int,

scbjwxq varchar(10),

xgbjw int,

xgbjwxq varchar(40),

dzjg int,

dzjgxq varchar(40),

beizhu varchar(100),

baoliu1 varchar(40),

baoliu2 varchar(40),

baoliu3 varchar(40),

scsj datetime,

primary key (jybs,scsj)
);



CREATE TABLE gdbank_log_change_backup
(
jybs varchar(30) ,

jym varchar(18),

jymc varchar(30),

khbs varchar(50),

khfzxx varchar(20),

khmc varchar(74),

jyrq date,

jysj time,

jyje int,

jycgbs int,

jycgbsxq varchar(10),

yjylsh varchar(30),

jyzt int,

jyztxq varchar(10),

fhxx varchar(40),

qybm varchar(30),

qymc varchar(40),

jyqdbm varchar(22),

jyqdmc varchar(40),

jyjg varchar(18),

jyjgmc varchar(40),

czyh varchar(18),

sfdwlbbm varchar(22),

sfdwlbmc varchar(40),

sfdwyjjgbm varchar(20),

sfdwyjjgmc varchar(40),

sfdwejjgbm varchar(20),

sfdwejjgmc varchar(40),

scbjw int,

scbjwxq varchar(10),

xgbjw int,

xgbjwxq varchar(40),

dzjg int,

dzjgxq varchar(40),

beizhu varchar(100),

baoliu1 varchar(40),

baoliu2 varchar(40),

baoliu3 varchar(40),

xgsj datetime,

primary key (jybs,xgsj)
);


CREATE TABLE gdbank_user
(
username varchar(30) ,

password varchar(30),

userauthority varchar(1),

userauthoritychinese varchar(20),
primary key (username)
);






CREATE TABLE gdbank_statement
(
jybs varchar(30) ,

jym varchar(18),

jymc varchar(30),

khbs varchar(50),

khfzxx varchar(20),

khmc varchar(74),

jyrq date,

jysj time,

jyje int,

jycgbs int,

jycgbsxq varchar(10),

yjylsh varchar(30),

jyzt int,

jyztxq varchar(10),

fhxx varchar(40),

qybm varchar(30),

qymc varchar(40),

jyqdbm varchar(22),

jyqdmc varchar(40),

jyjg varchar(18),

jyjgmc varchar(40),

czyh varchar(18),

sfdwlbbm varchar(22),

sfdwlbmc varchar(40),

sfdwyjjgbm varchar(20),

sfdwyjjgmc varchar(40),

sfdwejjgbm varchar(20),

sfdwejjgmc varchar(40),

dzwjm varchar(100),
sdrq date,

dzjg int,

dzjgxq varchar(40),

beizhu varchar(100),

baoliu1 varchar(40),

baoliu2 varchar(40),

baoliu3 varchar(40),


primary key (jybs,dzwjm,sdrq)
);


CREATE TABLE gdbank_statement_history
(
jybs varchar(30) ,

jym varchar(18),

jymc varchar(30),

khbs varchar(50),

khfzxx varchar(20),

khmc varchar(74),

jyrq date,

jysj time,

jyje int,

jycgbs int,

jycgbsxq varchar(10),

yjylsh varchar(30),

jyzt int,

jyztxq varchar(10),

fhxx varchar(40),

qybm varchar(30),

qymc varchar(40),

jyqdbm varchar(22),

jyqdmc varchar(40),

jyjg varchar(18),

jyjgmc varchar(40),

czyh varchar(18),

sfdwlbbm varchar(22),

sfdwlbmc varchar(40),

sfdwyjjgbm varchar(20),

sfdwyjjgmc varchar(40),

sfdwejjgbm varchar(20),

sfdwejjgmc varchar(40),

dzwjm varchar(100),
sdrq date,

dzjg int,

dzjgxq varchar(40),

beizhu varchar(100),

baoliu1 varchar(40),

baoliu2 varchar(40),

baoliu3 varchar(40),

scsj datetime,

primary key (jybs,dzwjm,sdrq,scsj)
);
