#!/bin/sh
/usr/bin/apxs -l mysqlclient -n whitelist_mysql -i -a whitelist_mysql.so
#/usr/bin/apxs -l mysqlclient -n blacklist_mysql -i -a blacklist_mysql.so
