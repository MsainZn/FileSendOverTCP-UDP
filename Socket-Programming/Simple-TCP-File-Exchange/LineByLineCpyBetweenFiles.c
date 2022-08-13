#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main ()
{
    FILE* file1 = fopen("bin/mainFile.log", "r");

    FILE* file2 = fopen("bin/mainFile223.log", "a");

    size_t BUFFER_MAX = 1024;

	char* bufferPtr   = (char*) calloc (BUFFER_MAX, sizeof(char));

    while (fgets(bufferPtr, BUFFER_MAX, file1))
    {
        fprintf(file2, "%s", bufferPtr);
        fprintf(stdout, "%s", bufferPtr);
        bzero(bufferPtr, BUFFER_MAX);
        //sleep(5);
    }

    fclose(file1);
    
    fclose(file2);
    

    return 0;
}


/*


    




*/