#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
  int rc = fork();
  int wc = 1;
  while (wc >= 0) {
    wc = waitpid(-1, NULL, WNOHANG);
    printf("%d\n", wc);
  }
  //wc = waitpid(-1, NULL, WNOHANG);
  //printf("%d\n", wc);
  return 0;  
}
