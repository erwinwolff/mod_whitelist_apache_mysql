cmake_minimum_required(VERSION 3.10)

project(mod_whitelist VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_definitions(-DLINUX -D_REENTRANT -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -Wno-undef)
include_directories(SYSTEM /usr/include/apache2 /usr/include/apr-1.0 /usr/include)

include(FindPkgConfig)
pkg_check_modules(LIBMYSQLCLIENT REQUIRED mysqlclient)

link_libraries(mysqlclient)

add_library(whitelist_mysql SHARED
src/CConfigUtil.cpp
src/CGetIpAddressList.cpp
src/get_ip_list.cpp
src/mod_whitelist_mysql.c
)

