#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>



int main(){

  switch(fork()){

    case -1:
      perror("Fork error");
      break;

    case 0: //filho
      printf(" world!\n");
      break;

    default: //pai
      printf("Hello");
  }


  return 0;

}
