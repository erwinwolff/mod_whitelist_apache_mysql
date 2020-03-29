## Why Mod_Whitelist?
Because sometimes you need simple IP based filtering. It is manageable from a MySQL table, it is up to you to fill the MySQL table. Its components are:

* CMake (for development only)
* [Apache 2.4](https://httpd.apache.org/)
* Either [MySQL](https://www.mysql.com/) or [MariaDB](https://mariadb.com/)
* mod_whitelist

## How do I configure it?
Configuration is very simple.

Inside a vhost you just set these parameters:
* WhiteListMySqlEnabled On
* WhiteListMySqlConnectrionstring "Server=localhost;Database=Testdb;User=test;Password=test;Port=3306"
* WhiteListMySqlQuery "SELECT Ip FROM allowedips"

## How do I compile it?
Building from source is very easy (on debian-clones):

* cmake .
* make
* ./apxs.sh 
* service apache2 restart
