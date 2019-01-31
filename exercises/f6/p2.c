
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


#define STDERR 2

int num_chars = 5000;

void * thrfunc(void * arg) {

  int* written_chars = (int*)malloc(sizeof(int));

  fprintf(stderr, "Starting thread %s\n", (char *) arg);

  while(1){
    if(num_chars <= 0)
      return (void*)written_chars;

    write(STDERR,arg,1);
    num_chars--;
    (*written_chars)++;
  }

}


int main(int argc, char* argv[]){

  pthread_t ta, tb;
  int* var1 = (int*)argv[1];
  int* var2 = (int*)argv[2];
  int res_t1, res_t2;

  pthread_create(&ta, NULL, thrfunc, var1);
  pthread_create(&tb, NULL, thrfunc, var2);

  pthread_join(ta, (void*)&res_t1);
  pthread_join(tb, (void*)&res_t2);

  printf("\nNumber of chars wrriten by t1: %d\n", res_t1);
  printf("\nNumber of chars wrriten by t2: %d\n", res_t2);



  return 0;
}
