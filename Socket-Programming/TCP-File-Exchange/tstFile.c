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
	if (argc < 3) 
		fprintfSwitchable(NULL, 1, "[+test] Please use the following format to initiate the ./tstFile LINE_NUMBER SLEEP_TIMER\n");

    int lineNumber  = atoi(argv[1]); 
    int ts          = atoi(argv[2]); 

	FILE* filePtr_ACK;
    filePtr_ACK = fopen("tstFile.txt", "r");
	if (filePtr_ACK == NULL) 
		{
			fprintfSwitchable(NULL, 0, "No File Found!\n");
			filePtr_ACK = fopen("tstFile.txt", "w");
		}
	else
	    fprintfSwitchable(NULL, 0, "Successfully Found.\n");
	fclose(filePtr_ACK);

    fprintfSwitchable(NULL, 0, "Successfully Opened.\n");
    
    
    for (size_t i = 0; i < lineNumber; i++)
    {
        filePtr_ACK = fopen("tstFile.txt", "a");
        fprintfSwitchable(filePtr_ACK, 0,"123456789abcdefghijklm\n");
        sleep(ts);
        fclose(filePtr_ACK);
    }
    
    fprintfSwitchable(NULL, 0, "Process Finished.\n");
	return 0;
}
