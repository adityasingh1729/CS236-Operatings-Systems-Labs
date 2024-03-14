#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
  printf("hello world (pid:%d)\n", (int) getpid());
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("I am child with pid = %d\n", (int) getpid());
    exit(1);
  } else {
    int wc = wait(NULL);
    printf("I am parent of child = %d, and my pid = %d, wait = %d\n", (int) rc, (int) getpid(), wc);
  }
  return 0;
}
