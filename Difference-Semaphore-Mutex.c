#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define THREAD_NUM 4

pthread_mutex_t mutexFuel;
sem_t semFuel;
int fuel = 50;

void* routine1(void* args) {
    while (1) {
        pthread_mutex_lock(&mutexFuel);
        fuel += 50;
        printf("Current value is %d\n", fuel);
    }
}

void* routine2(void* args) {
    while (1) {
        pthread_mutex_unlock(&mutexFuel);
        usleep(5000);
    }
}

int main(int argc, char *argv[]) {
    pthread_t th[THREAD_NUM];
    sem_init(&semFuel, 0, 1);
    pthread_mutex_init(&mutexFuel, NULL);
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (i % 2 == 0) {
            if (pthread_create(&th[i], NULL, &routine1, NULL) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &routine2, NULL) != 0) {
                perror("Failed to create thread");
            }   
        }
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    pthread_mutex_destroy(&mutexFuel);
    sem_destroy(&semFuel);
    return 0;
}

/*
if we use mutex and lock in one thread and unlock in another thread, it behaves abnormally as it requies to immiditely to remove the lock after critical
section has done its works in that thread, thus it breaks the program abruptly
meanwhile semaphores has no requirements for unlocking in same thread or at same time after the critical section has funished working.

Strictly speaking, 
a mutex is a locking mechanism used to synchronize access to a resource. Only one task (can be a thread or process based on OS abstraction) can acquire the mutex. It means there is ownership associated with a mutex, and only the owner can release the lock (mutex). 

Semaphore is signaling mechanism (“I am done, you can carry on” kind of signal). For example, if you are listening to songs (assume it as one task) on your mobile phone and at the same time, your friend calls you,
an interrupt is triggered upon which an interrupt service routine (ISR) signals the call processing task to wakeup. 
*/
