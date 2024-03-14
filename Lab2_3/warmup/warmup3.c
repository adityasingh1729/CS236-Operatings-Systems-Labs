#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
  printf("hello world (pid:%d)\n", (int) getpid());
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "frok failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("hello, i am a child (pid:%d)", (int) getpid());
    char *myargs[3];
    myargs[0] = strdup("ls");
    myargs[1] = strdup("Doc");
    myargs[2] = NULL;
    execvp(myargs[0], myargs);
    printf("WRONG WRONG -- SHOULDN'T PRINT\n");
  } else {
    int wc = wait(NULL);
    printf("I am parent of child = %d, and my pid = %d, wait = %d\n", (int) rc, (int) getpid(), wc);
  }
  return 0;
}
