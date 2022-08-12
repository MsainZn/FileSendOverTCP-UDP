
socket programming: A way for computer to communication over network 
-------------------------------------------------------------
How to compile ->
(1) ./server FILENAME SERVER_ADDR PORT_NUMBER BUFFER_SIZE WAITING_CLIENTS
	Example: ./server mainlog.log 172.17.34.89 5500 1024 5
(2) ./client FILENAME SERVER_ADDR PORT_NUMBER 
	Example: mainFile.log 172.17.34.89 5500 1024
-------------------------------------------------------------
Main includes:	
#include <stdio.h>        :: standard IO c-functions
#include <cstring.h>      :: standard c-string functions
#include <unistd.h>       :: standard POSIX operating system API (read/write/close/send/recv)
#include <stdlib.h>       :: standard ALLOC c-functions (malloc, atoi)
#include <sys/types.h>    :: definitions of data types used in sys/socket & netinet/in
#include <sys/socket.h>   :: definitions of structures needed for internet domain such as (struct sockaddr)
#include <netinet/in.h>   :: definitions of (struct sockaddr_in)
#include <netdb.h>    	  :: definitions of (struct hostnet) to store info of a given host & IPV-4
-------------------------------------------------------------
Socket-Phone Analogy:
1- socket()       Endpoint for communication - Having telephone
2- bind()         Assign a unique telephone number 
3- listen()       Wait for a caller
4- connect()      Dial a number
5- accept() 	  Recieve a call
6- send(), recv() Talk on phone
7- close()        Hang up	
Key-Points:
server-job is : socket() -> bind() -> listen() -> accept() -> read() -> write() -> close()
client-job is : socket() ->          -> connection()       -> read() -> write() -> close()
<< socket Port Number Needs to be the same >>
------------------------------------------------------------
Client-Server model:
Server: An entity which provides information
Client: An entity which seeks information
General Description:
	Server starts up first and waits for client-server
	to connect. After client connects it requests information.
	Finally the connection is finished and then it disconnects.
TCP Description:
	server :: calls socket() to create communication end-point
	client :: calls socket() to create communication end-point
	server :: calls bind() to attach a unique address
			creates IP that serves as an address
			creates port number which allows communicate to that machine
	server :: calls listen() to wait for client call
	client :: calls connect() estblish connection with server
	server :: calls accept() to accept client connection
	server/client :: both call read() and write() to exchange information 
	close() :: calls close() to end connection
------------------------------------------------------------
TCP/IP Data Structures:
In general, we connect to particular port of an IP address.
All we need to connect is IP address and port number.

// use for any network communication
struct sockaddr{
	// AF_INET
	unsigned short sa_family;
	// IP address and port number of server we want to connect
	char sa_data[14];
}

// we use this one ad then cast it to the upper when passing
// just use for intenet connection
struct sockaddr_in{
	// Network domain that is AF_INET
	short sin_family;
	// Port number
	unsigned short sin_port;
	// IP Adress
	struct in_addr sin_addr;
	// ???
	char sin_zero[8];
}

// 4-byte integer and an IP address 
struct in_addr {
	unsigned long s_addr;
}	
------------------------------------------------------------
Byte-Ordering: 
Saving format of communication [All network is Big Endian format]
We should always convert Byte-Order to Big Endian on any OS.

// host-to-network-short host-to-network-long
Host to Network byte-order: htons() htonl() 
// network-to-host-short network-to-host-long
Network to Host byte-order: ntohs() nohl()

// IP Format Changing (xxx.xxx.xx.x to Binary):
ASCII-dotted to binary: inet_aton()
Binary to ASCII-dotted: inet_ntoa()
------------------------------------------------------------
Main Functions:
(1) socket() function
	Goal -> Create an end-point for a network connection 
	int socket(int domain, int type, int protocol);
		domain   = AF_INET (IPv4 communication) :: Internet-Protocol-Version-4
					PF_INET (Exactly similar but some Refs use it)
		type     = SOCK_STREAM (TCP), SOCK_DGRAM (UDP)
		protocol = 0 (default for TCP)
		result == -1 is error - Any positive integer is Ok 
	Example: 
		socket (PF_INET, SOCK_STREAM, 0)

