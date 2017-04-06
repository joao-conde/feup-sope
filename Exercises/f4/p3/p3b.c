#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <sys/types.h>


#define TRUE  1
#define FALSE 0

//global variable indicating "to add" or "to subtract"
int add = 1;

void sigusr_handler(int signo){


  if(signo == SIGUSR1){

    add = 1;

  }


  if(signo == SIGUSR2){

    add = 0;

  }

}


 int main(){

   //variable to modify
   int var = 0;

   pid_t pid;

   //subscribing the handler
   struct sigaction act;

   act.sa_handler = sigusr_handler;
   act.sa_flags = 0;
   sigemptyset(&act.sa_mask);



   switch (pid = fork()) {

    case -1: //error forking

      perror("Error forking");
      break;

    case 0: //child process

      sigaction(SIGUSR1, &act, NULL);
      sigaction(SIGUSR2, &act, NULL);

      while(1){

         printf("Current Value: %d\n",var);

         if(add)
           var++;
         else
           var--;

         sleep(1);

       }

      break;

    default: //parent process

    while(1){
      waitpid(pid,NULL,0);



    }

   }

   return 0;
 }
