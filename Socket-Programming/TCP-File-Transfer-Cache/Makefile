CC=gcc
CFLAGS=-Wall -g

all: sender reciever

sender.o: client.c utilityFunctionsMHZ.h
	$(CC) $(CFLAGS) -c client.c

reciever.o: server.c utilityFunctionsMHZ.h
	$(CC) $(CFLAGS) -c server.c

reciever: server.c reciever.o
	$(CC) $(CFLAGS) -o $@ $^

sender: client.c sender.o
	$(CC) $(CFLAGS) -o $@ $^
