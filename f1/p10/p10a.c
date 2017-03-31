#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 250

int main(){

  char* buffer[BUFFER_SIZE];

  printf("Enter the command line: ");

  while(fgets(buffer, BUFFER_SIZE, stdin) != NULL){

    for(int i=0; i < BUFFER_SIZE; i++)
      printf("%s  ", buffer[i] );
  }

  for(int i=0; i < BUFFER_SIZE; i++)
    printf("%s  ", buffer[i] );

  printf("\n");

  return 0;

}
