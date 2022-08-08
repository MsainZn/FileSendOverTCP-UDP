// Standard C functions 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Standard socket functions & data structures (sockaddr & sockaddr_in)
#include <arpa/inet.h>
// A header file that provides access to the POSIX operating system API (READ, WRITE, LINK, SYNC, ...).
#include <unistd.h>

// server information (if required!!!)
//extern struct sockaddr_in* serverAddr;
//extern int serverBufferSize;
//extern int sockfd_server;

int main (int argc, char* argv[])
{
	// Main Parameters
	int clientBufferSize = 1024;
    int clientBufferRecv = 0;
    char* mssg = (char*) calloc (clientBufferSize, sizeof(char));

	// Error Flags
	int connectFlag;

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
		printf("[+client] Socket Successfully Initiated via Socket Number: %d .\n", sockfd_client);
	}

    // server info for connect
	struct sockaddr_in* serverAddr = 
						(struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
	
	serverAddr->sin_family      = AF_INET;
	//serverAddr->sin_port        = htons(atoi(argv[2]));
	//serverAddr->sin_addr.s_addr = inet_addr(argv[1]);
    serverAddr->sin_port        = htons(9002);
	serverAddr->sin_addr.s_addr = INADDR_ANY;


    // client :: Socket Initiation Via TCP Protocol
	connectFlag = connect(sockfd_client, (struct sockaddr*) serverAddr, sizeof(struct sockaddr_in));
    if (sockfd_client == -1)
	{
		perror("[-client] Error in Socket connection!\n");
		exit(-1);
	}

    // recieve data from server
    clientBufferRecv = recv(sockfd_client, mssg, clientBufferSize, 0);

    if (clientBufferRecv != clientBufferSize)
    {
        printf("[-client] Message is Recieved incomplete [Message-Recieve = %d , Message-Size = %d]\n", clientBufferRecv, clientBufferSize);
		exit(-1);
    }
    else
    {
        printf("[+client] Message Recieved Successfully:\n %s\n", mssg);
    }
    
    close(sockfd_client);

	return 0;
}