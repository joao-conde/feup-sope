// PROGRAMA p7.c  - COMPILE(gcc)

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[]){

   char prog[20];
   sprintf(prog,"%s.c",argv[1]);
   execlp("gcc","gcc",prog,"-Wall","-o",argv[1],NULL);

   printf("execlp call failed...");
   exit(1);

}
