#include "utilityFunctionsMHZ.h"



int main ()
{
    const char* fileLocFull = "log";

    struct bufferTCP* mssg = CreateBufferTCP();

    size_t CACHE_SIZE = FileSizeCalculator(fileLocFull);

	char* cacheBuffer = (char*) calloc (CACHE_SIZE, sizeof(char));
	bzero(cacheBuffer, CACHE_SIZE); 

    size_t fileRemainMemory = CACHE_SIZE;

    size_t SentBytes = 0, fileMemory_ACK = 0;
    int ACKFLAG = 0;
    while (1)
    {
        ACKFLAG = 0;
        SentBytes = 0;
        fileRemainMemory = CACHE_SIZE;
        
        while (1)
        {
            // EXIT CONDITION (Evaluate Buffer Size)
            if 	(fileRemainMemory == 0) 
            {
                ACKFLAG = 1;
                break;
            }    
            else if (fileRemainMemory < BUFFER_MAX) 
                mssg->BUFFER_USED = fileRemainMemory;
            else					
                mssg->BUFFER_USED = BUFFER_MAX;

            memcpy(mssg->mssgPtr, cacheBuffer+SentBytes, mssg->BUFFER_USED);
            memcpy(cacheBuffer+SentBytes, mssg->mssgPtr, mssg->BUFFER_USED);

            // In Session Statistics	
            fileRemainMemory -= mssg->BUFFER_USED;
            SentBytes		 += mssg->BUFFER_USED;
            fprintfSwitchable(NULL, 0, "[+client] (%lu) Sent | (%lu) Remaining.\n", SentBytes, fileRemainMemory);
        }

        if (ACKFLAG)
			{
				fileMemory_ACK += SentBytes;
				//break;
			} 
			else
				continue;

    }

    return 0;
}
