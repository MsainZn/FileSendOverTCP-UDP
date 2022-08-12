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
	if (argc < 3) 
	{	fprintf(stderr, "Please use the given format: ./client SERVER_ADDR PORT_NUMBER\n");
		exit(0);
	}

	// Input Parameters  
	char* serverIP 			 = argv[1];
	int portNumber   		 = atoi(argv[2]);			// 5500;
	size_t BUFFER_MAX 	   	 = 1024;//atoi(argv[4]);    // 1024;

	// Check IP integraty & server status
	if (serverIP == NULL)
	{
		fprintf(stderr, "[-client] Error in Socket Initiation Due to Either WRONG IP-Address or Server is Currently OFF!\n");
		exit(0);
	}

	// socket End-Point
	int sockfd;
	
	// Error Flags
	int openFileFlag, sendBufferFlag;

	// Socket main Structures
	struct sockaddr_in* serverAddr = 
						(struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
	// client buffer
	char* bufferPtr   = (char*) calloc (BUFFER_MAX, sizeof(char));

	if (serverAddr == NULL || bufferPtr == NULL)
		CheckError("[-client] Memory Allocation for sockadd_in strctures!\n");
	else
		printf("[+client] memory Allocation Successfully Completed.\n");

	// client :: Initiation Socket via TCP Protocol & Error Check
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		CheckError("[-client] Error in Socket Initiation!\n");
	else
		printf("[+client] Socket Successfully Initiated.\n");

	// Fill address with zero
	bzero(serverAddr, sizeof(struct sockaddr_in));   //memset(serverAddr, 0x00, sizeof(struct sockaddr_in));
	bzero(bufferPtr, BUFFER_MAX);  			         //memset(clientAddr, 0x00, BUFFER_MAX);

	// Set-up server parameters
	serverAddr->sin_family      = AF_INET;
    //bcopy((char*) serverIP->h_addr, serverAddr->sin_addr.s_addr, serverIP->h_length);
	serverAddr->sin_addr.s_addr = inet_addr(serverIP);
	serverAddr->sin_port        = htons(portNumber);
	
	// Connection Request to Server
	if (connect(sockfd, (struct sockaddr*) serverAddr, sizeof(struct sockaddr_in)) == -1)
		CheckError("[-client] Error in Connecting to Server!\n");
	else
		printf("[+client] Connection Successfully Accepted.\n");

	////////////////////////////////////////////////////////////////////////////////////
	int n1, n2, answ;
	///////////////////////////////////////////////////////////////////////////////////
	printf("\n---------------------Client-Side::Connection Established Per Request----------------------.\n\n");
	while (1)
	{
		if (read(sockfd, bufferPtr, BUFFER_MAX) == -1)
			printf("[-client] Error While Read from buffer!\n");
		printf("%s", bufferPtr);
		scanf("%d",&n1);
		bzero(bufferPtr, BUFFER_MAX);
		if (write(sockfd, &n1, sizeof(int)) == -1)
			printf("[-client] Error While Writing from buffer!\n");
	
		if (read(sockfd, bufferPtr, BUFFER_MAX) == -1)
			printf("[-client] Error While Read from buffer!\n");
		printf("%s", bufferPtr);
		scanf("%d",&n2);
		bzero(bufferPtr, BUFFER_MAX);
		if (write(sockfd, &n2, sizeof(int)) == -1)
			printf("[-client] Error While Writing from buffer!\n");
/*
		while (1)
		{
			bzero(bufferPtr, BUFFER_MAX);
			if (read(sockfd, bufferPtr, BUFFER_MAX) == -1)
				printf("[-client] Error While Read from buffer!\n");
			printf("%s ", bufferPtr);
			bzero(bufferPtr, BUFFER_MAX);

			scanf("%s", bufferPtr);
			if (write(sockfd, bufferPtr, BUFFER_MAX) == -1)
				printf("[-client] Error While Writing from buffer!\n");
			
			bzero(bufferPtr, BUFFER_MAX);
			if (read(sockfd, bufferPtr, BUFFER_MAX) == -1)
				printf("[-client] Error While Read from buffer!\n");
			
			if (memcmp(bufferPtr, "ok", 2) == 0) break;			
		}
		if (read(sockfd, &answ, sizeof(int)) == -1)
				printf("[-client] Error While Read from buffer!\n");
		
		printf("The Result is: %d\n", answ);
*/
		// Client Exit
		bzero(bufferPtr, BUFFER_MAX);
		if ( read(sockfd, bufferPtr, BUFFER_MAX) == -1 )
			printf("[-client] Error While Read from buffer!\n");
		printf("%s", bufferPtr);

		bzero(bufferPtr, BUFFER_MAX);
		fgets(bufferPtr, BUFFER_MAX, stdin);
		if (write(sockfd, bufferPtr, BUFFER_MAX) == -1)
			printf("[-client] Error While Writing from buffer!\n");
	
		if ( memcmp(bufferPtr,"N",  1) == 0 || memcmp(bufferPtr,"n",  1) == 0 ||
			 memcmp(bufferPtr,"NO", 2) == 0 || memcmp(bufferPtr,"no", 2) == 0 || 
			 memcmp(bufferPtr,"No", 2) == 0 ) 
			 break;
			  
		bzero(bufferPtr, BUFFER_MAX); 

	}
	printf("\n---------------------Client-Side::Connection Terminated Per Request----------------------.\n");

    close(sockfd);

	free(bufferPtr);

	return 0;
}