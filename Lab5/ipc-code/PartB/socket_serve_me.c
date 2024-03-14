#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "example_path"

int main(int argc, char *argv[])
{
    int sockfd;
    char buffer[256];
    struct sockaddr_un serv_addr, cli_addr;
    int n;

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCK_PATH);
    bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    bzero(buffer, 256);
    int len = sizeof(cli_addr);

    printf("server ready\n");

    n = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr *) &cli_addr, &len);

    printf("The message is: %s", buffer);

    unlink(SOCK_PATH);
}