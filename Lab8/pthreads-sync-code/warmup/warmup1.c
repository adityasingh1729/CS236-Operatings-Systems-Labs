#include <stdio.h>
#include <assert.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void
*counterIncrement()
{
    for (int i = 0; i < 1000; i++) {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
    return NULL;
}

int
main()
{
    pthread_t threads[10];
    int rc;
    printf("main begin: value of counter = %d\n", counter);
    for (int i = 0; i < 10; i++) {
        rc = pthread_create(&threads[i], NULL, counterIncrement, NULL); assert (rc == 0);
    }
    for (int i = 0; i < 10; i++) {
        rc = pthread_join(threads[i], NULL); assert (rc == 0);
    }
    printf("main end: value of counter = %d\n", counter);
    pthread_exit(NULL);
    return 0;
}