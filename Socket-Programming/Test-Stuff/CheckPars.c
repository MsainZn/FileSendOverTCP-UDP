#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

unsigned long FileLineCounter(const char *LogFileLocation)
{
    FILE* file = fopen(LogFileLocation, "r");
    size_t BUFFER_SIZE = 1024;
    char *buffer = (char*) calloc(BUFFER_SIZE, sizeof(char));
    long unsigned LC = 0;
    while (!feof(file))
    {
        fgets(buffer, 1024, file);
        ftell(file);
        LC++;
    }
    free(buffer);
    fclose(file);
    return LC;
}

void Parser (const char *LogFileLocation, const char *delimiter, int *DCCount, unsigned long *lineSent, int *byteSent, char* serverIP, int* serverPort, char* FileName)
{  
    FILE* file = fopen(LogFileLocation, "r");
    
    // Number of Lines in LogFile
    unsigned long LC = FileLineCounter(LogFileLocation);
    // Iterator of File
    size_t BUFFER_SIZE = 1024;
    char *buffer = (char*) calloc(BUFFER_SIZE, sizeof(char));
    bzero(buffer, BUFFER_SIZE);
  
    int i = 0;
    while (i < LC-2)
    {
        i++;
        bzero(buffer, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, file);
        ftell(file);
    }

    fclose(file);
    printf("%s", buffer);



}



int main ()
{
    
   
    Parser ("bin/logFile", NULL, NULL, NULL, NULL, NULL, NULL, NULL);
   
 /*  
    FILE* file1 = fopen("bin/logFile", "r");
    FILE* logFile = fopen("bin/logFile", "wa");

    fprintf (logFile, "DC-Counts\tLine-Sent\tBytes-Sent\tServer-IP\tServer-Port\tSource-File-Name\n");

    size_t BUFFER_MAX = 1024;
	char* bufferPtr   = (char*) calloc (BUFFER_MAX, sizeof(char));
    
    unsigned fileLineCounter = 0;

    while (fileLineCounter<20)
    {
        fileLineCounter++;
        printf("%lu\t", ftell(file1));
        fgets(bufferPtr, BUFFER_MAX, file1);
        fprintf(stdout, "%s", bufferPtr);
        bzero(bufferPtr, BUFFER_MAX);
        fprintf (logFile, "%d\t%u\t%lu\t%s\t%d\t%s\n", 0, fileLineCounter, ftell(file1), "172.17.34.89", 5500, "bin/mainFile.log");
    }

    // Virtual disconnection
    fseek(file1, ftell(file1), SEEK_SET);
    fprintf(stdout, "\n\nDISCONNECTION!!!!\n\n");

    while (!feof(file1))
    {
        fileLineCounter++;
        printf("%lu\t", ftell(file1));
        fgets(bufferPtr, BUFFER_MAX, file1);
        fprintf(stdout, "%s", bufferPtr);
        bzero(bufferPtr, BUFFER_MAX);
        fprintf (logFile, "%d\t%u\t%lu\t%s\t%d\t%s\n", 1, fileLineCounter, ftell(file1), "172.17.34.89", 5500, "bin/mainFile.log");
    }

    
    fclose(file1);
    */
    return 0;
}


/*


    




*/