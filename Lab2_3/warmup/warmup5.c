#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
  int N = 5;
  printf("hello world (pid:%d)\n", (int) getpid());
  for (int i = 0; i < N; i++) {
    int rc = fork();
    if (rc < 0) {
      fprintf(stderr, "fork failed\n");
      exit(1);
    } else if (rc == 0) {
      printf("Child = %d\n", (int) getpid());
    } else {
      //printf("Parent: %d\n", (int) getpid());
    }
  }
  int wc = 1;
  while (wc >= 0) {
    wc = waitpid(-1, NULL, WNOHANG);
  }
  return 0;
}
