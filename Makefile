CC = g++
IDIR = include/
SRCDIR = src/
OBJS = main.o connection.o plugbot.o connection_manager.o plugin_manager.o
CPPFLAGS = -std=c++11 -I$(IDIR)
LIBS = -lpthread -lboost_system -lboost_filesystem

all: PlugBot

PlugBot: $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o PlugBot

main.o: $(SRCDIR)main.cpp
	$(CC) $(CPPFLAGS) -c $(SRCDIR)main.cpp

connection.o: $(SRCDIR)connection.cpp
	$(CC) $(CPPFLAGS) -c $(SRCDIR)connection.cpp

plugbot.o: $(SRCDIR)plugbot.cpp
	$(CC) $(CPPFLAGS) -c $(SRCDIR)plugbot.cpp

connection_manager.o: $(SRCDIR)connection_manager.cpp
	$(CC) $(CPPFLAGS) -c $(SRCDIR)connection_manager.cpp

plugin_manager.o: $(SRCDIR)plugin_manager.cpp
	$(CC) $(CPPFLAGS) -c $(SRCDIR)plugin_manager.cpp

clean:
	rm *.o *~ PlugBot
