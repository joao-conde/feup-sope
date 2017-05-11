#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>

#define MAX_REQUESTS 128

int ID = 1;
int MAX_DURATION;
int GENERATED_M, GENERATED_F, REJECTED_M, REJECTED_F, DISCARDED_M, DISCARDED_F;

char* GENERATE_FIFO = "/tmp/entrada";
char* REJECTED_FIFO = "/tmp/rejeitados";
char* REGISTRY_MSG;

struct timeval start, stop;

typedef struct{
  int id;
  char gender;
  int duration;
  int denials;
} Request;

typedef enum {REQUESTS, REJECTED, DISCARDED} tip;

Request* requestList[MAX_REQUESTS];

/* REQUEST GENERATOR THREAD */
void* requestsThread(void* arg){

  int requests = *(int*) arg;
  if (requests >= 128){ // Whether the number of requests exceeds the array limit.
    printf("Too many requests to generate!\n");
    exit(-1);
  }

  for (int i = 0; i < requests; i++){
    Request* request = malloc(sizeof(Request));

    request->id = ID++;
    request->gender = (rand() % 2) ? 'M' : 'F';
    request->duration = rand() % MAX_DURATION + 1;
    request->denials = 0;

    requestList[i] = request;

    /*gettimeofday(&stop, NULL);
    long thisInst = ((stop.tv_sec - start.tv_sec) * 1000.0);
    inst += (stop.tv_usec - start.tv_usec) / 1000.0;

    pid_t thisPID = getpid();*/

  }
  pthread_exit(NULL);
}

void* rejectedListener(void* arg){

  int fifo_fd;
  Request* r;

  while ((fifo_fd = open(REJECTED_FIFO, O_RDONLY)) == -1){
    if (errno == ENOENT) printf("No rejected pipe available! Retrying...\n");
    sleep(2);
  }

  while(read(fifo_fd, r, sizeof(Request)) != 0){
    printf("REJECTED PIPE\nID: %d\nGender: %c\nDuration: %d\nDenials: %d\n", r->id, r->gender, r->duration, r->denials);
  }

  write(fifo_fd, r, sizeof(*r));
  pthread_exit(NULL);
}

int main(int argc, char* argv[]){
  gettimeofday(&start, NULL);

  if(argc != 3){
    printf("Wrong number of arguments. USAGE: program_name <number of requests> <max duration>\n");
    exit(-1);
  }

  //Creates registry messages path.
  char pid_str[32], endString[64] = "/tmp/ger.";
  sprintf(pid_str, "%d", getpid());
  strcat(endString, pid_str);
  REGISTRY_MSG = (char*) malloc(strlen(endString) + 1);
  strcpy(REGISTRY_MSG, endString);

  //If pipe already exists, errno has EEXIST value. (http://www.virtsync.com/c-error-codes-include-errno)
  if(mkfifo(GENERATE_FIFO, S_IRUSR | S_IWUSR) != 0 && errno != EEXIST){
    perror("Error creating GENERATE fifo");
    exit(-1);
  }

  int requests = atoi(argv[1]);
  MAX_DURATION = atoi(argv[2]);

  int fifo_fd;
  time_t t;
  srand((unsigned) time(&t));

  pthread_t gen_tid, req_tid;

  pthread_create(&gen_tid, NULL, requestsThread, (void*) &requests);
  pthread_create(&req_tid, NULL, rejectedListener, NULL);

  while ((fifo_fd = open(GENERATE_FIFO, O_WRONLY | O_NONBLOCK)) == -1){
    if (errno != ENXIO){
      perror("Error opening GENERATE_FIFO for write");
      exit(-1);
    }
  }


  /*printf("struct gender: %s\n", &r->gender);
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

  r = generateRequest(maxDuration);
  printf("struct ID: %d\n", r->id);
  printf("struct gender: %s\n", &r->gender);
  printf("struct duration: %d\n", r->duration);
  printf("struct denials: %d\n", r->denials);
  */


  for(int i = 0; i < requests; i++){
    Request* r = requestList[i];
    printf("struct ID: %d\nstruct gender: %s\nstruct duration: %d\nstruct denials: %d\n", r->id, &r->gender, r->duration, r->denials);
    write(fifo_fd, r, sizeof(*r));
  }

  unlink(GENERATE_FIFO); //This should probably be an exit handler.

}
