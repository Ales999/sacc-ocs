#!/bin/bash

g++ -g -O0 -Wall -Weffc++ -DUTEST -DDEBUG namepair.cpp krbconv.cpp -I/usr/include/mysql -lnsl -lz -lm -lcrypt -lmysqlclient -okrbconvert-test
#g++ -g -O0 -W -Wall -DUTEST namepair.cpp krbconv.cpp -I/usr/include/mysql -lnsl -lm -lcrypt -lmysqlclient -okrbconvert-test
