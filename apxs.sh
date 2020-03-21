#!/bin/sh
/usr/bin/apxs -l mysqlclient -n mod_whitelist_mysql -i -a -c  src/mod_whitelist_mysql.c src/CConfigUtil.cpp  src/CGetIpAddressList.cpp src/CConfigUtil.h  src/CGetIpAddressList.h
