/*
	How to compile ->
	(1) make all
		if no make was provided then gcc server.c utilityFunctionsMHZ.h -o server
	(2) ./server FILENAME PORT_NUMBER
		Example: ./server 5500
	(3) ./client FILENAME SERVER_ADDR PORT_NUMBER 
		Example: mainFile.log 172.17.34.89 5500
*/	

#include "utilityFunctionsMHZ.h"

int main (int argc, char* argv[])
{
	// Input Parameters Error Check
	if (argc < 4) 
	{	fprintf(stderr, "Please use the given format: ./client SERVER_ADDR PORT_NUMBER\n");
		exit(0);
	}

	// Initial confirmation client side
	char checkSend;
	while (1)
	{
		printf("Start Sending File [Y/N]: ");
		scanf("%c", &checkSend);
		if (checkSend == 'N') 
		{
			printf("Ask Again In 60 Seconds.\n");
			sleep(60);
		}
		else
			break;
	} 


	// Input Parameters  
	const char* fileNameLocation = argv[1];
	char* serverIP 			 	 = argv[2];
	int portNumber   			 = atoi(argv[3]);			// 5500;
	size_t BUFFER_MAX 	   		 = 1024;//atoi(argv[4]);    // 1024;
	

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

	FILE* targetLogFile  = fopen(fileNameLocation, "r");

	FILE* sendStatusFile = fopen("sendStatus.log", "wra");

	fprintf (sendStatusFile, "DC-Counts \t Line-Sent \t Bytes-Sent \t Server-IP \t Server-Port \t Source-File-Name \n");
	//fprintf (sendStatusFile, "DC-Counts \t Line-Sent \t Bytes-Sent \t Server-IP \t Server-Port \t Source-File-Name \n");

	if (serverAddr == NULL || bufferPtr == NULL || targetLogFile == NULL) 
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
	
	printf("\n---------------------Client-Side::Sending Started------------------------\n\n");	
	while (!feof(targetLogFile))
	{ 
		printf("%lu\n", ftell(targetLogFile));//sleep(2);
		fgets(bufferPtr, BUFFER_MAX, targetLogFile);
		fprintf(targetLogFile, "%s", bufferPtr);
		if (write(sockfd, bufferPtr, BUFFER_MAX) == -1)
			printf("Error While Writting (Sending)!\n");
		bzero(bufferPtr, BUFFER_MAX);
		
	}
	printf("\n---------------------Client-Side::Sending Has Ended----------------------\n\n");	

	bzero(bufferPtr, BUFFER_MAX);

	if (write(sockfd, "/END-PROCESS/", 13) == -1)
		printf("Error While Writting (Sending)!\n");

    close(sockfd);
	fclose(targetLogFile);
	free(bufferPtr);

	return 0;
}


/*

	write(sockfd, "Ready to recieve Send Starts?", BUFFER_MAX);
	read(sockfd, bufferPtr, BUFFER_MAX);

	if (memcmp(bufferPtr, "yes", 3) == 0 || memcmp(bufferPtr, "y", 1) == 0 ||
		memcmp(bufferPtr, "YES", 3) == 0 || memcmp(bufferPtr, "Y", 1) == 0 ||
		memcmp(bufferPtr, "Yes", 3) == 0	)
	{
		bzero(bufferPtr, BUFFER_MAX);  			        
		printf("\n---------------------Client-Side::Connection Established Per Request----------------------.\n\n");
		while (fgets(bufferPtr, BUFFER_MAX, targetLogFile))
		{
			if (write(sockfd, bufferPtr, BUFFER_MAX) == -1)
				printf("Error While Writting (Sending)!\n");
			bzero(bufferPtr, BUFFER_MAX);
			sleep(10);
		}
		printf("\n---------------------Client-Side::Connection Terminated Per Request----------------------.\n");

		memset(bufferPtr, "EOF", 3);
		if (write(sockfd, bufferPtr, BUFFER_MAX) == -1)
			printf("Error While Writting (Sending)!\n");
	}




*/