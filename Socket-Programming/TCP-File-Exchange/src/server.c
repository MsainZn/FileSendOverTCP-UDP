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
	// Input Parameters Error Check
	if (argc < 5) 
		fprintfSwitchable(NULL, 1, "[+server] Please use the following format to initiate the ./server FILENAME PORT_NUMBER SLEEP_TIMER MAX_BUFFER_SIZER\n");

	// Initial confirmation client side
	char checkSend;
	fprintfIO (NULL, "Start Server [Y/N]: ", 1, 0);
	scanf(" %c", &checkSend);
	if (checkSend == 'N' || checkSend == 'n') 
		fprintfIO (NULL, "Server Abort Process As Requested!\n", 1, 1);

	// Input Parameters  
	const char* fileLocFull = argv[1];
	int portNumber          = atoi(argv[2]); 	     
	size_t sleepTimer       = atoi(argv[3]); 
	size_t BUFFER_MAX       = atoi(argv[4]);
	size_t WaitQueue        = 5;
	
	// server :: Socket Accept Client Request
	socklen_t socklen = sizeof(struct sockaddr_in);

	// File Process, Data Structure (NACK is the Sender/Client Side)
	FILE* filePtr_ACK = fopen(fileLocFull, "r");
	if (filePtr_ACK == NULL) 
		{
			fprintfSwitchable(NULL, 0, "[-server] Not Such File Name Exists To Send!\n");
			filePtr_ACK = fopen(fileLocFull, "w");
		}
	else
	fprintfSwitchable(NULL, 0, "[+server] File Selected For Sending Successfully Found.\n");
	fclose(filePtr_ACK);

	int fileDes_ACK = 0;
    size_t fileMemory_ACK = 0;
    struct stat fileInfo_ACK;
	bzero(&fileInfo_ACK, sizeof(struct stat));
	size_t remainMem = 0;
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
	if (write(sockfd_new, &BUFFER_MAX, sizeof(size_t)) == -1)
		fprintfSwitchable(NULL, 1, "[-server] Error in Buffer Size Negitiation!\n");
	else 
		fprintfSwitchable(NULL, 0, "[+server] MAX Buffer-Size is Sent to client: %lu.\n", BUFFER_MAX);
	
	// Initializing Sender Sleep Clock (Set Sender/Client Buffer-Size Equal to Server)
	if (write(sockfd_new, &sleepTimer, sizeof(size_t)) == -1)
		fprintfSwitchable(NULL, 1, "[-server] Error in Sleep Timer Negitiation!\n");
	else 
		fprintfSwitchable(NULL, 0, "[+server] Sleep-Timer is Sent to client: %lu\n", sleepTimer);

	// Server Buffer
	char* bufferPtr;
	size_t BUFFER_USED;
	
	// Exit Material
	fd_set          s;
    struct timeval  timeout;
    char* exitFlag = (char*) calloc(12, sizeof(char));
    int sFlag;

	int ACKFLAG = 0;
	unsigned sendRecvCounts;
	unsigned long OveralIteration = 0;
	
	fprintfSwitchable(NULL, 0, "**********************Server-Side::Recieving Started**************************\n");
	while (1)
	{
		// Exit Condition
		fflush(stdout);
        FD_ZERO(&s);
        FD_SET(STDIN_FILENO, &s);
        timeout.tv_sec = sleepTimer; timeout.tv_usec = 0;
        sFlag = select(STDIN_FILENO+1, &s, NULL, NULL, &timeout);
        if      (sFlag < 0) exit(0);
        else if (sFlag == 0) 
        {
            fflush(stdout);
            fprintfSwitchable(NULL, 0, "-\n");
        }
        else
        {
            fgets(exitFlag, 12, stdin);
            fprintfSwitchable(NULL, 0, "[+server] To Quit, please enter \"EXIT\": "); 
            fflush(stdout);
            if ( memcmp(exitFlag, "EXIT", 4) == 0 ) break;
        }

		// Sending Session Started
		OveralIteration++;

		// File Status @ Server (Reciever) 
        filePtr_ACK = fopen(fileLocFull, "r");
        fileDes_ACK = fileno(filePtr_ACK);
        fstat(fileDes_ACK, &fileInfo_ACK);
        fileMemory_ACK = fileInfo_ACK.st_size;
        fclose(filePtr_ACK);
		
		// Send ACK Status
        if (write(sockfd_new, &fileInfo_ACK, sizeof(struct stat)) == -1)
			fprintfSwitchable(NULL, 1, "[-server] Error in Sending File Info!\n");
		else
			fprintfSwitchable(NULL, 0, "[+server] File (Size) Info Sent Successfully.\n"); 

		// Recieve Expecting Data Size From Client
		if (read(sockfd_new, &remainMem, sizeof(int)) == -1)
			fprintfSwitchable(NULL, 1, "[-server] Failed to Recieve Remaining Memory Info!\n"); 
		
		// Session Overview
		fprintfSwitchable(NULL, 0, "[+server] Session [%lu] Overview:\nACK Memory is: %lu\nRemaining Memory is: %lu\n\n", 
                                                OveralIteration, fileMemory_ACK, remainMem); 

		// Check The Remaining Size Before Going Further
		//if      (remainMem < 0)
			//fprintfSwitchable(NULL, 1, "[-server] Remain Bytes Cannot be Below Zero!!!\n"); 
        if (remainMem == 0) 
		{
			fprintfSwitchable(NULL, 0, "[+server] Remaining Memory is Zero. Process Will Contine After %lu[s] Timeout.\n", sleepTimer); 
			continue;
		}

		// Recieving Process
		while(1)
		{
			if (remainMem == 0) 
				break;
			else if (remainMem < BUFFER_MAX) 
				BUFFER_USED = remainMem;
       		else					
				BUFFER_USED = BUFFER_MAX; 

			//sleep(5);
			// Sending Data From Buffer to Server
			if (read(sockfd_new, &BUFFER_USED, sizeof(size_t)) == -1)
				fprintfSwitchable(NULL, 1, "[-server] Error While Sending!\n"); 
			
			// Create BUFFER
			fprintfSwitchable(NULL, 0, "[+server] Negotiated Buffer-Size is %lu.\n", BUFFER_USED); 
			bufferPtr = (char*) calloc (BUFFER_USED, sizeof(char));
			bzero(bufferPtr, BUFFER_USED);   

			// Sending Data From Buffer to Server
			if (read(sockfd_new, bufferPtr, BUFFER_USED) == -1)
				fprintfSwitchable(NULL, 1, "[-server] Error While Sending!\n"); 

			fprintfSwitchable(NULL, 0, "[+server] Buffer Recieved Successfully.\n"); 
			
			// Copy Data From File to Buffer
			filePtr_ACK = fopen(fileLocFull, "a");
			//fseek(filePtr_ACK, fileMemory_ACK, SEEK_SET);
			fwrite(bufferPtr, sizeof(char), BUFFER_USED, filePtr_ACK);
			fclose(filePtr_ACK);

			fprintfSwitchable(NULL, 0, "[+server] Data is Successfully Persist In File.\n");  

			// Recv ACK From Server
			if (write(sockfd_new, &ACKFLAG, sizeof(int)) == -1)	
				fprintfSwitchable(NULL, 1, "[-server] Error While Recieving ACK!\n\n"); 

			fprintfSwitchable(NULL, 0, "[+server] Persist ACK Sent to Client.\n");  

			// In Session Statistics
			remainMem      -= BUFFER_USED;
			fileMemory_ACK += BUFFER_USED;	
			
			fprintfSwitchable(NULL, 0, "[+server] ACK Memory is %lu :: Buffer-Used is %lu :: Remaining Memory is: %lu\n", 
                                                fileMemory_ACK, BUFFER_USED, remainMem);		
			free(bufferPtr);
			ACKFLAG = 0;
		}
	
		// Session Summary
		fprintfSwitchable(NULL, 0, "[+client] Session %lu Summary:\n ACK Memory is %lu :: Buffer-Used is %lu :: Remaining Memory is: %lu\n", 
                                                OveralIteration, fileMemory_ACK, BUFFER_USED, remainMem);	
	
	}
	fprintfSwitchable(NULL, 0, "\n***********************Server-Side::Recieving Ended***************************\n");
	
	close(sockfd);
	close(sockfd_new);
	free(serverAddr);
	free(clientAddr);

	return 0;
}
