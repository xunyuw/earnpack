g++ -g -rdynamic ezMysql.cpp  -std=c++11 -lev -lmysqlclient -Wall -DENABLE_LOGGER -L/usr/local/lib/mysql/ -L/usr/lib/mysql/ -I/usr/include/mysql  -o ./test
