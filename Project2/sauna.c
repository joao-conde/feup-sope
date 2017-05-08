#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

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
    printf("Wrong number of arguments! USAGE: sauna <max sauna users>\n");
    exit(-1);
  }

  if(mkfifo(REJECTED_FIFO, O_RDWR) != 0 && errno != EEXIST){
    printf("Error creating REJECTED fifo\n");
    exit(-1);
  }

  SAUNA_CAPACITY = atoi(argv[1]);
  int fifo_fd;
  Request *r = malloc(sizeof(Request));

  printf("\nSAUNA CAPACITY: %d\n\n", SAUNA_CAPACITY);

  while ((fifo_fd = open(GENERATE_FIFO, O_RDONLY)) == -1){
    if (errno == ENOENT) printf("No pipe available! Retrying...\n");
    sleep(1); //Polls every second.
  }

  read(fifo_fd, r, sizeof(Request));
  if (r == NULL) exit(-1);

  printf("ID: %d\nGender: %c\nDuration: %d\nDenials: %d\n", r->id, r->gender, r->duration, r->denials);

  return 0;
}
