#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include "alloc.h"

int init_alloc()
{
    void *page = mmap(0, PAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (page == MAP_FAILED) {
        perror("Map Failed");
        
    }
}

int cleanup()
{

}

char *alloc(int)
{

}

void dealloc(char *)
{

}

// int main()
// {

//     return 0;
// }