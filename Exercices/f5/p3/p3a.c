#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){

  FILE *fp;
  char* name_list[150];

  if(argc != 2){
    printf("Wrong format... USAGE: p3a [FILE NAME]\n");
    exit(1);
  }

  if((fp = fopen(argv[1] , "r")) == NULL){
    printf("Error openning file...\n");
    exit(2);
  }

  int index = 0;
  while(!feof(fp)){

    fgets(fp, &line_size);
    puts(line_size);


  }

  fclose(fp);
/*
  for(int i = 0; i < sizeof(name_list); i++){
    puts("NAME: ");
    puts(name_list[i]);
  }*/


  return 0;
}
