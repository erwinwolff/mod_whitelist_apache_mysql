## Why Mod_Whitelist?
Because sometimes you need simple IP based filtering. It is manageable from a MySQL table, it is up to you to fill the MySQL table. Its components are:

* Apache 2.4
* MySQL
* mod_whitelist

## How do I configure it?
Configuration is very simple.

Inside a vhost you just set these parameters:
* SetHandler whitelist_mysql
* WhiteListMySqlEnabled On
* WhiteListMySqlConnectrionstring "Server=localhost;Database=Testdb;User=test;Password=test;Port=3306"
* WhiteListMySqlQuery "SELECT Ip FROM allowedips"

### Why was it created?
Because I needed it ;) 

### Stage of development
Mod_Whitelist is usable on a functional level, but should be considered beta nonetheless. 

### Do you accepted pull-requests?
It depends, if it really adds something. I don't want to have this piece of C++ code solve every problem in the world, so keep pull requests focused towards whitelisting ip-addresses based on a database provided list.
