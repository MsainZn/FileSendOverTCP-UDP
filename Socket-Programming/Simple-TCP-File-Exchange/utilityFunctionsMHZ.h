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

// Usefull functions

void CheckError(const char *errMssg)
{
    perror(errMssg);
    exit(1);
}

#endif
