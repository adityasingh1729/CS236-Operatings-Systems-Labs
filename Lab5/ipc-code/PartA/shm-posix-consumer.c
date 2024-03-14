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
    const int SIZE = 4096;
    const char *name = "OS";

    const char *free_str = "freeeee";

    const char *fifo_w = "/tmp/wfifo";
    const char *fifo_r = "/tmp/rfifo";
    const char *fifo_c = "/tmp/confirm";

    mkfifo(fifo_w, 0666);
    mkfifo(fifo_r, 0666);
    mkfifo(fifo_c, 0666);

    int fifo_write = open(fifo_w, O_RDONLY);
    int fifo_read = open(fifo_r, O_WRONLY);
    int confirm = open(fifo_c, O_WRONLY);
    int shm_fd;
    void *ptr;

    shm_fd = shm_open(name, O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);

    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Map Failed\n");
        exit(-1);
    }

    char *str = (char *)malloc(8);

    for (int i = 0; i < 1000; i++) {
        int idx;
        read(fifo_write, &idx, sizeof(int));
        memcpy(str, ptr + idx*8, 8);
        printf("[CONSUMER] read %s from index %d\n", str, idx);
        write(fifo_read, &idx, sizeof(int));
        sleep(1);
    }

    write(confirm, "c", 1);
    
    close(fifo_write);
	close(fifo_read);
	close(confirm);

	return 0;

}