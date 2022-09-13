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
#include <errno.h>
#define BUFFER_MAX 1024

struct bufferTCP
{
	char mssgPtr[BUFFER_MAX];
    size_t BUFFER_USED;
};

// Usefull functions
struct bufferTCP* CreateBufferTCP();
void WriteLog(const char* logFileName, const char* mssg);
size_t FileCorrector (const char* fileName);;
FILE* FileOpenSafe (const char* fileName, const char* rwaMode);
size_t FileSizeCalculator (const char* fileName);
void fprintfSwitchable(FILE* stream, int errFlag, const char* mssg, ...);
FILE* FileOpenSafe (const char* fileName, const char* rwaMode);
size_t FileSizeCalculator (const char* fileName);
size_t FileCorrector (const char* fileName);
size_t FileModifier(const char* fileName);
void Initializer (const char* caller, FILE* stream);
int isEnd (size_t sleepTimer, const char* caller);
int isFileAvailable (const char* fileLocFull, const char* caller, FILE* stream);

void WriteLog(const char* logFileName, const char* mssg)
{
    FILE* logFilePtr = fopen(logFileName, "a");
    //fputs(mssg, logFilePtr);
    fclose(logFilePtr);
}


void fprintfSwitchable(FILE* stream, int errFlag, const char* mssg, ...)
{
    va_list args;

    va_start(args, mssg);

    if (stream == NULL)
    {
        vfprintf(stdout, mssg, args);
        fflush(stdout);
    }
    else
    {
        vfprintf(stream, mssg, args);
        fflush(stream);
    }    

    if (errFlag)  exit(1);
        
    va_end(args);
}


FILE* FileOpenSafe (const char* fileName, const char* rwaMode)
{
    if (fileName == NULL)
        fprintfSwitchable(NULL, -1, "FileName or Location is Incorrect!\n");

    FILE* filePtr = fopen(fileName, rwaMode);

    if (filePtr == NULL)
        fprintfSwitchable(NULL, -1, "File Location is Invalid Or File Does Not Exists!\n");

    return filePtr;
}


size_t FileSizeCalculator (const char* fileName)
{
    FILE* filePtr = FileOpenSafe(fileName, "r");
    int fileDescriptor = fileno(filePtr);
    struct stat fileInfo;
    bzero(&fileInfo, sizeof(struct stat));
    fstat(fileDescriptor, &fileInfo);
    size_t fileSize =  fileInfo.st_size;
    fclose(filePtr);

    return fileSize;
}

size_t FileCorrector (const char* fileName)
{
    size_t fileSize = FileSizeCalculator (fileName);
    size_t fileSizeTrue;
    if (fileSize == 0)
    {
        fprintfSwitchable(NULL, 0, "File is Empty, thus requires no correction!\n");
        return 0;
    }
        
    // Main & Temp File
    FILE* mainFile = FileOpenSafe (fileName, "r");
    FILE* tempFile = FileOpenSafe("temp", "w");
    
    // Fault Detection (\00 0\0 00) and correct
    char checkBuff[3];
    char curserrChar;
    
    size_t ii;
    for (ii = 0; ii < fileSize; ii++)
    {
        curserrChar = fgetc(mainFile);
        //printf("%c", curserrChar);
        if (curserrChar == '\00')
        {    
            fileSizeTrue = ftell(mainFile)-2;
            break;
        }
        fputc(curserrChar, tempFile);
    }
    fclose(mainFile);
    fclose(tempFile);

    // Remove The Old File
    if (remove(fileName) == 0) 
        fprintfSwitchable(NULL, 0, "Faulty File (%s) Removed Successfully.\n", fileName);
    else
        fprintfSwitchable(NULL, 1, "Error while Removing the Faulty File: %s!\n", fileName);


    // Remove The Old File
    if (rename("temp", fileName) == 0) 
        fprintfSwitchable(NULL, 0, "Corrected File Renamed Successfully.\n");
    else
        fprintfSwitchable(NULL, 1, "Error while Removing the Faulty File: %s!\n");

    fprintfSwitchable(NULL, 0, "Correction Implemented Successfully.\n");

    return fileSizeTrue;
}


