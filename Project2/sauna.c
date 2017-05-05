#include <stdio.h>
#include <stdlib.h>

int SAUNA_CAPACITY;

int RECEIVED_F, RECEIVED_M, REJECTIONS_F, REJECTIONS_M, SERVED_F, SERVED_M;

int main(int argc, char* argv[]){

  if(argc != 2){
    printf("Wrong number of arguments. USAGE: program_name <max sauna users>\n");
    exit(-1);
  }

  SAUNA_CAPACITY = atoi(argv[1]);


  printf("number of sauna spots: %d\n", SAUNA_CAPACITY);

  return 0;
}
