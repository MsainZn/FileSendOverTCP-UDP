/*
	How to compile ->

	(1) make all
		if no make was provided then gcc server.c utilityFunctionsMHZ.h -o server
	(2) ./server PORT_NUMBER
		Example: ./server 5500
	(3) ./client SERVER_ADDR PORT_NUMBER 
		Example: mainFile.log 172.17.34.89 5500
*/

#include "utilityFunctionsMHZ.h"

int main (int argc, char* argv[])
{
	// Input Parameters Error Check
	if (argc < 2) 
	{	fprintf(stderr, "Please use the following format to initiate the server: ./server PORT_NUMBER\n");
		exit(0);
	}
	// Input Parameters  
	int portNumber   		 	 = atoi(argv[1]); // 5500;
	size_t BUFFER_MAX 	   	 	 = 1024; // atoi(argv[4]);    // 1024;
	size_t maxClientWaitingQueue = 5;	 // atoi(argv[5]);    // 5;

	// server :: Socket Accept Client Request
	socklen_t socklen = sizeof(struct sockaddr_in);

	// socket End-Point
	int sockfd, sockfd_new;

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
	while (1)
	{
		// server :: Socket Listening
		if (listen(sockfd, maxClientWaitingQueue) == -1)
			CheckError("[-server] Error while Listening!\n");
		else
			printf("[+server] Listening...\n");
		
		sockfd_new = accept(sockfd, (struct sockaddr*) clientAddr, &socklen);
		if (sockfd == -1)
			CheckError("[-server] Error in Accepting Client Connection!\n");
		else
			printf("[+server] Connection Successfully Accepted.\n");
			
		printf("\n---------------------Server-Side::Connection Established Per Request----------------------.\n\n");
		while (1)
		{
			// Server recieve message from client
			bzero(bufferPtr, BUFFER_MAX);
			if (read(sockfd_new, bufferPtr, BUFFER_MAX) == -1)
				printf("[-server] Error While Read from buffer!\n");
			else
				printf("Client >> %s", bufferPtr);

			// Server Reply to client in a form of terminal
			bzero(bufferPtr, BUFFER_MAX);		 				 // clear buffer
			printf("~Me    >> ");
			fgets(bufferPtr, BUFFER_MAX, stdin);			     // get mssg from terminal (me)
			if (write(sockfd_new, bufferPtr, BUFFER_MAX) == -1)  // send buffer to client
				printf("[-server] Error While Writing from buffer!\n");
			
			// Exit Condition
			if (strncmp("CLIENT-EOF", bufferPtr, 10) == 0) break;
		}
		printf("\n---------------------Server-Side::Connection Terminated Per Request----------------------.\n");

		printf("Enter TURN-OFF to Turn Server OFF: ");
			fgets(bufferPtr, BUFFER_MAX, stdin);
			if (strncmp("TURN-EOF", bufferPtr, 10) == 0) break;
	}
	printf("\n---------------------Server Shut-Down Per Request----------------------.\n");

	close(sockfd);
	close(sockfd_new);

	return 0;
}