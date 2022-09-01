/********************************************
 *              CSCI 4630                   *
 *                                          *
 * This program demonstrates a producer and *
 * consumer that communicate via a pipe.    *
 ********************************************/

#include <stdio.h>
#include <unistd.h>

/*******************************************
 *               producer                  *
 *******************************************
 * Write information into the pipe whose   *
 * write-end is given by pipe_write_end.   *
 *******************************************/

void producer(FILE *pipe_write_end)
{
  int i;
  for(i = 1; i <= 100; i++) {
    fprintf(pipe_write_end, "%d ", i);
  }
  fclose(pipe_write_end);
  exit(0);
}

/*******************************************
 *               consumer                  *
 *******************************************
 * Read information from the pipe whose    *
 * read-end is given by pipe_read_end,     *
 * and copy it to the standard output.     *
 *******************************************/

void consumer(FILE *pipe_read_end)
{
  int n,k;

  while(1) {
    int n = fscanf(pipe_read_end, "%d", &k);
    if(n == 1) printf("consumer: got %d\n", k);
    else break;
  }
  fclose(pipe_read_end);
  exit(0);
}

/*******************************************
 *              main                       *
 *******************************************
 * Build the producer and consumer and     *
 * wait for them to finish.                *
 *******************************************/

int main()
{
  pid_t producer_id, consumer_id;
  int pd[2];
  FILE *pipe_write_end, *pipe_read_end;

  /*-----------------*
   * Build the pipe. *
   *-----------------*/

  pipe(pd);
  pipe_read_end  = fdopen(pd[0], "r");
  pipe_write_end = fdopen(pd[1], "w");
  
  /*--------------------*
   * Fork the producer. *
   *--------------------*/

  producer_id = fork();
  if(producer_id == 0) {
    fclose(pipe_read_end);
    producer(pipe_write_end);
  }

  /*--------------------*
   * Fork the consumer. *
   *--------------------*/

  consumer_id = fork();
  if(consumer_id == 0) {
    fclose(pipe_write_end);
    consumer(pipe_read_end);
  }

  /*--------------------------*
   * Wait for both to finish. *
   *--------------------------*/

  fclose(pipe_read_end);
  fclose(pipe_write_end);
  wait(NULL);
  wait(NULL);

  return 0;
}