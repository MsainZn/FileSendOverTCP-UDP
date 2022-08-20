#ifndef utilityFunctionsMHZ_H
#define utilityFunctionsMHZ_H

#include <stdio.h>      //:: standard IO c-functions
#include <string.h>     //:: standard c-string functions
#include <unistd.h>     //:: standard POSIX operating system API (read/write/send/recv)
#include <stdlib.h>     //:: standard ALLOC c-functions (malloc, atoi)
#include <sys/types.h>  //:: definitions of data types used in sys/socket & netinet/in
#include <sys/socket.h> //:: definitions of structures needed for internet domain such as (struct sockaddr)
#include <netinet/in.h> //:: definitions of (struct sockaddr_in)
#include <netdb.h>      //:: definitions of (struct hostnet) to store info of a given host & IPV-4
#include <arpa/inet.h>  //:: inet()
#include <sys/stat.h>
#include <stdarg.h>     //:: Variable Input function

// Usefull functions

void WriteLog(const char* logFileName, const char* mssg)
{
    FILE* logFilePtr = fopen(logFileName, "a");
    //fputs(mssg, logFilePtr);
    fclose(logFilePtr);
}

void fprintfIO(const char* logFileName, const char* mssg, int printConsoleFlag, int errFlag)
{
    if (logFileName != NULL)
        WriteLog(logFileName, mssg);

    if (printConsoleFlag) 
        fprintf(stdout, "%s", mssg);

    if (errFlag)  
        exit(1);
}


void fprintfSwitchable(FILE* stream, int errFlag, const char* mssg, ...)
{
    va_list args;

    va_start(args, mssg);

    if (stream == NULL)
        vfprintf(stdout, mssg, args);
    else
        vfprintf(stream, mssg, args);

    if (errFlag)  exit(1);
        
    va_end(args);
}


/*
void ExitCondition (unsigned timerSec, const char* exitKeyword)
{
    fd_set          s;
    struct timeval  timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    char* exitFlag = (char*) calloc(12, sizeof(char));
    int sFlag;
    while (1)
    {
        fflush(stdout);
        FD_ZERO(&s);
        FD_SET(STDIN_FILENO, &s);
        timeout.tv_sec = 10; timeout.tv_usec = 0;
        sFlag = select(STDIN_FILENO+1, &s, NULL, NULL, &timeout);
        if      (sFlag < 0) _exit(0);
        else if (sFlag == 0) 
        {
            fflush(stdout);
            printf("-\n");
        }
        else     
        {
            fgets(exitFlag, 12, stdin);
            printf("To Quit, please enter \"EXIT\": "); 
            fflush(stdout);
            if ( memcmp(exitFlag, "EXIT", 4) == 0 ) break;
        }
    }


}

*/
#endif
