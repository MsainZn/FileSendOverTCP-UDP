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
	if (argc < 4) 
		fprintfSwitchable(NULL, 1, "[-client] Please use the given format: ./client SERVER_ADDR PORT_NUMBER\n");

	// Initial confirmation client side
	char checkSend;
	fprintfSwitchable(NULL, 0, "[-client] Start Sending File [Y/N]: ");
	scanf(" %c", &checkSend);
	if (checkSend == 'N') 
		fprintfSwitchable(NULL, 1, "[-client] Abort Process As Requested!\n");

	// Input Parameters  
	const char* fileLocFull = argv[1];
	char* serverIP 			= argv[2];
	int portNumber   		= atoi(argv[3]);

	// File Process, Data Structure (NACK is the Sender/Client Side)
	FILE* filePtr_NACK;
	int   fileDes_NACK;
    unsigned long fileMemory_NACK = 0;
    struct stat fileInfo_NACK;
	bzero(&fileInfo_NACK, sizeof(struct stat));
	
	// ACK is the Reciever Side/Server
	struct stat fileInfo_ACK;
	unsigned long fileMemory_ACK = 0;
	int remainMem = 0;

	// Checks if The Target Send File Exists!
	filePtr_NACK = fopen(fileLocFull, "r");
	if (filePtr_NACK == NULL)
		fprintfSwitchable(NULL, 1, "[-client] Not Such File Name Exists To Send!\n");
	else
		fprintfSwitchable(NULL, 0, "[+client] File Selected For Sending Successfully Found.\n");
	fclose(filePtr_NACK);

	// Create Socket via TCP Protocol & Error Check
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		fprintfSwitchable(NULL, 1, "[-client] Error in Socket Initiation!\n");
	else
		fprintfSwitchable(NULL, 0, "[+client] Socket Successfully Initiated.\n");
	// Socket main Structures
	struct sockaddr_in* serverAddr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
	
	// Socket Structures Memory Allocation Test
	if (serverAddr == NULL) 
		fprintfSwitchable(NULL, 1, "[-client] Memory Allocation for sockadd_in strctures Failed!\n");
	else
		fprintfSwitchable(NULL, 0, "[+client] Memory Allocation for sockadd_in strctures Successfully Completed.\n");
	
	// Set-up server parameters
	bzero(serverAddr, sizeof(struct sockaddr_in));   //memset(serverAddr, 0x00, sizeof(struct sockaddr_in));
	serverAddr->sin_family      = AF_INET;
	serverAddr->sin_addr.s_addr = inet_addr(serverIP);
	serverAddr->sin_port        = htons(portNumber);

	// Connection Request to Server
	if (connect(sockfd, (struct sockaddr*) serverAddr, sizeof(struct sockaddr_in)) == -1)
		fprintfSwitchable(NULL, 1, "[-client] Error in Connecting to Server!\n");
	else
		fprintfSwitchable(NULL, 0, "[+client] Connection Successfully Accepted.\n");

	// Buffer Properties and Size (Set Sender/Client Buffer-Size Equal to Server)
	char* bufferPtr;
	size_t BUFFER_MAX, BUFFER_USED;
	if (read(sockfd, &BUFFER_MAX, sizeof(size_t)) == -1)
		fprintfSwitchable(NULL, 1, "[-client] Error in MAX Buffer Size Negitiation!\n"); 
	else 
		fprintfSwitchable(NULL, 0, "[+client] Buffer Size is Set Equal to Server: %lu.\n", BUFFER_MAX);
	
	// Initializing Sender Sleep Clock
	size_t sleepTimer;
	if (read(sockfd, &sleepTimer, sizeof(size_t)) == -1)
		fprintfSwitchable(NULL, 1, "[-client] Error in Sleep Timer Negitiation!\n", 1, 1); 
	else 
		fprintfSwitchable(NULL, 0, "[+client] Sleep Timer is Set Equal to Server: %lu\n", sleepTimer); 

	// Exit Material
	fd_set          s;
    struct timeval  timeout;
    char* exitFlag = (char*) calloc(12, sizeof(char));
    int sFlag;

	int ACKFLAG = 0;
	unsigned sendRecvCounts;
	unsigned long OveralIteration = 0;
	fprintfSwitchable(NULL, 0, "**********************Client-Side::Sending Started**************************\n"); 
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
            fprintfSwitchable(NULL, 0,"[+client] To Quit, please enter \"EXIT\": "); 
            fflush(stdout);
            if ( memcmp(exitFlag, "EXIT", 4) == 0 ) break;
        }

		// Sending Session Started
		OveralIteration++;

		// File Status @ Client (Sender)
        filePtr_NACK = fopen(fileLocFull, "r");
        fileDes_NACK = fileno(filePtr_NACK);
        fstat(fileDes_NACK, &fileInfo_NACK);
        fileMemory_NACK = fileInfo_NACK.st_size;
        fclose(filePtr_NACK);
        
        // File Status @ Server (Reciever) 
        if (read(sockfd, &fileInfo_ACK, sizeof(struct stat)) == -1)
			fprintfSwitchable(NULL, 1, "[-client] Error in reading File Info!\n"); 
		fileMemory_ACK = fileInfo_ACK.st_size;
		fprintfSwitchable(NULL, 0, "[+client] ACK Memory is Recieved Sucesfully: %lu.\n", fileMemory_ACK); 

		// Send-Recieve Validity
        remainMem = fileMemory_NACK - fileMemory_ACK;
			
		// Send Expecting Data Size For Server
		if (write(sockfd, &remainMem, sizeof(int)) == -1)
			fprintfSwitchable(NULL, 1, "[-client] Failed To send Remaining Memory Info!\n"); 

		// Session Overview
		fprintfSwitchable(NULL, 0, "[+client] Session [%lu] Overview:\nACK Memory is: %lu\nNACK Memory is: %lu\nRemaining Memory is: %d\n", 
                                                OveralIteration, fileMemory_ACK, fileMemory_NACK, remainMem); 

		// Check The Remaining Size Before Going Further
		if      (remainMem < 0)
			fprintfSwitchable(NULL, 1, "[-client] Remain Bytes Cannot be Below Zero!!!\n"); 
        else if (remainMem == 0) 
		{
			fprintfSwitchable(NULL, 0, "[+client] Remaining Memory is Zero. Process Will Contine After %lu[s] Timeout.\n", sleepTimer);  
			continue;
		}

		// Sending Process
		while (remainMem)
		{
			// Evaluate Buffer Size
			if (remainMem < BUFFER_MAX) BUFFER_USED = remainMem;
       		else 						BUFFER_USED = BUFFER_MAX;        
			
			// Sending Data From Buffer to Server
			while (write(sockfd, &BUFFER_USED, sizeof(int)) == -1)
				fprintfSwitchable(NULL, 1, "[-client] Error While Sending!\n"); 
				
			// Create BUFFER
			fprintfSwitchable(NULL, 0, "[+client] Buffer Used is: %lu\n", BUFFER_USED); 
			bufferPtr = (char*) calloc (BUFFER_USED, sizeof(char));
			bzero(bufferPtr, BUFFER_USED);   

			// Copy Data From File to Buffer
			filePtr_NACK = fopen(fileLocFull, "r");
        	fseek(filePtr_NACK, fileMemory_ACK, SEEK_SET);
        	fread(bufferPtr, sizeof(char), BUFFER_USED, filePtr_NACK);
			fclose(filePtr_NACK);
			
			// Sending Data From Buffer to Server
			while (write(sockfd, bufferPtr, BUFFER_USED) == -1)
				fprintfSwitchable(NULL, 1, "[-client] Error While Sending!\n"); 
			
			fprintfSwitchable(NULL, 0, "[+client] Data Sent Successfully.\n"); 
			
			// Recv ACK From Server
			while (read(sockfd, &ACKFLAG, sizeof(int)) == -1)	
			{
				bzero(bufferPtr, BUFFER_USED); 
				fprintfSwitchable(NULL, 0, "[-client] Error While Recieving ACK !\n"); 
			}
			fprintfSwitchable(NULL, 0, "[+client] ACK Recieved.\n");  

			// In Session Statistics	
			remainMem      -= BUFFER_USED;
			fileMemory_ACK += BUFFER_USED;
			fprintfSwitchable(NULL, 0, "[+client] Send Counts: %d :: Remaining Memory is: %d\n", remainMem, sendRecvCounts);
			free(bufferPtr);
			ACKFLAG = 0;
		}

		// Session Summary
		fprintfSwitchable(NULL, 0, "[+client] Session %lu Summary:\nACK Memory is: %lu\nRemaining Memory is: %d\n", 
                                                OveralIteration, fileMemory_ACK, remainMem);		
	
	}
	fprintfSwitchable(NULL, 0, "***********************Client-Side::Sending Ended***************************\n");	

    close(sockfd);
	free(serverAddr);
	free(exitFlag);

	return 0;
}

