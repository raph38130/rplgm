#
#		Makefile to generate a Tango device server client
#
BASE=/disk/removable
TANGO_BASE = $(BASE)/projet2/TANGO/

CC = c++
TANGO_LIB = $(TANGO_BASE)/lib/suse72/

#OOC = /mnt/ooc
OOC = /home/raph/ooc

INCLUDE_DIRS = -I $(OOC)/include -I $(TANGO_BASE)/include/suse72 \
               -I $(MYSQL_BASE)/include/mysql \
	       -I . 

LIB_DIRS = -L $(MYSQL_BASE)/lib/mysql \
	 -L $(TANGO_LIB) -L $(OOC)/lib

#-DOB_NO_VERSION_CHECK
CFLAGS = -g $(INCLUDE_DIRS) -DDEBUG -D_REENTRANT 
LFLAGS_SERVER = $(LIB_DIRS) -ltango -lOB -lJTC -lmysqlclient -lpthread 
LFLAGS_CLIENT = -g $(LIB_DIRS) -lOB -lJTC -ltango  -lpthread -ldl
LFLAGS_TACO = -L$(TACO_BASE)/lib/linux/x86 -ltacog++
#		-ldsapig++ -ldcapi -ldbapi -ldsxdr 

CFLAGS_TACO = -I$(TACO_BASE)/include -I$(TACO_BASE)/include++ \
	-I$(TACO_BASE)/include/private


Tango2client: Tango2client.o 
	$(CC) Tango2client.o $(LFLAGS_CLIENT) -o Tango2client 

Tango2client.o: Tango2client.cpp
	$(CC) Tango2client.cpp $(CFLAGS) -c -o Tango2client.o

CClient: CClient.o 
	$(CC) CClient.o $(LFLAGS_CLIENT) -o CClient 

CClient.o: CClient.cpp
	$(CC) CClient.cpp $(CFLAGS) -c -o CClient.o

clean:
	rm -f *.o core


