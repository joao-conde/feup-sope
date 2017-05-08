#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

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

  //If pipe already exists, errno has EEXIST value. (http://www.virtsync.com/c-error-codes-include-errno)
  if(mkfifo(GENERATE_FIFO, S_IRUSR | S_IWUSR) != 0 && errno != EEXIST){
    perror("Error creating GENERATE fifo");
    exit(-1);
  }

  int requests = atoi(argv[1]);
  int maxDuration = atoi(argv[2]);
  int fifo_fd;
  time_t t;
  srand((unsigned) time(&t));

  while ((fifo_fd = open(GENERATE_FIFO, O_WRONLY | O_NONBLOCK)) == -1){
    if (errno != ENXIO){
      perror("Error opening GENERATE_FIFO for write");
      exit(-1);
    }
  }

  Request* r;
  /*printf("struct gender: %s\n", &r->gender);
  printf("struct duration: %d\n", r->duration);
  printf("struct denials: %d\n", r->denials);*/

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

  for(int i = 0; i < requests; i++){
    r = generateRequest(maxDuration);
    printf("struct ID: %d\nstruct gender: %s\nstruct duration: %d\nstruct denials: %d\n", r->id, &r->gender, r->duration, r->denials);
    write(fifo_fd, r, sizeof(*r));
  }

  unlink(GENERATE_FIFO); //This should probably be an exit handler.

  return 0;
}
