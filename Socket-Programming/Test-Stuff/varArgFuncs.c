#include <stdio.h>
#include <stdarg.h>
#include <string.h>

int FindMaxValue(int argCount, ...)
{
    va_list args;
    va_start(args, argCount);

    int x, maxx;
    for (size_t i = 0; i < argCount; i++)
    {
        x = va_arg(args, int);
        if (i == 0) maxx = x;
        if (x>maxx) maxx = x;
    }
    va_end(args);
    return maxx;
}


void fprintfIO(FILE* stream, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    vfprintf(stream, format, args);



    va_end(args);
}


int main(int argc, char** argv)
{
    
    //printf("%d\n", FindMaxValue(5,20,3,4,15,96));


    fprintfIO (stdout, "Hello %d is %lf",1,3.5);
    fprintfIO (stdout, "nji %s is %lf","hiiiio",3.5);




    return 0;
}