// Standard C functions 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Standard socket functions & data structures (sockaddr & sockaddr_in)
#include <arpa/inet.h>

//extern struct sockaddr_in* clientAddr;
//extern int clientBufferSize;
// extern int sockfd_client;

int main (int argc, char* argv[])
{
	// Main Parameters
	int serverBufferSize = 1024;
    char* mssg = "Hey you have reached the server, good job";
	int serverBufferSend = 0;

	// Main Server Parameters
	socklen_t socklen = sizeof(struct sockaddr_in);
	int maxClientWaitingQueue = 5; 

	// LATER USE
	// char* serverIP = ACTUAL ADDRESS :: "127.0.0.1";
	// int serverPort = ACTUAL PORT :: 8080;
	
	// Error Flags
	int bindFlag, listenFlag;

	// server & clinet :: End-Point
	int sockfd_server, sockfd_client;


	// client :: Socket Initiation Via TCP Protocol
	sockfd_server = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd_server == -1)
	{
		perror("[-server] Error in Socket Initiation!\n");
		exit(-1);
	}
	else
	{
		printf("[+server] Socket Successfully Initiated via Socket Number: %d .\n", sockfd_server);
	}


	struct sockaddr_in* serverAddr = 
						(struct sockaddr_in*) malloc(socklen);
	
	serverAddr->sin_family        = AF_INET;
	//serverAddr->sin_port        = ACTUAL PORT 80
	//serverAddr->sin_addr.s_addr = inet_addr(ACTUAL ADDRESS) "10.0.0.1"
    serverAddr->sin_port          = htons(9002);
	serverAddr->sin_addr.s_addr   = INADDR_ANY; // current IP address
	memset(serverAddr->sin_zero, '\0', 8);

	
	// server :: Socket Binding
	bindFlag = bind(sockfd_server, (struct sockaddr*) serverAddr, socklen);
	if (bindFlag == -1)
	{
		perror("[-server] Error in Socket Binding!\n");
		exit(-1);
	}
	else
	{
		printf("[+server] Socket Successfully Binded.\n");
	}

	// server :: Socket Listening
	listenFlag = listen(sockfd_server, maxClientWaitingQueue);
	if (listenFlag == -1)
	{
		perror("[-server] Error while Listening!\n");
		exit(-1);
	}
	else
	{
		printf("[+server] Listening...\n");
	}

	
	// server :: Socket Accept Client Request
	//sockfd_client = accept(sockfd_server, (struct sockaddr*) clientAddr, &socklen);
	sockfd_client = accept(sockfd_server, NULL, &socklen);
	
	// server :: send data to client
	serverBufferSend = send(sockfd_client, mssg, serverBufferSize, 0);
	if (serverBufferSend != serverBufferSize)
    {
        printf("[-server] Message is sent incomplete [Message-Send = %d , Message-Size = %d]\n", serverBufferSend, serverBufferSize);
		exit(-1);
    }
    else
    {
        printf("[+server] Message Sent Successfully:\n %s\n", mssg);
    }
    
    close(sockfd_server);

	return 0;
}