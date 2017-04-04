#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#define NUMBER_OF_THREADS 5

void * sleeper_thread(void* arg){
  sleep(1);
  printf("Awwww...Good morning. I am thread number: %lu\n", pthread_self());

  int* order = malloc(sizeof(int));
  *order = *((int*)arg) + 1;
  return order;
}

int main(){

  pthread_t tid[NUMBER_OF_THREADS];
  int* result;

  for(int i = 0; i < NUMBER_OF_THREADS; i++){
    pthread_create(&tid[i],NULL,sleeper_thread,&i);
    pthread_join(tid[i],(void*)&result);
    printf("Thread %lu return value(order): %d\n\n", tid[i] ,*result);
  }

  pthread_exit(0);
  //se retornasse com "return" ou "exit" a main thread nao esperava pelas outras
  //e estas terminariam com a main thread
}
