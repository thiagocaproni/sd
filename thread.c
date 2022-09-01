#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NTHREADS 50
void *thread_function(void *);
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  counter = 0;

int main()
{
   srand(time(NULL));
   pthread_t thread_id[NTHREADS], id[NTHREADS];
   int i, j;

   for(i=0; i < NTHREADS; i++)
   {
      id[i] = i;
      pthread_create( &thread_id[i], NULL, thread_function, &id[i]);
   }

   for(j=0; j < NTHREADS; j++)
   {
      // A funcao join suspende a execução da thread que criou as outras thread
      // que a thread alvo (passada como parâmetro) finalize
      pthread_join(thread_id[j], NULL); 
   }

   printf("Final counter value: %d\n", counter);
   return 0;
}

void *thread_function(void *dummyPtr)
{
   int *myid =  dummyPtr;
   sleep(1);
   pthread_mutex_lock( &mutex1 );
   counter++;
   printf("Thread number %d: %d\n", *myid, counter);
   pthread_mutex_unlock( &mutex1 );
}