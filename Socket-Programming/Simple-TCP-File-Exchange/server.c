/*
	How to compile ->

	(1) make all
		if no make was provided then gcc server.c utilityFunctionsMHZ.h -o server
	(2) ./server FILENAME SERVER_ADDR PORT_NUMBER BUFFER_SIZE WAITING_CLIENTS
		Example: ./server mainlog.log 172.17.34.89 5500 1024 5
	(3) ./client FILENAME SERVER_ADDR PORT_NUMBER 
		Example: mainFile.log 172.17.34.89 5500 1024
*/

#include "utilityFunctionsMHZ.h"

int main (int argc, char* argv[])
{
	// Input Parameters Error Check
	if (argc < 6) 
	{	fprintf(stderr, "Please use the following format to initiate the server: ./server FILENAME SERVER_ADDR PORT_NUMBER BUFFER_SIZE WAITING_CLIENTS\n\
 		    			       Example: ./server mainlog.log 172.17.34.89 5500 1024 5\n");
		exit(0);
	}
	// Input Parameters  
	char* logFileName		 	 = argv[1]; 		 //"mainFile.log";
	char* serverIP   		 	 = argv[2];			 //"192.168.1.1";
	int portNumber   		 	 = atoi(argv[3]);	 // 5500;
	size_t BUFFER_MAX 	   	 	 = atoi(argv[4]);    // 1024;
	size_t maxClientWaitingQueue = atoi(argv[5]);    // 5;

	// socket End-Point
	int sockfd, sockfd_new;

	// Error Flags
	int openFileFlag, sendBufferFlag;

	// Socket main Structures
	struct sockaddr_in* serverAddr = 
						(struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
	struct sockaddr_in* clientAddr = 
						(struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
	// client buffer
	char* bufferPtr = (char*) calloc (BUFFER_MAX, sizeof(char));
	
	if (clientAddr == NULL || serverAddr == NULL || bufferPtr == NULL)
		CheckError("[-server] Memory Allocation for sockadd_in strctures!\n");
	else
		printf("[+server] memory Allocation Successfully Completed.\n");

	// client :: Initiation Socket via TCP Protocol & Error Check
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockfd == -1)
		CheckError("[-server] Error in Socket Initiation!\n");
	else
		printf("[+server] Socket Successfully Initiated.\n");

	// Fill address with zero
	bzero(serverAddr, sizeof(struct sockaddr_in)); //memset(serverAddr, 0x00, sizeof(struct sockaddr_in));
	bzero(clientAddr, sizeof(struct sockaddr_in)); //memset(clientAddr, 0x00, sizeof(struct sockaddr_in));
	bzero(bufferPtr, BUFFER_MAX);  			       //memset(clientAddr, 0x00, BUFFER_MAX);
	
	// set-up Server parameters
	serverAddr->sin_family      = AF_INET;
    serverAddr->sin_addr.s_addr = INADDR_ANY;// inet_addr(serverIP);
	serverAddr->sin_port        = htons(portNumber);

	// server :: Socket Binding
	if (bind(sockfd, (struct sockaddr*) serverAddr, sizeof(struct sockaddr_in)) == -1)
		CheckError("[-server] Error in Socket Binding!\n");
	else
		printf("[+server] Socket Successfully Binded.\n");

	// server :: Socket Listening
	if (listen(sockfd, maxClientWaitingQueue) == -1)
		CheckError("[-server] Error while Listening!\n");
	else
		printf("[+server] Listening...\n");
	
	// server :: Socket Accept Client Request
	socklen_t socklen = sizeof(struct sockaddr_in);
	sockfd_new = accept(sockfd, (struct sockaddr*) clientAddr, &socklen);
	if (sockfd == -1)
		CheckError("[-server] Error in Accepting Client Connection!\n");
	else
		printf("[+server] Connection Successfully Accepted.\n");
		
	printf("\n---------------------Server-Side::Connection Established Per Request----------------------.\n");
	while (1)
	{
		// Server recieve message from client
		bzero(bufferPtr, BUFFER_MAX);
		if (read(sockfd_new, bufferPtr, BUFFER_MAX) == -1)
			CheckError("[-server] Error While Read from buffer!\n");
		else
			printf("Client >> %s", bufferPtr);

		// Server Reply to client in a form of terminal
		bzero(bufferPtr, BUFFER_MAX);		 				 // clear buffer
		printf("ME     >> ");
		fgets(bufferPtr, BUFFER_MAX, stdin);			     // get mssg from terminal (me)
		if (write(sockfd_new, bufferPtr, BUFFER_MAX) == -1)  // send buffer to client
			CheckError("[-server] Error While Writing from buffer!\n");
		
		// Exit Condition
		if (strncmp("SERVER-EOF", bufferPtr, 10) == 0) break;
	}
	printf("\n---------------------Server-Side::Connection Terminated Per Request----------------------.\n");
    close(sockfd);
	close(sockfd_new);

	return 0;
}



// Standard C functions 
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
// Standard socket functions & data structures (sockaddr & sockaddr_in)
//#include <arpa/inet.h>

//#include <sys/types.h>
//#include <sys/socket.h>
//#include <errno.h>
//#include <unistd.h>
//#include <netinet/in.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <sys/sendfile.h>


/*
	// FILE Send
	FILE *logFileSend = NULL;
	logFileSend = open(logFileName);

	if (logFileSend == NULL)
	{
		perror("[-server] File Open Encountered Error!\n");
		exit(-1);
	}
	else
	{
		printf("[+server] File Successfully Opened.\n");
	}

	int offset = 0;
	do
	{
		memset(sendBuffer, 0x00, BUFFER_MAX);
		sendBufferFlag = send(sockfd_new, sendBuffer, &offset, BUFFER_MAX);
	
	} while (sendBufferFlag>0);
*/	