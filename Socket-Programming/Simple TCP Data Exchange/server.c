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

		int num1, num2, answer;
		char operationChoice;

		printf("\n---------------------Server-Side::Connection Established Per Request----------------------.\n\n");	
		while (1)
		{
			if (write(sockfd_new, "Enter Number (1): ", strlen("Enter Number (1): ") == -1))
				CheckError("Error While Writting (Sending)!\n");
			if (read(sockfd_new, &num1, sizeof(int)) == -1)
					printf("Error While Getting info from client (Recieving)!\n");

			if (write(sockfd_new, "Enter Number (2): ", strlen("Enter Number (2): ") == -1))
				CheckError("Error While Writting (Sending)!\n");
			if (read(sockfd_new, &num2, sizeof(int)) == -1)
					printf("Error While Getting info from client (Recieving)!\n");

			do
			{	operationChoice = 'N';
				if (write(sockfd_new, "Enter Your Operation choice oe E to exit: ", strlen("Enter Your Operation choice oe E to exit: ") == -1))
					CheckError("Error While Writting (Sending)!\n");
				if (write(sockfd_new, &operationChoice, sizeof(char)))
					CheckError("Error While Writting (Sending)!\n");
				if (read(sockfd_new, &operationChoice, sizeof(char)) == -1)
					printf("Error While Getting info from client (Recieving)!\n");

				switch (operationChoice)
				{
					case '+': answer = num1 + num2; operationChoice = 'K'; break;
					case '-': answer = num1 - num2; operationChoice = 'K';break;
					case '*': answer = num1 * num2; operationChoice = 'K';break;
					case '/': 
						if (num2==0) answer = 0;
						else answer =num1 / num2; 
						break;
					case '^': answer = num1 ^ num2; operationChoice = 'K';break;
					case '%': answer = num1 % num2; operationChoice = 'K';break;
					default:  operationChoice = 'N'; break;
				}
				
			} while ( operationChoice == 'N');

			if (write(sockfd_new, &answer, sizeof(int)))
				CheckError("Error While Writting (Sending)!\n");
		}
		printf("\n---------------------Server-Side::Connection Terminated Per Request----------------------.\n");

		printf("Enter TURN-OFF to Turn Server OFF: ");
			fgets(bufferPtr, BUFFER_MAX, stdin);
			if (strncmp("TURN-EOF", bufferPtr, 10) == 0) break;
			bzero(bufferPtr, BUFFER_MAX); 
	}
	printf("\n---------------------Server Shut-Down Per Request----------------------.\n");

	close(sockfd);
	close(sockfd_new);

	return 0;
}