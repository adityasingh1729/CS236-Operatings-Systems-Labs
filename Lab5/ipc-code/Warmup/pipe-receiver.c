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
    int pipe_fd;

    pipe_fd = open(name, O_RDONLY);
    if (pipe_fd == -1) {
        error("open error");
    }
    int size = 256;
    char message[size];
    int n = read(pipe_fd, message, size);
    printf("%s", message);
    close(pipe_fd);
    return 0;
}