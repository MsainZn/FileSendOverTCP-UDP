#include "utilityFunctionsMHZ.h"

int main(int argc, char* argv[])
{
    printf("ll1 size is: %lu\n", FileSizeCalculator("../bin/log"));
    printf("ll2 size is: %lu\n", FileSizeCalculator("../bin/ll2"));
    //printf("ll2 size is: %lu\n", FileSizeCalculator("../bin/ll3"));

    printf("\nLocation Of Error is: %lu\n", FileCorrector("../bin/ll2"));

    return 0;
}