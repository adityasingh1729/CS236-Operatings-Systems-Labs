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
    const char *name = "msgqueue2";
    char message[256];

    int key = 5;
    int msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        error("msgget failed\n");
    }

    strncpy(message, "Hello World\n", sizeof(message));

    if (msgsnd(msgid, message, sizeof(message), 0) == -1) {
        error("msgsnd failed\n");
    }

    sleep(5);

    strncpy(message, "second message\n", sizeof(message));

    if (msgsnd(msgid, message, sizeof(message), 0) == -1) {
        error("msgsnd failed\n");
    }

    return 0;
}