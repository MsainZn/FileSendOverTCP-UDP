#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main ()
{
    FILE* file1 = fopen("bin/mainFile.log", "r");

    size_t BUFFER_MAX = 1024;

	char* bufferPtr   = (char*) calloc (BUFFER_MAX, sizeof(char));
    unsigned fileLineCounter = 0;
    
    fseek(file1, 339, SEEK_SET);
    while (!feof(file1))
    {
        fileLineCounter++;
        printf("%lu\t", ftell(file1));
        fgets(bufferPtr, BUFFER_MAX, file1);
        fprintf(stdout, "%s", bufferPtr);
        bzero(bufferPtr, BUFFER_MAX);
        //sleep(5);
    }

    printf("\n%ld\n", fileLineCounter);
    
    fclose(file1);
    
    return 0;
}


/*


    




*/