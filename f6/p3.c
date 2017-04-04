// PROGRAMA p03.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 10


void *PrintHello(void *threadnum){
   printf("Hello from thread no. %d!\n", *(int *) threadnum);
   pthread_exit(NULL);
}


int main(){

   pthread_t threads[NUM_THREADS];

   for(int t=0; t< NUM_THREADS; t++){
      printf("Creating thread %d\n", t);
      pthread_create(&threads[t], NULL, PrintHello, (void *)&t);
      pthread_join(threads[t],NULL); //esta é a solução: com "pthread_join" obriga-se a main thread a bloquear nesta instrução e esperar que a thread acabada de criar termine
   }


  //exit(0)   ---- > com o uso de "return" ou "exit" a main thread acaba e
  //por isso todas as threads ainda em execução também, nao fazendo o print

  pthread_exit(0);
}
