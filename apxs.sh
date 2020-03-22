#!/bin/sh
/usr/bin/apxs -l mysqlclient -n whitelist_mysql -i -a -c  src/mod_whitelist_mysql.c src/CConfigUtil.cpp src/CGetIpAddressList.cpp src/CConfigUtil.h src/CGetIpAddressList.h
/usr/bin/apxs -l mysqlclient -n blacklist_mysql -i -a -c  src/mod_blacklist_mysql.c src/CConfigUtil.cpp src/CGetIpAddressList.cpp src/CConfigUtil.h src/CGetIpAddressList.h
