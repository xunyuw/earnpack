BINPATH = .
BINNAME = client
TARGET =  $(BINPATH)/$(BINNAME)

SRCDIR = .

CC = g++

INCDIR = .

CFLAGS = -pipe -c -I$(INCDIR) -I/usr/local/include/mysql -I/usr/include/mysql -Wno-deprecated  \
		 -L/usr/local/lib/mysql/ -L/usr/lib/mysql/ 

LFLAGS =-std=c++11 -g -lev -lmysqlclient -lm -lz -lpthread -Wall -DMAIN_TEST -L/usr/local/lib/mysql/ \
		 -L/usr/lib/mysql/ -L/usr/local/lib -I/usr/include/mysql

OBJS = ../dataHandle.cpp ../ezMysql.cpp client.cpp


$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LFLAGS) 

$(SRCDIR)/%.cpp : $(INCDIR)/%.h

%.o : $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ $<
	
#rm -f *.o
clean:
	rm -f *.o $(TARGET)

