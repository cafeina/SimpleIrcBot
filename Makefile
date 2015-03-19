CC = g++
IDIR = include/
SRCDIR = src/
OBJS = main.o connection.o plugbot.o connection_manager.o plugin_manager.o
CPPFLAGS = -std=c++11 -Wall -I$(IDIR) 
LIBS = -lpthread -lboost_system -lboost_filesystem

all: PlugBot

PlugBot: $(OBJS)
	$(CC) $^ $(LIBS) -o $@

main.o: $(SRCDIR)main.cpp
	$(CC) $(CPPFLAGS) -c $^

connection.o: $(SRCDIR)connection.cpp
	$(CC) $(CPPFLAGS) -c $^

plugbot.o: $(SRCDIR)plugbot.cpp
	$(CC) $(CPPFLAGS) -c $^

connection_manager.o: $(SRCDIR)connection_manager.cpp
	$(CC) $(CPPFLAGS) -c $^

plugin_manager.o: $(SRCDIR)plugin_manager.cpp
	$(CC) $(CPPFLAGS) -c $^

clean:
	rm *.o *~ PlugBot
