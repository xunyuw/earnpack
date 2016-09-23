##

DIR_ROOT = $(shell pwd)
DIR_OUT = $(DIR_ROOT)/output
APP = $(DIR_OUT)/thingLink
SUBDIRS = util model dao net mqtt service
SUBDIRS += devices
SUBDIRS += output

CXX = g++
CXXFLAGS = -Wall -O2 -std=c++11 -DTRACE
LDFLAGS = -L${DIR_ROOT}/lib -lpaho-mqtt3a -lpthread -lreactcpp -lev -Wl,--no-as-needed


export DIR_ROOT DIR_OUT CXX CXXFLAGS LDFLAGS APP

.PHONY:all clean
#${APP}: all
#	$(CXX) -std=c++11 $(OBJ) -o $@ $(LDFLAGS)

all:
	for I in ${SUBDIRS}; do \
		make -C $$I || exit 1;\
	done

.PHONY:clean
clean:
	-rm -f ${APP}
	find ${DIR_OUT} -name *.o -exec rm -rf {} \;

