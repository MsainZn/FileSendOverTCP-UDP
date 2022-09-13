all:	client server

ftpclient: client.c
	gcc -std=gnu99 -Wall $< -o $@

ftpserver: server.c
	gcc -std=gnu99 -Wall $< -o $@

clean:
	rm -f client server *.o *~ core
	rm -f server client


















