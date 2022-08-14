#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main ()
{
    FILE* file1 = fopen("bin/mainFile.log", "r");
    FILE* logFile = fopen("bin/logFile", "wa");

    fprintf (logFile, "DC-Counts\tLine-Sent\tBytes-Sent\tServer-IP\tServer-Port\tSource-File-Name\n");

    size_t BUFFER_MAX = 1024;
	char* bufferPtr   = (char*) calloc (BUFFER_MAX, sizeof(char));
    
    unsigned long fileLineCounter = 0;

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

    fprintf(stdout, "\n%lu\n", fileLineCounter);

    
    fclose(file1);
    
    return 0;
}


/*


    




*/