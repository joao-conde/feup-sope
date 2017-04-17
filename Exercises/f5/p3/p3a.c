#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_NAMES 150
#define MAX_LINE_SIZE 30


int cmp_names(const void* elem1, const void* elem2){

  char firstLetter1 = *((char*)elem1);
  char firstLetter2 = *((char*)elem2);

  return firstLetter1 - firstLetter2;
}


int main(int argc, char* argv[]){

  FILE *fp;
  char* name_list[MAX_NAMES];
  char* line;

  if(argc != 2){
    printf("Wrong format... USAGE: p3a [FILE NAME]\n");
    exit(1);
  }

  if((fp = fopen(argv[1] , "r")) == NULL){
    printf("Error openning file...\n");
    exit(2);
  }


  for(int j = 0; j < MAX_NAMES; j++){
    name_list[MAX_NAMES] = NULL;
  }

  int index = 0;
  while(!feof(fp)){

    line = malloc(sizeof(char) * MAX_LINE_SIZE);

    if(fgets(line,MAX_LINE_SIZE,fp) == NULL)
      printf("null\n");

    name_list[index] = line;
    index++;

  }

  //qsort(name_list,sizeof(name_list)/sizeof(line),sizeof(line),cmp_names);

  fclose(fp);

  for(int i=0; name_list[i] != NULL; i++){
    puts(name_list[i]);
  }

  return 0;
}
