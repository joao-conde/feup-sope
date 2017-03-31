#include <stdio.h>
#include <unistd.h>


#define READ  0
#define WRITE 1

int main(){

  int n[2], fd[2];

  if(pipe(fd) < 0)
    perror("Error creating pipe");

  switch(fork()){

    case -1:
      perror("Error in fork()");
      break;

    case 0: //child process;

      close(fd[WRITE]);
      read(fd[READ],n,2*sizeof(int));
      close(fd[READ]);

      printf("SUM: %d\n", n[0] + n[1]);
      printf("SUB: %d\n", n[0] - n[1]);
      printf("MULT: %d\n", n[0] * n[1]);

      if(n[1] == 0){
        printf("Dividend must not be 0!\n");
      }
      else{
        printf("DIV: %f\n", (float)n[0] / (float)n[1]);
      }

      break;

    default: //parent process
      close(fd[READ]);
      printf("Numbers (n1 n2):");
      scanf("%d %d", &n[0], &n[1]);
      write(fd[WRITE],&n,sizeof(int)*2);
      close(fd[WRITE]);
  }

  return 0;
}
