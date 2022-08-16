#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <errno.h> 

// 
int main (int argc, char* argv[])
{  
    if (argc < 3) 
    {
        fprintf(stderr, "Not Enough Inputs!\nPlease ise the following: ./FileOperation fileToSendName fileToRecieveName\n");
        exit(0);
    }
    
    // checks whether program should proceed
    char CheckWork = 'Y';

    // File Name Getting From User
    const char* sendFileName = argv[1];
    const char* recvFileName = argv[2];

    // File Pointers
    FILE* sendFileptr;
    FILE* recvFileptr;
      
    // Check Sender Existance
    sendFileptr = fopen(sendFileName, "r");
    if (sendFileptr == NULL) 
    {
        fprintf(stderr, "Not Such File Name Exists To Send!\n");
        exit(0);
    }
    fclose(sendFileptr);
    

    // Check Reciuver Existance
    recvFileptr = fopen(recvFileName, "r");
    if (recvFileptr == NULL) 
    {
        fprintf(stderr, "Not Such File Name Exists, create a New File\n");
        recvFileptr = fopen(recvFileName, "w");
    }
    fclose(recvFileptr);


    // File Descriptors (FILE*)
    int sendDesc, recvDesc;

    // File Descriptors (FILE*)
    struct stat sendFileInfo;
    struct stat recieveFileInfo;

    // File Memory Info
    unsigned long sendFileMem = 0, recvFileMem = 0;
    int remainMem = 0;

    // Buffers
    char* sendBuffer;
    char* recvBuffer;

    // Buffer Size
    size_t BUFFER_MAX = 4096;
    size_t sendBufferSize, recvBufferSize;
    
    while (1)
    {      
        // Exit Checks
        //fprintf(stdout, "\n-------------------------------------------\n");
        sleep(10); 
        //fprintf(stdout, "Press N to Get out: ");
        //scanf(" %c", &CheckWork);
        //if (CheckWork=='N') break;
        //fprintf(stdout, "\n-------------------------------------------\n");

        // File Status @ Sender 
        sendFileptr = fopen(sendFileName, "r");
        sendDesc    = fileno(sendFileptr);
        fstat(sendDesc, &sendFileInfo);
        sendFileMem = sendFileInfo.st_size;
        fclose(sendFileptr);
        
        // File Status @ Reciever 
        recvFileptr = fopen(recvFileName, "r");
        recvDesc    = fileno(recvFileptr);
        fstat(recvDesc, &recieveFileInfo);
        recvFileMem = recieveFileInfo.st_size;
        fclose(recvFileptr);

        // Send-Recieve Validity Checks
        remainMem = sendFileMem - recvFileMem;
        if (remainMem < 0)
        {
            fprintf(stdout, "remain Bytes cannot be Below Zero!!!\n");
            continue;//break;
        }
        // Remaining Status & Send Configuration
        if (remainMem == 0)
        {
            // Send Statistics
            fprintf(stdout, "Recieved Memory is: %lu\nRemaining Memory is: %d\nSend Memory is: %lu\nMax Buffer-Size is: %lu\n", 
                                                recvFileMem, remainMem, sendBufferSize, BUFFER_MAX);
            break;
        }        
        else if (remainMem < BUFFER_MAX) sendBufferSize = remainMem;
        else                             sendBufferSize = BUFFER_MAX;  

        sendBuffer = (char*) calloc(sendBufferSize, sizeof(char));  
        
        // Send Statistics
        fprintf(stdout, "Recieved Memory is: %lu\nRemaining Memory is: %d\nSend Memory is: %lu\nMax Buffer-Size is: %lu\n", 
                                                recvFileMem, remainMem, sendBufferSize, BUFFER_MAX);

        // Sending & closing < NEED CONFIRMATION FROM RECVEIVER LATER >
        sendBuffer = (char*) calloc(sendBufferSize, sizeof(char));
        sendFileptr = fopen(sendFileName, "r");
        bzero(sendBuffer, sendBufferSize);
        fseek(sendFileptr, recvFileMem, SEEK_SET);
        fread(sendBuffer, sizeof(char), sendBufferSize, sendFileptr);
        //fprintf(stdout, "%s\n", sendBuffer);//puts(sendBuffer);
        fclose(sendFileptr);
        
        fprintf(stdout, "\n-----------------------------------------------------------------------------------------------\n");
        
        // Recieving & closing < NEED CONFIRMATION TO SENDER LATER >
        recvBuffer = (char*) calloc(BUFFER_MAX, sizeof(char));
        bzero(recvBuffer, BUFFER_MAX);
        
        memcpy(recvBuffer, sendBuffer, sendBufferSize);
        recvFileptr = fopen(recvFileName, "a");
        fseek(recvFileptr, recvFileMem, SEEK_SET);
        fwrite(recvBuffer, sizeof(char), sendBufferSize, recvFileptr);
        fclose(recvFileptr);
        //fprintf(stdout, "%s\n", recvBuffer);//puts(recvBuffer);

        // Free BUffer Memory
        free(recvBuffer);
        free(sendBuffer);
        
    }

            
    return 0;
}


/*
    gets(str) == scanf("%[^\n]s", str);
    %[^\n] :: supports whitespace and terminates with newline (\n))

    use fflush(stdin) after getting contents from TUI


*/