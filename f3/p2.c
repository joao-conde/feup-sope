// PROGRAMA p2.c
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int main(void) {

 //write(STDOUT_FILENO,"1",1);
 printf("%s", "1\n" );

 if(fork() > 0) {
   //write(STDOUT_FILENO,"2",1);
   //write(STDOUT_FILENO,"3",1);

   printf("%s", "2");
   printf("%s", "3");

 } else {
   //write(STDOUT_FILENO,"4",1);
   //write(STDOUT_FILENO,"5",1);

   printf("%s", "4");
   printf("%s", "5");

 }

  //write(STDOUT_FILENO,"\n",1);

  printf("\n");

  return 0;
}
