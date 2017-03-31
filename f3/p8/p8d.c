// PROGRAMA p8.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char *argv[], char *envp[])
{
   pid_t pid;
   char* exec_args[3];
   exec_args[0] = "-laR";
   exec_args[1] = argv[1];
   exec_args[2] = NULL;


   if (argc != 2) {
     printf("usage: %s dirname\n",argv[0]);
     exit(1);
   }
   pid=fork();
   if (pid > 0){
     printf("My child is going to execute command \"ls -laR %s\"\n", argv[1]);
     sleep(1);
   }
   else if (pid == 0){
     execv("/bin/ls", exec_args);
     printf("Command not executed !\n");
     exit(1);
   }
   exit(0);
}
