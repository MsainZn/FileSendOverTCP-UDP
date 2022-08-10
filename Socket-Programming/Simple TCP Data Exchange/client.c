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
	char* bufferPtr = (char*) calloc (BUFFER_MAX, sizeof(char));
	
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
	char optyp;
	///////////////////////////////////////////////////////////////////////////////////
	printf("\n---------------------Client-Side::Connection Established Per Request----------------------.\n\n");
	while (1)
	{
		if (read(sockfd, bufferPtr, BUFFER_MAX) == -1)
			printf("[-client] Error While Read from buffer!\n");
		printf("%s: \n", bufferPtr);
		scanf("%d",&n1);
		if (write(sockfd, &n1, sizeof(int)) == -1)
			printf("[-client] Error While Writing from buffer!\n");
	
		if (read(sockfd, bufferPtr, BUFFER_MAX) == -1)
			printf("[-client] Error While Read from buffer!\n");
		printf("%s: \n", bufferPtr);
		scanf("%d",&n2);
		if (write(sockfd, &n1, sizeof(int)) == -1)
			printf("[-client] Error While Writing from buffer!\n");
		bzero(bufferPtr, BUFFER_MAX);

		do {
			if (read(sockfd, bufferPtr, BUFFER_MAX) == -1)
				printf("[-client] Error While Read from buffer!\n");
			printf("%s: \n", bufferPtr);

			if (read(sockfd, bufferPtr, sizeof(char)) == -1)
				printf("[-client] Error While Read from buffer!\n");
			printf("%s: \n", bufferPtr);

			scanf("%c",&optyp);
			if (write(sockfd, &optyp, sizeof(int)) == -1)
				printf("[-client] Error While Writing from buffer!\n");
		} while (optyp);
		


		// Server recieve message from client
		bzero(bufferPtr, BUFFER_MAX);
		printf("~Me    >> ");
		fgets(bufferPtr, BUFFER_MAX, stdin);
	
		if (write(sockfd, bufferPtr, BUFFER_MAX) == -1)
			printf("[-client] Error While Writing from buffer!\n");

		// Client Exit
		if (strncmp("EOF", bufferPtr, 3) == 0) break;

		
	}
	printf("\n---------------------Client-Side::Connection Terminated Per Request----------------------.\n");

    close(sockfd);

	return 0;
}


/*

char* logFileRecievedName = argv[1]; //"mainFile.log";
	char* serverIP = argv[2];			 //"192.168.1.1";
	int portNumber = atoi(argv[3]);	     // 5500
	// client side buffer size
	size_t BUFFER_MAX = atoi(argv[4]);	 // 1024

	// client buffer
    char* recvBuffer = (char*) calloc (BUFFER_MAX, sizeof(char));
	// Error Flags
	int connectFlag, openFileFlag, recvBufferFlag;
	// Socket End-Point
	int sockfd_client;

    // client :: Socket Initiation Via TCP Protocol
	sockfd_client = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd_client == -1)
	{
		perror("[-client] Error in Socket Initiation!\n");
		exit(-1);
	}
	else
	{
		printf("[+client] Socket Successfully Initiated via Socket Number: %d.\n", sockfd_client);
	}

    // server info for connect
	struct sockaddr_in* serverAddr = 
						(struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
	
	// Fill address with zero
	memset(serverAddr, 0x00, sizeof(struct sockaddr_in));
	serverAddr->sin_family      = AF_INET;
    serverAddr->sin_addr.s_addr = inet_addr(serverIP);
	serverAddr->sin_port        = htons(portNumber);
	

    // client :: Socket Initiation Via TCP Protocol
	connectFlag = connect(sockfd_client, (struct sockaddr*) serverAddr, sizeof(struct sockaddr_in));
    if (connectFlag == -1)
	{
		perror("[-client] Error in Socket connection!\n");
		exit(-1);
	}

	// FILE Handeling
	FILE *logFileRecieved = NULL;
	// Nedd change for later
	logFileRecieved = fopen(logFileRecievedName, "w");

	if (logFileRecieved == NULL)
	{
		perror("[-client] File Open Encountered Error!\n");
		exit(-1);
	}
	else
	{
		printf("[+client] File Successfully Created.\n");
	}

	do
	{
		memset(recvBuffer, 0x00, BUFFER_MAX);
		recvBufferFlag = recv(sockfd_client, recvBuffer, BUFFER_MAX, 0);		
		fwrite(recvBuffer, sizeof(char), recvBufferFlag, logFileRecieved);
	} while (recvBufferFlag>0);
	
    
	fclose(logFileRecieved);
    close(sockfd_client);



// Standard C functions 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Standard socket functions & data structures (sockaddr & sockaddr_in)
#include <arpa/inet.h>
// A header file that provides access to the POSIX operating system API (READ, WRITE, LINK, SYNC, ...).
#include <unistd.h>

#include <fcntl.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>




*/