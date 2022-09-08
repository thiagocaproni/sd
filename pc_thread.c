#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define THREAD_NUM 10

sem_t semEmpty;
sem_t semFull;

pthread_mutex_t mutexBuffer;

int buffer[10];
int count = 0;

void* producer(void* args) {
    int x;
    while (1) {
            x = rand() % 100;
            sleep(rand() % 5);

            // Add to the buffer
            sem_wait(&semEmpty);
            pthread_mutex_lock(&mutexBuffer);
            buffer[count] = x;
            count++;
            pthread_mutex_unlock(&mutexBuffer);
            sem_post(&semFull);
    }
}

void* consumer(void* args) {
     int y, *myid =  args;;
    while (1) {
        // Remove from the buffer
            sem_wait(&semFull);
            pthread_mutex_lock(&mutexBuffer);
            y = buffer[count - 1];
            count--;				  
            pthread_mutex_unlock(&mutexBuffer);
            sem_post(&semEmpty);

            // Consume
           //printf("%ld - Got %d\n",  pthread_self(), y);
           printf("%d - Got %d\n",  *myid, y);
            sleep(rand() % 5);
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semEmpty, 0, 10);
    sem_init(&semFull, 0, 0);
    int i, id[THREAD_NUM];

    for (i = 0; i < THREAD_NUM; i++) {
        id[i] =  i;
        if (i == 0) {
            if (pthread_create(&th[i], NULL, &producer, &id[i]) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &consumer, &id[i]) != 0) {
                perror("Failed to create thread");
            }
        }
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
    return 0;
}