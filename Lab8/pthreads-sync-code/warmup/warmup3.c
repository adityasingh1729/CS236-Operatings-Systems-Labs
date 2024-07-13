#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void
*threadDeclare(void *arg)
{
    pthread_mutex_lock(&lock);
    int randNum = (rand() % 10) + 1;
    sleep(randNum);
    printf("I am thread %ld\n", (long) arg);
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&c);
    pthread_exit(NULL);
    return NULL;
}

int
main()
{
    srand(time(NULL));
    pthread_t threads[N];
    for (long i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, threadDeclare, (void *)i);
        pthread_cond_wait(&c, &lock);
    }
    pthread_cond_signal(&c);
    for (long i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("I am the main thread\n");
    pthread_exit(NULL);
    return 0;
}