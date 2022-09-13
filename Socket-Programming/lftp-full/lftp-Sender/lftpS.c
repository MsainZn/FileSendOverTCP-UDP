/*
	How to compile ->
	(1) make all
		if no make was provided then gcc server.c utilityFunctionsMHZ.h -o server
	(2) ./server FILENAME PORT_NUMBER SLEEP_TIMER MAX_BUFFER_SIZE
		Example: ./server mainFile2.log 5500 10 4096
	(3) ./client FILENAME SERVER_ADDR PORT_NUMBER 
		Example: ./client mainFile.log 172.17.34.89 5500
*/	

#include "utilityFunctionsMHZ.h"

int main (int argc, char* argv[])
{
	// Initial Error Check And Call Initializer
	if (argc < 4) 
	{	
		fprintfSwitchable(NULL, 0, "[-client] Please use the Standard Input Format: ./lftpS FILENAME SERVER_ADDR PORT_NUMBER\n");
		exit(0);
	}
	else if (!isFileAvailable(argv[1], "client", NULL))
	{
		exit(0);
	}
	else
	{
		Initializer ("client", NULL);
		fprintfSwitchable(NULL, 0, "[+%s] Make Sure to Run server First.\n", "client");
	}
		
	// Input Parameters  
	const char* fileLocFull = argv[1];
	char* serverIP 			= argv[2];
	int portNumber   		= atoi(argv[3]);

	// Buffer & Cache Properties and Size 
	struct bufferTCP* mssg = CreateBufferTCP();
	size_t MAX_CACHE_SIZE, CACHE_SIZE;
	char* cacheBuffer;

	// File Process, Data Structure (NACK-Sender & ACK-Reciever)
	FILE* filePtr_NACK;
    size_t fileMemory_NACK = 0;
	size_t fileMemory_ACK = 0;
	size_t fileRemainMemory = 0;

	// Create Socket via TCP Protocol & Error Check
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		fprintfSwitchable(NULL, 1, "[-client] Error in Socket Initiation!\n");
	else
		fprintfSwitchable(NULL, 0, "[+client] Socket Successfully Initiated.\n");
	// Socket main Structures
	struct sockaddr_in* serverAddr = malloc(sizeof(struct sockaddr_in));
	
	// Socket Structures Memory Allocation Test
	if (serverAddr == NULL) 
		fprintfSwitchable(NULL, 1, "[-client] Memory Allocation for sockadd_in strctures Failed!\n");
	else
		fprintfSwitchable(NULL, 0, "[+client] Memory Allocation for sockadd_in strctures Successfully Completed.\n");
	
	// Set-up server parameters
	bzero(serverAddr, sizeof(struct sockaddr_in));   //memset(serverAddr, 0x00, sizeof(struct sockaddr_in));
	serverAddr->sin_family      = AF_INET;
	//serverAddr->sin_addr.s_addr = inet_addr(serverIP);
	inet_pton(AF_INET, serverIP, &(serverAddr->sin_addr));
	serverAddr->sin_port        = htons(portNumber);

	// Connection Request to Server
	if (connect(sockfd, (struct sockaddr*) serverAddr, sizeof(struct sockaddr_in)) == -1)
		fprintfSwitchable(NULL, 1, "[-client] Error in Connecting to Server!\n");
	else
		fprintfSwitchable(NULL, 0, "[+client] Connection Successfully Accepted.\n");

	if (read(sockfd, &MAX_CACHE_SIZE, sizeof(size_t)) == -1)
		fprintfSwitchable(NULL, 1, "[-client] Error in MAX CACHE Size Negitiation!\n"); 
	else 
		fprintfSwitchable(NULL, 0, "[+client] CACHE Size is Set Equal to Server: %lu.\n", MAX_CACHE_SIZE);
	
	// Initializing Sender Sleep Clock
	size_t sleepTimer;
	if (read(sockfd, &sleepTimer, sizeof(size_t)) == -1)
		fprintfSwitchable(NULL, 1, "[-client] Error in Sleep Timer Negitiation!\n", 1, 1); 
	else 
		fprintfSwitchable(NULL, 0, "[+client] Sleep Timer is Set Equal to Server: %lu\n", sleepTimer); 
	
	int iack;
	int ACKFLAG = 0;
	size_t OveralIteration = 0;
	size_t SentBytes;
	size_t Not_Written = 0;
	ssize_t readFLAG;
	ssize_t writeFLAG;
	size_t InnerIteration = 0;

	fprintfSwitchable(NULL, 0, "**********************Client-Side::Sending Started**************************\n"); 
	while (1)
	{
		// Exit Condition
		if (isEnd (sleepTimer, "client"))  break;

		fileMemory_NACK = FileSizeCalculator(fileLocFull);

        // File Status @ Server (Reciever) 
        if (read(sockfd, &fileMemory_ACK, sizeof(size_t)) == -1)
			fprintfSwitchable(NULL, 1, "[-client] Error in reading File Info!\n"); 

		// Send-Recieve Validity
        fileRemainMemory = fileMemory_NACK - fileMemory_ACK;
			
		// Send Expecting Data Size For Server
		if (write(sockfd, &fileRemainMemory, sizeof(size_t)) == -1)
			fprintfSwitchable(NULL, 1, "[-client] Failed To Send Remaining Memory Info!\n"); 
 
		// Assess Sending
		if (fileMemory_NACK == fileMemory_ACK) 
		{
			fprintfSwitchable(NULL, 0, "[+client] Remaining Memory is Zero. Send Process Will Contine After %lu[s] Timeout.\n", sleepTimer);;  
			continue;
		}

		fprintfSwitchable(NULL, 0, "[+client] Session [%lu] Overview: \nACK Memory is %lu\nNACK Memory is %lu\nRemaining Memory is: %lu\n\n", 
                                OveralIteration, fileMemory_ACK, fileMemory_NACK, fileRemainMemory);
		
		// Creating Cache
		CACHE_SIZE = fileRemainMemory;
		cacheBuffer = (char*) calloc (CACHE_SIZE, sizeof(char));
		bzero(cacheBuffer, CACHE_SIZE);
		
		// Copy Data From File to CACHE-Buffer
		filePtr_NACK = fopen(fileLocFull, "r");
        fseek(filePtr_NACK, fileMemory_ACK, SEEK_SET);
        fread(cacheBuffer, sizeof(char), CACHE_SIZE, filePtr_NACK);
		fclose(filePtr_NACK);
		
		ACKFLAG = 0;
		SentBytes = 0;
		fileRemainMemory = CACHE_SIZE;
		InnerIteration = 0;
		while (1)
		{
			// EXIT CONDITION
			if (fileRemainMemory == 0) break;
			
			// Evaluate Buffer Size
			if (fileRemainMemory >= BUFFER_MAX) 					
				mssg->BUFFER_USED = BUFFER_MAX;
			else 
				mssg->BUFFER_USED = fileRemainMemory;
			
			// Copy From CACHE to Socket Buffer and Send
			bzero(mssg->mssgPtr, BUFFER_MAX);
			memcpy(mssg->mssgPtr, cacheBuffer+SentBytes, mssg->BUFFER_USED);
			
			while ( (writeFLAG = write(sockfd, mssg->mssgPtr, mssg->BUFFER_USED)) != mssg->BUFFER_USED );
			//writeFLAG = write(sockfd, mssg->mssgPtr, mssg->BUFFER_USED);
			// Cases:
			if (writeFLAG == 0) 
			{
				fprintfSwitchable(NULL, 0, "[-client] Nothing to write\n"); 
			}
			else if (writeFLAG == -1)
			{
				// Display Problem
				fprintfSwitchable(NULL, 0, "[-client] Error While Sending Buffer!\n");
				fprintfSwitchable(NULL, 0, "[-client] %s\n", strerror(errno));
			} 
			else
			{
				// In Session Statistics	
				fileRemainMemory -= mssg->BUFFER_USED;
				SentBytes		 += mssg->BUFFER_USED;
			}	
			fprintfSwitchable(NULL, 0, "[+client] (%lu) Current-Iteration | (%lu) Total-Sent | (%lu) Remaining.\n", writeFLAG, SentBytes, fileRemainMemory);
			bzero(mssg->mssgPtr, BUFFER_MAX); 
		}
		if (writeFLAG != -1 && fileRemainMemory == 0)
		{
			fileMemory_ACK = SentBytes;
			OveralIteration++;
		}
			
		// Session Summary
		fprintfSwitchable(NULL, 0, "[+client] Session %lu Summary:\nACK Memory is %lu :: Remaining Memory is: %lu\n\n", 
                                          OveralIteration, fileMemory_ACK, fileRemainMemory);	
		free(cacheBuffer);
	
	}
	fprintfSwitchable(NULL, 0, "\n***********************Client-Side::Sending Ended***************************\n");	

    close(sockfd);
	free(serverAddr);
	free(mssg);

	return 0;
}
