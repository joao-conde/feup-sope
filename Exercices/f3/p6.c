
// PROGRAMA p6.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

int main(void)
{
 pid_t pid;
 int i, j;
 printf("I'm process %d. My parent is %d.\n", getpid(),getppid());
 for (i=1; i<=3; i++) {
       pid = fork();
       if ( pid < 0) {
                  printf("fork      error");
                  exit(1);
       }
       else if (pid == 0) {
             printf("I'm process %d. My parent is %d. I'm going to work for 1 second ...\n", getpid(),getppid());
             sleep(1); // simulando o trabalho do filho
             printf("I'm process %d. My parent is %d. I finished my work\n", getpid(),getppid());
             exit(0);  // a eliminar na alinea c)
       }
       else            // simulando o trabalho do pai
                  for (j=1; j<=10; j++) {
                        sleep(1);
                        waitpid(-1,NULL,WNOHANG);
                        printf("father      working      ...\n");
                      }
 }
 exit(0);
}
