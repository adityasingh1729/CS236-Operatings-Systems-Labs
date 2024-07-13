#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    void *ptr = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    char *data = "hello world";
    size_t len = strlen(data) + 1;
    memcpy(ptr, data, len);
    int num;
    scanf("%d", &num);
    return 0;
}