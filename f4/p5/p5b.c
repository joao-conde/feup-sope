// PROGRAMA p04a.c
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

void sigchld_handler(int sig){
  while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(void)
{

  //subscribing the SIGCHLD handler
  struct sigaction act;
  act.sa_handler = sigchld_handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaction(SIGCHLD, &act, NULL);


  pid_t pid;
  int i, n, status;



  for (i=1; i<=15; i++) {

    pid=fork();

    if (pid == 0){
      printf("CHILD no. %d (PID=%d) working ... \n",i,getpid());
      sleep(15); // child working ...
      printf("CHILD no. %d (PID=%d) exiting ... \n",i,getpid());
      exit(0);
    }
  }

  for (i=1 ;i<=16; i++ ) {
    printf("PARENT: working hard (task no. %d) ...\n",i);
    n=20;
    while((n=sleep(n))!=0);
    printf("PARENT: end of task no. %d\n",i);
    printf("PARENT: waiting for child no. %d ...\n",i);


    if (pid != -1)
      printf("PARENT: child with PID=%d terminated with exit code %d\n",pid,WEXITSTATUS(status));
  }

  exit(0);
}
