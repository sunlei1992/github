#!/bin/sh
javac -encoding "gb2312" -classpath ./src/log4j-1.2.17.jar:./src/dom4j.jar:./src/mysql-connector-java-5.1.27-bin.jar:./src/json.jar ./src/GDPayService/GDPayService.java ./src/GDPayService/Dbmanage.java ./src/GDPayService/SignatureUtil.java ./src/GDPayService/Configure.java
mv ./src/GDPayService/*.class ./bin/GDPayService
