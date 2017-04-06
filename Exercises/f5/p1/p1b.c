#include <stdio.h>
#include <unistd.h>


#define READ  0
#define WRITE 1

int main(){

  struct Pair {
   int   n1;
   int   n2;
 } info;

  int fd[2];

  if(pipe(fd) < 0)
    perror("Error creating pipe");

  switch(fork()){

    case -1:
      perror("Error in fork()");
      break;

    case 0: //child process;

      close(fd[WRITE]);
      read(fd[READ],&info,sizeof(info));
      close(fd[READ]);

      printf("SUM: %d\n", info.n1 + info.n2);
      printf("SUB: %d\n", info.n1 - info.n2);
      printf("MULT: %d\n", info.n1 * info.n2);

      if(info.n2 == 0){
        printf("Dividend must not be 0!\n");
      }
      else{
        printf("DIV: %f\n", (float)info.n1 / (float)info.n2);
      }

      break;

    default: //parent process
      close(fd[READ]);
      printf("Numbers (n1 n2):");
      scanf("%d %d", &info.n1, &info.n2);
      write(fd[WRITE],&info,sizeof(info));
      close(fd[WRITE]);
  }

  return 0;
}
