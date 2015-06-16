BINPATH = .
BINNAME = collector
TARGET =  $(BINPATH)/$(BINNAME)

SRCDIR = .

CC = g++

INCDIR = .

CFLAGS = -I$(INCDIR) -I/usr/local/include/mysql -I/usr/include/mysql \
		 -L/usr/local/lib/mysql/ -L/usr/lib/mysql/ 

LFLAGS = -std=c++11 -lev -lmysqlclient -Wall -DENABLE_LOGGER -L/usr/local/lib/mysql/ \
		 -L/usr/lib/mysql/ -I/usr/include/mysql

OBJS = collector.cpp logger.cpp config.cpp dataHandle.cpp ezMysql.cpp 


$(TARGET) : $(OBJS)
	$(CC) -g -rdynamic $(OBJS) $(LFLAGS)  -o $(TARGET)

$(SRCDIR)/%.cpp : $(INCDIR)/%.h

%.o : $(SRCDIR)/%.cpp
	#$(CC) $(CFLAGS) -o $@ $<
	$(CC) $(CFLAGS) -g $@ $<
	
#rm -f *.o
clean:
	rm -f *.o $(TARGET)

