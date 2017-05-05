#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


int ID = 1;
int GENERATED_M, GENERATED_F, REJECTED_M, REJECTED_F, DISCARDED_M, DISCARDED_F;

char* GENERATE_FIFO = "/tmp/entrada";
char* REJECTED_FIFO = "/tmp/rejeitados";

typedef struct{
  int id;
  char gender;
  int duration;
  int denials;
} Request;


Request* generateRequest(int maxDuration){

    Request* request = malloc(sizeof(Request));

    request->id = ID++;
    request->gender = (rand()%2) ? 'M' : 'F';
    request->duration = rand() % maxDuration + 1;
    request->denials = 0;

    return request;
}

int main(int argc, char* argv[]){

  if(argc != 3){
    printf("Wrong number of arguments. USAGE: program_name <number of requests> <max duration>\n");
    exit(-1);
  }

  if(mkfifo(GENERATE_FIFO, O_RDWR) != 0){
    printf("Error creating GENERATE fifo\n");
    exit(-1);
  }

  int requests = atoi(argv[1]);
  int maxDuration = atoi(argv[2]);
  int fifo_fd;
  time_t t;
  srand((unsigned) time(&t));

  if(fifo_fd = open(GENERATE_FIFO, O_WRONLY)) == -1){
    printf("Error openning for write GENERATE_FIFO\n");
    exit(-1);
  }



  Request* r = generateRequest(maxDuration);
  printf("struct ID: %d\n", r->id);
  printf("struct gender: %s\n", &r->gender);
  printf("struct duration: %d\n", r->duration);
  printf("struct denials: %d\n", r->denials);

  /*
  r = generateRequest(maxDuration);
  printf("struct ID: %d\n", r->id);
  printf("struct gender: %s\n", &r->gender);
  printf("struct duration: %d\n", r->duration);
  printf("struct denials: %d\n", r->denials);

  r = generateRequest(maxDuration);
  printf("struct ID: %d\n", r->id);
  printf("struct gender: %s\n", &r->gender);
  printf("struct duration: %d\n", r->duration);
  printf("struct denials: %d\n", r->denials);

  r = generateRequest(maxDuration);
  printf("struct ID: %d\n", r->id);
  printf("struct gender: %s\n", &r->gender);
  printf("struct duration: %d\n", r->duration);
  printf("struct denials: %d\n", r->denials);

  r = generateRequest(maxDuration);
  printf("struct ID: %d\n", r->id);
  printf("struct gender: %s\n", &r->gender);
  printf("struct duration: %d\n", r->duration);
  printf("struct denials: %d\n", r->denials);
  */

  write(fifo_fd,r,sizeof(r));


  return 0;
}
