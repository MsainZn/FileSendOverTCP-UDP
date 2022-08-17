#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/types.h>


int main() 
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
        if      (sFlag < 0) exit(0);
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

  return 0; 

}


/*






/*

#include <stdio.h>
#include <unistd.h>
#include <sys/poll.h>

  struct pollfd pfd;
  pfd.events = POLLIN;
  pfd.fd = STDIN_FILENO;
  char exitFlag;
  int pFlag;

  while (1) 
  {
    printf("\nPress E to Exit: ");
    fflush(stdout);
    pFlag = poll(&pfd, 1, 3000);
    if      (pFlag < 0) _exit(0);
    else if (pFlag == 0) 
    {
        fflush(stdout);
        printf("-\n");
    }
    else     
    {
      fflush(stdin);
      scanf("%c", &exitFlag);
      printf("%c\n", exitFlag);
      fflush(stdout);
      if ( exitFlag =='E' ) break;
    }

  }
*/