(2) bind() function
	Goal -> Attach server to a specific port and IP --> bind struct sockadd* & sockfd  
	int bind(int sockfd, struct sockadd* my_addr
				, socklen_t addrlen);
		sockfd   = socket descriptor returned by socket()
		my_addr  = pointer to a valid sockaddr_in structure
					which is added as a sockaddr* pointer
		addrlen  = sizeof(struct sockaddr_in)
		result   = -1 is error - 0 is Ok
	Example: 
		struct sockaddr_in* my_addr = (struct sockaddr_in*) 
										malloc(sizeof(struct sockaddr_in));
		bzero(my_addr, sizeof(struct sockaddr_in)); or memset(my_addr, 0x00, sizeof(struct sockaddr_in));
		
		my_addr->sin_family = AF_INET;
		my_addr->sin_port   = htons(80);
		my_addr->sin_addr.s_addr = INADDR_ANY; (All available IP address in domain);
		
		bind(sockfd, (struct sockaddr*) my_addr, sizeof(struct sockaddr_in));	

(3) listen() function
	Goal -> Initialize a wait queue of connections for this socket 
	int listen(int sockfd, int backlog); 
		sockfd   = socket descriptor returned by socket()
		backlog  = maximum length of the pending connections queue
		result   = -1 is error - Any positive values is Ok
	Example: 
		listen(sockfd, 10);	

(4) connect function
	Goal -> client calls connect to establish connection with server port
	int connect(int sockfd, (struct sockaddr*) server_addr, socklen_t* serverLen); 
		sockfd      = socket descriptor returned by socket()
		server_addr = server address (info) that is a struct sockaddr_in* pointer
					casted into sockaddr* pointer
		serverLen   = Pointer to sizeof(sockaddr_in) of server
		result      = -1 is error - Any positive values is Ok
	Example: 
		connect (sockfd, (struct sockaddr*) server_addr, 
					sizeof(struct sockaddr_in));

(5) accept() function
	Goal -> server Accepts a client request [connect() function]
	int accept(int sockfd, (struct sockaddr*) client, socklen_t* clientLen); 
		sockfd      = socket descriptor returned by socket() -> server
		client_addr = client (info) address that is a struct sockaddr_in* pointer
					casted into sockaddr* pointer
		clientLen   = Pointer to sizeof(sockaddr_in)
		result      = A new file descriptor 
					(All the communications will now be commence with this new_sockfd)
	Example: 
		struct sockaddr_in* client_addr = (struct sockaddr_in*) 
							malloc(sizeof(struct sockaddr_in));

		int clientLen = sizeof(struct sockaddr_in*) of client;
		accept(sockfd, (struct sockaddr*) client_addr, &clientLen);

(6) send()/recv()/read()/write() functions
	Goal -> data pass
	int read (int newsockfd, void* messgBuff, size_t messgBuffLen); 
	int write(int newsockfd, void* messgBuff, size_t messgBuffLen); 
		sockfd      = new socket descriptor returned by accept() function
		messBuffer  = Pointer to buffer
		messBuffLen = Size of the messege buffer

	int send(int sockfd, void* messgBuff, size_t messgBuffLen, int flag); 
	int recv(int sockfd, void* messgBuff, size_t messgBuffLen, int flag); 
		sockfd      = socket descriptor returned by socket()
		messBuffer  = Pointer to buffer
		messBuffLen = Size of the messege buffer
		flags       = 0
		result      = returns the actual byte sent/recv.
	Example: 
		char sendBuffer [1024];
		char recvBuffer [1024];
		int sendBytes = send(sockfd, sendBuffer, 1024, 0);
		int recvBytes = recv(sockfd, recvBuffer, 1024, 0);
		
(7) close function
	Goal -> Close the connection
	int close(int sockfd);
	sockfd = socket descriptor returned by socket()
	Example: 
		close(sockfd);


