#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main ()
{
    FILE* file1 = fopen("bin/mainFile.log", "r");
    //FILE* logFile = fopen("bin/logFile", "wa");

    //fprintf (logFile, "DC-Counts \t Line-Sent \t Bytes-Sent \t Server-IP \t Server-Port \t Source-File-Name \n");

    size_t BUFFER_MAX = 1024;
	char* bufferPtr   = (char*) calloc (BUFFER_MAX, sizeof(char));
    
    
    //
    unsigned fileLineCounter = 0;

    //fprintf (logFile, "%d %u %lu %d %d %s", 0, fileLineCounter, ftell(file1), 1, 5500, "bin/mainFile.log");
    //
    while (fileLineCounter<20)
    {
        fileLineCounter++;
        printf("%lu\t", ftell(file1));
        fgets(bufferPtr, BUFFER_MAX, file1);
        fprintf(stdout, "%s", bufferPtr);
        bzero(bufferPtr, BUFFER_MAX);
        //fprintf (logFile, "%d %u %lu %d %d %s\n", 0, fileLineCounter, ftell(file1), 1, 5500, "bin/mainFile.log");
        //sleep(5);
    }

    //fscanf();


    fseek(file1, ftell(file1), SEEK_SET);
    fprintf(stdout, "\n\nDISCONNECTION!!!!\n\n");
    

    while (!feof(file1))
    {
        fileLineCounter++;
        printf("%lu\t", ftell(file1));
        fgets(bufferPtr, BUFFER_MAX, file1);
        fprintf(stdout, "%s", bufferPtr);
        bzero(bufferPtr, BUFFER_MAX);
        //fprintf (logFile, "%d %u %lu %d %d %s\n", 0, fileLineCounter, ftell(file1), 1, 5500, "bin/mainFile.log");
        //sleep(5);
    }

    
    fclose(file1);
    
    return 0;
}


/*


    




*/