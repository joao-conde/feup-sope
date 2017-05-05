#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int SAUNA_CAPACITY;
int RECEIVED_F, RECEIVED_M, REJECTIONS_F, REJECTIONS_M, SERVED_F, SERVED_M;
char SAUNA_GENDER;

char* GENERATE_FIFO = "/tmp/entrada";
char* REJECTED_FIFO = "/tmp/rejeitados";

typedef struct{
  int id;
  char gender;
  int duration;
  int denials;
} Request;

int main(int argc, char* argv[]){


  if(argc != 2){
    printf("Wrong number of arguments. USAGE: program_name <max sauna users>\n");
    exit(-1);
  }

  if(mkfifo(REJECTED_FIFO, O_RDWR) != 0){
    printf("Error creating REJECTED fifo\n");
    exit(-1);
  }


  SAUNA_CAPACITY = atoi(argv[1]);
  int fifo_fd;
  Request* r;

  if((fifo_fd = open(GENERATE_FIFO, O_RDONLY)) == -1){
    printf("Error openning for read GENERATE_FIFO\n");
    exit(-1);
  }


  printf("number of sauna spots: %d\n", SAUNA_CAPACITY);

  read(fifo_fd, r , sizeof(r));

  printf("id %d\n", r->id);

  return 0;
}
