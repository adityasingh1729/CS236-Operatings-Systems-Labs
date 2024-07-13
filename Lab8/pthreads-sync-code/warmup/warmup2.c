#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#define N 100

void
*threadDeclare(void *arg)
{
    printf("I am thread %ld\n", (long) arg);
    pthread_exit(NULL);
    return NULL;
}

int
main()
{
    pthread_t threads[N];
    for (long i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, threadDeclare, (void *)i); // Learning: I tried casting int with (void *) but that did not work, we can cast long with (void *)
    }
    for (long i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("I am the main thread\n");
    pthread_exit(NULL);
    return 0;
}