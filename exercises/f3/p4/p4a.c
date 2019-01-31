#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){


  switch(fork()){

    case -1: //erro
      perror("Fork error");
      break;

    case 0: //filho
      printf("Hello ");
      break;

    default: //pai
      sleep(1);
      printf("world!\n");

  }


  return 0;

}
