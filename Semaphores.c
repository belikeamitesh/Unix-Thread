#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define THREAD_NUM 4

sem_t semaphore;

void* routine(void* args) {
    sem_wait(&semaphore);
    sleep(1);
    printf("Hello from thread %d\n", *(int*)args);
    sem_post(&semaphore);
    free(args);
}

int main(int argc, char *argv[]) {
    pthread_t th[THREAD_NUM];
    sem_init(&semaphore, 0, 4);
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, &routine, a) != 0) {
            perror("Failed to create thread");
        }
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    sem_destroy(&semaphore);
    return 0;
}
/*
How is this working?
we need to run 4 threads T1 t2 t3 t4
we create semaphore of single process (0)
we give it resources 4 or 2 or 1
When resources=4
now when T1 goes in critical section
resouces-- =3
t2 goes t3 goes and t4 goes
Thus all prints simultaneously

suppose resoucrs =2
then t1 goes and t2 goes
thus now resources=0 means once they are completed others can enter
so prints t1 and t2
then waits(sleeps)
then t3 and t4 are printer

thats how we apply locks using semaphores if we have multipe resources
*/
