#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <stdio.h>


int main(){

  pid_t pid, pid2;

  switch (pid = fork()) {

    case -1: //error
      perror("Fork1 Error");
      break;

    case 0: //child

      switch (pid2 = fork()) {
        case -1: //error
          perror("Fork2 Error");
          break;

        case 0: //child
          printf("Hello ");
          break;

        default: //parent
          waitpid(pid2,NULL,0);
          printf("my ");

      }

      break;

    default: //parent
      waitpid(pid,NULL,0);
      printf("friend!\n");

  }

  return 0;

}
