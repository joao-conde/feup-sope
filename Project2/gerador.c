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


int ID = 1;
int MAX_DURATION;
int GENERATED_M, GENERATED_F, REJECTED_M, REJECTED_F, DISCARDED_M, DISCARDED_F;

char* GENERATE_FIFO = "/tmp/entrada";
char* REJECTED_FIFO = "/tmp/rejeitados";
char* REGISTRY_MSG;

int GENERATE_FD;

struct timeval start, stop;

typedef enum {REQUESTS, REJECTED, DISCARDED} Tip;

typedef struct{
  int id;
  char gender;
  int duration;
  int denials;
} Request;

/* REQUEST GENERATOR THREAD */
void* requestsThread(void* arg){
  int requests = *(int*) arg;

  //Creates the generate pipe.
  if(mkfifo(GENERATE_FIFO, S_IRUSR | S_IWUSR) != 0 && errno != EEXIST){
    perror("Error creating GENERATE fifo");
    exit(-1);
  }

  //Tries to open the generate pipe.
  while ((GENERATE_FD = open(GENERATE_FIFO, O_WRONLY)) == -1){
    if (errno != ENXIO){ //errno equals ENXIO if read side hasn't been opened yet.
      perror("Error opening GENERATE_FIFO for write");
      exit(-1);
    }
  }

  //Generates 'requests' number of random requests.
  for (int i = 0; i < requests; i++){
    Request* request = malloc(sizeof(Request));

    request->id = ID++;
    request->gender = (rand() % 2) ? 'M' : 'F';
    request->duration = rand() % MAX_DURATION + 1;
    request->denials = 0;

    //printf("GENERATED STRUCT\nID:%d\nGENDER:%s\nDUR:%d\nDENIALS:%d\n", request->id, &(request->gender), request->duration, request->denials);
    write(GENERATE_FD, request, sizeof(Request));
  }

  return NULL;
}

void* rejectedListener(void* arg){
  int fifo_fd;
  Request* r = malloc(sizeof(Request));

  while ((fifo_fd = open(REJECTED_FIFO, O_RDONLY)) == -1){
    if (errno == ENOENT) printf("No rejected pipe available! Retrying...\n");
    sleep(1);
  }

  while(read(fifo_fd, r, sizeof(Request)) != 0){
    printf("REJECTED PIPE\nID: %d\nGender: %c\nDuration: %d\nDenials: %d\n", r->id, r->gender, r->duration, r->denials);
    if (r->denials < 3) write(GENERATE_FD, r, sizeof(*r));
    else if (r->gender == 'M') DISCARDED_M++;
    else if (r->gender == 'F') DISCARDED_F++;
    sleep(1); //Tries to enter every second.
  }
  free(r);
  printf("fds");
  return NULL;

}

int main(int argc, char* argv[]){

  //gettimeofday(&start, NULL); //TODO: Should probably be moved. what is this LUL --conde

  //Seed randomization for RNG.
  time_t t;
  srand((unsigned) time(&t));

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

  int requests = atoi(argv[1]);
  MAX_DURATION = atoi(argv[2]);


  pthread_t gen_tid, rej_tid;

  pthread_create(&gen_tid, NULL, requestsThread, (void*) &requests);
  pthread_create(&rej_tid, NULL, rejectedListener, NULL);

  pthread_join(gen_tid, NULL);
  pthread_join(rej_tid, NULL);

  unlink(GENERATE_FIFO); //This should probably be an exit handler.

  pthread_exit(NULL);
}
