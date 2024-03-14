#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{
    const char *name = "Pipe";
    char message[256];

    int pipe_fd;

    if (mkfifo(name, 0666) == -1) {
        error("mkfifo error");
    }

    pipe_fd = open(name, O_WRONLY);
    if (pipe_fd == -1) {
        error("open error");
    }

    printf("Please enter the message: ");
    bzero(message,256);
    fgets(message,255,stdin);

    int n;
    n = write(pipe_fd, message, strlen(message));
    if (n == -1) {
        error("write error");
    }
    close(pipe_fd);
}