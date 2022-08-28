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
	if (argc < 5) 
	{
		fprintfSwitchable(NULL, 0, "[+server] Please use the following format to initiate the ./server FILENAME PORT_NUMBER SLEEP_TIMER MAX_CACHE_SIZER\n");
		exit(0);
	}	
	else
	{
		Initializer ("server", NULL);
	}
		
	// Input Parameters  
	const char* fileLocFull = argv[1];
	int portNumber          = atoi(argv[2]); 	     
	size_t sleepTimer       = atoi(argv[3]); 
	size_t MAX_CACHE_SIZE   = atoi(argv[4]);
	size_t WaitQueue        = 5;
	
	// server :: Socket Accept Client Request
	socklen_t socklen = sizeof(struct sockaddr_in);

	// File Process, Data Structure (NACK is the Sender/Client Side)
	if (!isFileAvailable(argv[1], "server", NULL))
	{
		FILE* filePtr_ACK = fopen(fileLocFull, "w");
		fclose(filePtr_ACK);
	}
	
	//size_t checker = FileCorrector (fileLocFull);
	size_t checker = FileModifier (fileLocFull);

	// File Process, Data Structure (NACK-Sender & ACK-Reciever)
	FILE* filePtr_ACK;
    size_t fileMemory_ACK = 0;
	size_t fileRemainMemory = 0;
	
	// Buffer Properties and Size (Set Sender/Client Buffer-Size Equal to Server)
	size_t BUFFER_MAX, BUFFER_USED;
	BUFFER_MAX = 1024;
	char* bufferPtr = (char*) calloc(BUFFER_MAX, sizeof(char));

	// Cache properties
	size_t CACHE_SIZE;
	char* cacheBuffer;

	// socket End-Point
	int sockfd, sockfd_new;
	// client :: Initiation Socket via TCP Protocol & Error Check
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		fprintfSwitchable(NULL, 1, "[-server] Error in Socket Initiation!\n");
	else
		fprintfSwitchable(NULL, 0, "[+server] Socket Successfully Initiated.\n");

	// Socket main Structures
	struct sockaddr_in* serverAddr;
	struct sockaddr_in* clientAddr;
	serverAddr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));		
	clientAddr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
	
	// Socket Structures Memory Allocation Test
	if (clientAddr == NULL || serverAddr == NULL)
		fprintfSwitchable(NULL, 1, "[-server] Memory Allocation for sockadd_in strctures Failed!\n");
	else
		fprintfSwitchable(NULL, 0, "[+server] Memory Allocation Successfully Completed.\n");
	
	bzero(serverAddr, sizeof(struct sockaddr_in)); //memset(serverAddr, 0x00, sizeof(struct sockaddr_in));
	bzero(clientAddr, sizeof(struct sockaddr_in)); //memset(clientAddr, 0x00, sizeof(struct sockaddr_in));
	
	// set-up Server parameters
	serverAddr->sin_family      = AF_INET;
    serverAddr->sin_addr.s_addr = INADDR_ANY;	// inet_addr(serverIP);
	serverAddr->sin_port        = htons(portNumber);

	// server :: Socket Binding
	if (bind(sockfd, (struct sockaddr*) serverAddr, sizeof(struct sockaddr_in)) == -1)
		fprintfSwitchable(NULL, 1, "[-server] Error in Socket Binding!\n");
	else
		fprintfSwitchable(NULL, 0, "[+server] Socket Successfully Binded.\n");
	
	// server :: Socket Listening
	if (listen(sockfd, WaitQueue) == -1)
		fprintfSwitchable(NULL, 1, "[-server] Error while Listening!\n");
	else
		fprintfSwitchable(NULL, 0, "[+server] Listening...\n");
	
	// server :: Socket Listening
	sockfd_new = accept(sockfd, (struct sockaddr*) clientAddr, &socklen);
	if (sockfd_new == -1)
		fprintfSwitchable(NULL, 1, "[-server] Error in Accepting Client Connection!\n");
	else
		fprintfSwitchable(NULL, 0, "[+server] Connection Successfully Accepted.\n");

	// Establishing BUFFER_MAX_SIZE (Set Sender/Client Buffer-Size Equal to Server)
	if (write(sockfd_new, &MAX_CACHE_SIZE, sizeof(size_t)) == -1)
		fprintfSwitchable(NULL, 1, "[-server] Error in Buffer Size Negitiation!\n");
	else 
		fprintfSwitchable(NULL, 0, "[+server] MAX Buffer-Size is Sent to client: %lu.\n", MAX_CACHE_SIZE);
	
	// Initializing Sender Sleep Clock (Set Sender/Client Buffer-Size Equal to Server)
	if (write(sockfd_new, &sleepTimer, sizeof(size_t)) == -1)
		fprintfSwitchable(NULL, 1, "[-server] Error in Sleep Timer Negitiation!\n");
	else 
		fprintfSwitchable(NULL, 0, "[+server] Sleep-Timer is Sent to client: %lu\n", sleepTimer);

	int ACKFLAG = 0, CONFLAG = 0;
	size_t OveralIteration = 0;
	size_t SentBytes;
	size_t Not_Written;
	int iack = 0;
	fprintfSwitchable(NULL, 0, "**********************Server-Side::Recieving Started**************************\n");
	while (1)
	{
		// Exit Condition
		if (isEnd (sleepTimer, "server"))  break;
		// File Status @ Server (Reciever) 
        fileMemory_ACK = FileSizeCalculator(fileLocFull);
		
		// Send ACK Status
        if (write(sockfd_new, &fileMemory_ACK, sizeof(size_t)) == -1)
			fprintfSwitchable(NULL, 1, "[-server] Error in Sending File Info!\n");
		else
			fprintfSwitchable(NULL, 0, "[+server] File (Size) Info Sent Successfully.\n"); 

		// Recieve Expecting Data Size From Client
		if (read(sockfd_new, &fileRemainMemory, sizeof(size_t)) == -1)
			fprintfSwitchable(NULL, 1, "[-server] Failed to Recieve Remaining Memory Info!\n"); 
		else
			fprintfSwitchable(NULL, 0, "[+server] Session [%lu] Overview:\nACK Memory is: %lu\nRemaining Memory is: %lu\n\n", 
                                    OveralIteration, fileMemory_ACK, fileRemainMemory); 
        if (fileRemainMemory == 0) 
		{
			fprintfSwitchable(NULL, 0, "[+server] Remaining Memory is Zero. Process Will Contine After %lu[s] Timeout.\n", sleepTimer); 
			continue;
		}
		else if (fileRemainMemory < BUFFER_MAX)
		{
			fprintfSwitchable(NULL, 0, "[+server] Remaining Buffer is Smaller Than Cache Limit %lu. Recieve Process Will Contine After %lu[s] Timeout.\n",
							  BUFFER_MAX, sleepTimer);  
			Not_Written++;
			if (Not_Written < 10) 
				continue;
			else
				Not_Written = 0;
		}

		// Creating Cache
		CACHE_SIZE = fileRemainMemory;
		cacheBuffer = (char*) calloc (CACHE_SIZE, sizeof(char));
		bzero(cacheBuffer, CACHE_SIZE);

		// Main Send-ACK LOOP
		while (1)
		{
			ACKFLAG = 0;
			SentBytes = 0;
			fileRemainMemory = CACHE_SIZE;
			while (1)
			{
				iack = 0;
				// EXIT CONDITION (Evaluate Buffer Size)
				if (fileRemainMemory == 0) 
				{
					ACKFLAG = 1;
					break;
				}
				else if (fileRemainMemory < BUFFER_MAX) 
					BUFFER_USED = fileRemainMemory;
				else					
					BUFFER_USED = BUFFER_MAX;      
				
				// Incoming Buffer Size
				if (read(sockfd_new, &BUFFER_USED, sizeof(size_t)) == -1)
					fprintfSwitchable(NULL, 1, "[-server] Error While Reading Buffer Size!\n"); 
				else
				{
					//fprintfSwitchable(NULL, 0, "[+server] Buffer Size Successfully Negotiated.\n"); 
					// Recieving Data From Buffer to Server
					if (read(sockfd_new, bufferPtr, BUFFER_USED) == -1)
						fprintfSwitchable(NULL, 1, "[-server] Error While Recieving Buffer!\n"); 
					else
					{
						//fprintfSwitchable(NULL, 0, "[+server] Buffer Recieved Successfully.\n"); 
						// Copy From CACHE to Socket Buffer
						memcpy(cacheBuffer+SentBytes, bufferPtr, BUFFER_USED);
						// In Session Statistics	
						fileRemainMemory -= BUFFER_USED;
						SentBytes 		 += BUFFER_USED;
						//fprintfSwitchable(NULL, 0, "[+server] Sent Bytes is %lu :: Remaining Memory is: %lu\n", SentBytes, fileRemainMemory);
						iack = 1;
						// EXIT CONDITION (Successful Sent)
						if (write(sockfd_new, &iack, sizeof(int)) == -1)	
							fprintfSwitchable(NULL, 1, "[-server] Error While Recieving ACK!\n");
						//else	
							//fprintfSwitchable(NULL, 0, "[+server] ACK Sent.\n");
					}
				}	
			}

			// EXIT CONDITION (Successful Sent)
			if (write(sockfd_new, &ACKFLAG, sizeof(int)) == -1)	
				fprintfSwitchable(NULL, 1, "[-server] Error While Recieving ACK!\n");
			else	
				fprintfSwitchable(NULL, 0, "[+server] ACK Sent.\n");
			
			if (ACKFLAG)
			{
				fileMemory_ACK += SentBytes;
				break;
			} 
			else
				continue;
		}		

		// Copy Data From File to CACHE-Buffer
		filePtr_ACK = fopen(fileLocFull, "a");
        fwrite(cacheBuffer, sizeof(char), CACHE_SIZE, filePtr_ACK);
		fclose(filePtr_ACK);

		// Session Summary
		fprintfSwitchable(NULL, 0, "[+client] Session %lu Summary:\nACK Memory is %lu :: Remaining Memory is: %lu\n", 
                                                OveralIteration, fileMemory_ACK, fileRemainMemory);	
		free(cacheBuffer);
		OveralIteration++;
	}
	fprintfSwitchable(NULL, 0, "\n***********************Server-Side::Recieving Ended***************************\n");
	
	close(sockfd);
	close(sockfd_new);
	free(bufferPtr);
	free(serverAddr);
	free(clientAddr);

	return 0;
}
