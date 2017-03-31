#include <stdio.h>
#include <unistd.h>


#define READ  0
#define WRITE 1

int main(){

  int fd[2];

  struct{
    char string1[6];
    char string2[6];
  }info;

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

      printf("SUM: %d\n", (int)info.string1 + (int)info.string2);
      printf("SUB: %d\n", (int)info.string1 - (int)info.string2);
      printf("MULT: %d\n", (int)info.string1 * (int)info.string2);

      if((int)info.string1 == 0){
        printf("Dividend must not be 0!\n");
      }
      else{
        printf("DIV: %f\n", (float)info.string1 / (float)info.string2);
      }

      break;

    default: //parent process
      close(fd[READ]);
      printf("Numbers (n1 n2):");
      scanf("%s %s", &info.string1, &info.string2);
      write(fd[WRITE],&info,sizeof(info));
      close(fd[WRITE]);
  }

  return 0;
}
