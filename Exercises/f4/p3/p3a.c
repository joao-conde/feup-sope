#include <signal.h>
#include <unistd.h>
#include <stdio.h>


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

   //subscribing the handler
   struct sigaction act;

   act.sa_handler = sigusr_handler;
   act.sa_flags = 0;
   sigemptyset(&act.sa_mask);

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


   return 0;

 }
