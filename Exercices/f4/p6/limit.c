#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>



void sigalarm_handler(int signo){
    kill(getppid(),SIGKILL);
}

int main(int argc, char* argv[]){

  unsigned int timer = *argv[1];

  struct sigaction act;
  act.sa_handler = sigalarm_handler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);

  char* args = &argv[3];

  switch(fork()){
    case -1:
      perror("Error forking");
      break;

    case 0: //child process
      sleep(1);
      sigaction(SIGALRM, &act, NULL);
      alarm(timer);
      wait(NULL);
      break;

    default: //parent process
      if(execlp("limit", "10", "prog", "lol", NULL) == -1)
        perror("Error in exec() call");
  }


  return 0;
}
