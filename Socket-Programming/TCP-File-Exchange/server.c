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
	{	fprintf(stderr, "Please use the following format to initiate the server: ./server PORT_NUMBER\n");
		exit(0);
	}

	// Initial confirmation client side
	char checkSend;
	fprintfIO (NULL, "Start Server [Y/N]: ", 1, 0);
	scanf(" %c", &checkSend);
	if (checkSend == 'N') 
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
			fprintfIO (NULL,"[-server] Not Such File Name Exists To Send!\n", 1, 0);
			filePtr_ACK = fopen(fileLocFull, "w");
		}
	else
		fprintfIO(NULL, "[+server] File Selected For Sending Successfully Found.\n", 1, 0);
	fclose(filePtr_ACK);

	int fileDes_ACK;
    unsigned long fileMemory_ACK = 0;
    struct stat fileInfo_ACK;
	bzero(&fileInfo_ACK, sizeof(struct stat));
	int remainMem = 0;
	// socket End-Point
	int sockfd, sockfd_new;
	// client :: Initiation Socket via TCP Protocol & Error Check
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		fprintfIO(NULL, "[-server] Error in Socket Initiation!\n", 1, 1);
	else
		fprintfIO(NULL, "[+server] Socket Successfully Initiated.\n", 1, 0);

	// Socket main Structures
	struct sockaddr_in* serverAddr;
	struct sockaddr_in* clientAddr;
	serverAddr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));		
	clientAddr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
	
	// Socket Structures Memory Allocation Test
	if (clientAddr == NULL || serverAddr == NULL)
		fprintfIO(NULL, "[-server] Memory Allocation for sockadd_in strctures Failed!\n", 1, 1);
	else
		fprintfIO(NULL, "[+server] Memory Allocation Successfully Completed.\n", 1, 0);
	
	bzero(serverAddr, sizeof(struct sockaddr_in)); //memset(serverAddr, 0x00, sizeof(struct sockaddr_in));
	bzero(clientAddr, sizeof(struct sockaddr_in)); //memset(clientAddr, 0x00, sizeof(struct sockaddr_in));
	
	// set-up Server parameters
	serverAddr->sin_family      = AF_INET;
    serverAddr->sin_addr.s_addr = INADDR_ANY;	// inet_addr(serverIP);
	serverAddr->sin_port        = htons(portNumber);

	// server :: Socket Binding
	if (bind(sockfd, (struct sockaddr*) serverAddr, sizeof(struct sockaddr_in)) == -1)
		fprintfIO(NULL, "[-server] Error in Socket Binding!\n", 1, 1);
	else
		fprintfIO(NULL, "[+server] Socket Successfully Binded.\n", 1, 0);
	
	// server :: Socket Listening
	if (listen(sockfd, WaitQueue) == -1)
		fprintfIO(NULL, "[-server] Error while Listening!\n", 1, 1);
	else
		fprintfIO(NULL, "[+server] Listening...\n", 1, 0);
	
	// server :: Socket Listening
	sockfd_new = accept(sockfd, (struct sockaddr*) clientAddr, &socklen);
	if (sockfd_new == -1)
		fprintfIO(NULL, "[-server] Error in Accepting Client Connection!\n", 1, 1);
	else
		fprintfIO(NULL, "[+server] Connection Successfully Accepted.\n", 1, 0);

	// Establishing BUFFER_MAX_SIZE (Set Sender/Client Buffer-Size Equal to Server)
	if (write(sockfd_new, &BUFFER_MAX, sizeof(size_t)) == -1)
		fprintfIO(NULL, "[-server] Error in Buffer Size Negitiation!\n", 1, 1); 
	else 
		fprintfIO(NULL, "[+server] Buffer-Size is Sent to client.\n", 1, 0);
	
	// Initializing Sender Sleep Clock (Set Sender/Client Buffer-Size Equal to Server)
	if (write(sockfd_new, &sleepTimer, sizeof(size_t)) == -1)
		fprintfIO(NULL, "[-server] Error in Sleep Timer Negitiation!\n", 1, 1); 
	else 
		fprintfIO(NULL, "[+server] Sleep-Timer is Sent to client\n", 1, 0); 

	fprintf(stdout, "Sleep Timer: %lu\t Buffer Size: %lu\n", sleepTimer, BUFFER_MAX);

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
	fprintfIO(NULL, "**********************Server-Side::Recieving Started**************************\n", 1, 0);
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
            printf("-\n");
        }
        else
        {
            fgets(exitFlag, 12, stdin);
            fprintfIO(NULL, "[+server] To Quit, please enter \"EXIT\": ", 1, 0); 
            fflush(stdout);
            if ( memcmp(exitFlag, "EXIT", 4) == 0 ) break;
        }

		// Sending Session Started
		OveralIteration++;
		fprintf(stdout, "[+server] Session %lu Started:\n", OveralIteration);
		// File Status @ Server (Reciever) 
        filePtr_ACK = fopen(fileLocFull, "r");
        fileDes_ACK = fileno(filePtr_ACK);
        fstat(fileDes_ACK, &fileInfo_ACK);
        fileMemory_ACK = fileInfo_ACK.st_size;
        fclose(filePtr_ACK);
		
		// Send ACK Status
        if (write(sockfd_new, &fileInfo_ACK, sizeof(struct stat)) == -1)
			fprintfIO(NULL, "[-server] Error in Sending File Info!\n", 1, 1); 
		fprintfIO(NULL, "[+server] File Info (size) Sent Successfully\n", 1, 0); 

		// Recieve Expecting Data Size From Client
		if (read(sockfd_new, &remainMem, sizeof(int)) == -1)
			fprintfIO(NULL, "[-server] Failed To send Remaining Memory Info!\n", 1, 1); 
		fprintfIO(NULL, "[+server] Remained Size is Recieved Successfully.\n", 1, 0); 

		// Session Overview
		fprintf(stdout, "[+server] Total ACK Memory: %lu :: Total Remained Memory is: %d\n", fileMemory_ACK, remainMem);

		// Check The Remaining Size Before Going Further
		if      (remainMem < 0)
			fprintfIO(NULL, "[-server] Remain Bytes Cannot be Below Zero!!!\n", 1, 1); 
        else if (remainMem == 0) 
		{
			fprintfIO(NULL, "[+server] Remaining Memory is Zero. Process Will Contine After A Short Timeout\n", 1, 0); 
			continue;
		}

		// Recieving Process
		while(remainMem)
		{
			// Sending Data From Buffer to Server
			while (read(sockfd_new, &BUFFER_USED, sizeof(int)) == -1)
				fprintfIO(NULL, "[-server] Error While Sending!\n", 1, 0); 
			fprintf(stdout, "[+server] Negotiated Buffer Size is: %lu.\n", BUFFER_USED); 

			// Create BUFFER
			bufferPtr = (char*) calloc (BUFFER_USED, sizeof(char));
			bzero(bufferPtr, BUFFER_USED);   

			// Sending Data From Buffer to Server
			while (read(sockfd_new, bufferPtr, BUFFER_USED) == -1)
			{
				bzero(bufferPtr, BUFFER_USED); 
				fprintfIO(NULL, "[-server] Error While Sending!\n", 1, 0); 
			}
				
			// Copy Data From File to Buffer
			filePtr_ACK = fopen(fileLocFull, "a");
			fseek(filePtr_ACK, fileMemory_ACK, SEEK_SET);
			fwrite(bufferPtr, sizeof(char), BUFFER_USED, filePtr_ACK);
			fclose(filePtr_ACK);

			// Recv ACK From Server
			while (write(sockfd_new, &ACKFLAG, sizeof(int)) == -1)	
				fprintfIO(NULL, "[-server] Error While Recieving ACK !\n", 1, 0); 

			// In Session Statistics
			remainMem      -= BUFFER_USED;
			fileMemory_ACK += BUFFER_USED;
			fprintf(stdout, "[+server] Send Size: %lu :: Remaining Memory is: %d\n", BUFFER_USED, remainMem);
			free(bufferPtr);
			ACKFLAG = 0;
		}
	
		// Session Summary
		fprintf(stdout, "[+server] Session %lu Summary:\nACK Memory is: %lu\nRemaining Memory is: %d\n\n", 
                                                OveralIteration, fileMemory_ACK, remainMem);		
	
	}
	fprintfIO(NULL, "***********************Server-Side::Recieving Ended***************************\n", 1, 0);
	
	close(sockfd);
	close(sockfd_new);
	free(serverAddr);
	free(clientAddr);

	return 0;
}
