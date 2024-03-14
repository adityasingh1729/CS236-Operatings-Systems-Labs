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
#include <sys/msg.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{
    const char *name = "msgqueue";
    char message[256];

    int key = 5;
    int msgid = msgget(key, O_RDWR);
    if (msgid == -1) {
        error("msgget failed\n");
    }

    while(1) {
        int n = msgrcv(msgid, message, sizeof(message), 0, 0);
        if (n == -1) {
            error("msgrcv failed\n");
        }
        printf("%s", message);
    }
    return 0;
}