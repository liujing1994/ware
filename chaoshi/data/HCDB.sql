use master
GO
if(exists(select * from sysdatabases where name='Sales'))
     drop database Sales
create database Sales ON 
(
  NAME ='Sales_dat',
  filename='F:\salesdat.mdf',
  SIZE = 10,
  MAXSIZE = 100,
  FILEGROWTH = 10%
)
LOG ON
( NAME = 'Sales_log',
   FILENAME = 'F:\salelog.ldf',
   SIZE = 5MB,
   MAXSIZE = 25MB,
   FILEGROWTH = 5MB )
GO

use Sales
-----------------------创建tb_operator---------------------------
if(exists(select * from sysobjects where name='tb_operator'))
  drop table tb_operator
create table tb_operator
(
  operid varchar(8) not null,
  opername varchar(8) not null,
  operpassword varchar(8) not null,
  operlevel varchar(10) not null DEFAULT '销售人',
  CONSTRAINT ID_PK PRIMARY KEY (operid)

)

-----------------------创建tb_provider--------------------------
if(exists(select * from sysobjects where name='tb_provider'))
  drop table tb_provider
create table tb_provider
(
  provid varchar(8) not null,
  provname varchar(20) not null,
  provphone char(9) not null,
  provaddr varchar(20) not null,
CONSTRAINT ID_PK1 PRIMARY KEY (provid)
)
-----------------------创建tb_storage--------------------------
if(exists(select * from sysobjects where name='tb_storage'))
  drop table tb_storage
create table tb_storage
(
 storid varchar(20) not null,
 merchandiseid varchar(20) not null unique,
 stordate datetime not null,
 stortype varchar(10) not null,
 provid varchar(8) not null,
 storinfo varchar(30),
 operid varchar(8) not null unique,
 storflag varchar(2) not null,
 unitprice money not null, 
 numbers float not null,
 paymoney money not null,
CONSTRAINT ID_PK2 PRIMARY KEY (storid)
)
-----------------------创建tb_merchangiseinfo--------------------------
if(exists(select * from sysobjects where name='tb_merchangiseinfo'))
  drop table tb_merchangiseinfo
create table tb_merchangiseinfo
(
 id varchar(20) not null,
 mercname varchar(20) not null,
 spec varchar(20) not null,
 mercunit varchar(4) not null,
 inprice money not null,
 outprice money not null,
 provid varchar(8) not null unique,
 discount float not null,
 CONSTRAINT ID_PK3 PRIMARY KEY (id)
)
-----------------------创建tb_merchangisestorage--------------------------
if(exists(select * from sysobjects where name='tb_merchangisestorage'))
  drop table tb_merchangisestorage
create table tb_merchangisestorage
(
 mercid varchar(20) not null,
 storid varchar(20) not null,
 storagenum float not null,
 upperlimit float not null,
 lowerlimit float not null,
CONSTRAINT ID_PK4 PRIMARY KEY (mercid)
)

GO
-----------------------创建tb_sale--------------------------
if(exists(select * from sysobjects where name='tb_sale'))
  drop table tb_sale
create table tb_sale
(
 buyid int not null,
 mercid varchar(20) not null,
 buycount float not null,
 CONSTRAINT ID_PK5 PRIMARY KEY (mercid)
)