size_t FileModifier(const char* fileName)
{
    size_t fileSize = FileSizeCalculator (fileName);
    size_t fileSizeTrue;
    if (fileSize == 0)
    {
        fprintfSwitchable(NULL, 0, "[+server] File is Empty, thus requires no correction.\n");
        return 0;
    }
        
    // Main & Temp File
    FILE* mainFile = FileOpenSafe (fileName, "r");
    FILE* tempFile = FileOpenSafe("temp", "w");
    
    // Fault Detection (\00 0\0 00) and correct
    char checkBuff[3];
    char curserrChar;
    size_t ii;
    for (ii = 0; ii < fileSize; ii++)
    {
        curserrChar = fgetc(mainFile);
        if (curserrChar == '\00') break;
        fputc(curserrChar, tempFile);
    }
    fclose(mainFile);
    fclose(tempFile);


    mainFile = FileOpenSafe (fileName, "w");
    tempFile = FileOpenSafe("temp", "r");
    fileSizeTrue = FileSizeCalculator ("temp");

    ii=0;
    for (ii = 0; ii < fileSizeTrue; ii++)
    {
        curserrChar = fgetc(tempFile);
        fputc(curserrChar, mainFile);
    }
    
    fclose(mainFile);
    fclose(tempFile);

    // Remove The Old File
    if (remove("temp") == 0) 
        fprintfSwitchable(NULL, 0, "[+server] Temp File Removed Successfully.\n");
    else
        fprintfSwitchable(NULL, 1, "[-server] Error while Removing the Faulty File!\n");

    fprintfSwitchable(NULL, 0, "[+server] File Modifier Executed Successfully.\n");

    return fileSizeTrue;
}


void Initializer (const char* caller, FILE* stream)
{
    // Initial confirmation client side
	char checkSend;
	fprintfSwitchable(stream, 0, "[+%s] Start Process [Y/N]: ", caller);
	scanf(" %c", &checkSend);
	if (checkSend == 'N' || checkSend == 'n')
    {
        fprintfSwitchable(stream, 0, "[-%s] Abort Process As Requested!\n", caller);
        exit(0);
    } 
    else
        return;
}
    
int isEnd (size_t sleepTimer, const char* caller)
{
    // Exit Material
    fd_set          s;
    struct timeval  timeout;
    char exitFlag[12];
    memset(&exitFlag, 'F', 12);
    int sFlag;

    fflush(stdout);
    FD_ZERO(&s);
    FD_SET(0, &s);
    timeout.tv_sec = sleepTimer; timeout.tv_usec = 0;
    sFlag = select(1, &s, NULL, NULL, &timeout);
    
    if      (sFlag < 0) 
        return 0;
    else if (sFlag == 0) 
    {
        fflush(stdout);
        fprintfSwitchable(NULL, 0, "-\n");
        return 0;
    }
    else
    {
        fgets(exitFlag, 12, stdin);
        fprintfSwitchable(NULL, 0, "\n[+%s] To Quit, please enter \"exit\": ", caller); 
        fflush(stdout);
        if ( memcmp(&exitFlag, "exit", 4) == 0 ) 
            return 1;
        else
            return 0;
    }

}

int isFileAvailable (const char* fileLocFull, const char* caller, FILE* stream)
{
    // Checks if The Target Send File Exists!
    FILE* filePtr_NACK = fopen(fileLocFull, "r");
    if (filePtr_NACK == NULL)
    {
        fprintfSwitchable(stream, 0, "[-%s] Not Such File Name Exists!\n", caller);
        return 0;
    }
    else
    {
        fprintfSwitchable(stream, 0, "[+%s] File Found Successfully.\n", caller);
        fclose(filePtr_NACK);
        return 1;
    }
    
}


void copyWithOffsetDest(char* dest, const char* src, size_t start, size_t length)
{
    size_t i = 0;
    for (i = 0; i < length; i++)
        dest[start+i] = src[i];
}

void copyWithOffsetSource(char* dest, const char* src, size_t start, size_t length)
{
    size_t i = 0;
    for (i = 0; i < length; i++)
        dest[i] = src[i+start];
}


struct bufferTCP* CreateBufferTCP()
{
    struct bufferTCP* mssg = (struct bufferTCP*) 
                malloc(sizeof(struct bufferTCP));
    return mssg;
}

#endif
