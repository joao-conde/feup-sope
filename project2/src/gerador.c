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
int MAX_DURATION, REQUESTS;
int GENERATED_M = 0, GENERATED_F = 0, REJECTED_M = 0, REJECTED_F = 0, DISCARDED_M = 0, DISCARDED_F = 0;

//FIFO and log files & descriptors
char* GENERATE_FIFO = "/tmp/entrada";
char* REJECTED_FIFO = "/tmp/rejeitados";
char* LOG_MSG_PATH;
int GENERATE_FD;
FILE* LOG_FILE;

struct timeval start, stop;

char* tip[] = {"REQUESTED", "REJECTED", "DISCARDED"};

typedef struct{
  int id;
  char gender;
  int duration;
  int denials;
} Request;

/* REQUEST GENERATOR THREAD */
void* requestsThread(void* arg){

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

  //Sends amount of requests to read.
  write(GENERATE_FD, &REQUESTS, sizeof(int));

  //Generates 'requests' number of random requests.
  for (int i = 0; i < REQUESTS; i++){
    Request* request = malloc(sizeof(Request));

    request->id = ID++;
    request->gender = (rand() % 2) ? 'M' : 'F';
    request->duration = rand() % MAX_DURATION + 1;
    request->denials = 0;

    if (request->gender == 'M') GENERATED_M++;
    else GENERATED_F++;

    //Logs the operation.
    gettimeofday(&stop, NULL);
    float elapsed = (float) ((float) stop.tv_usec - start.tv_usec) / 1000;

    LOG_FILE = fopen(LOG_MSG_PATH, "a"); //Opens log file.
    fprintf(LOG_FILE, "%4.2f - %4d - %2d - %c - %5d - %9s\n", elapsed, getpid(), request->id, request->gender, request->duration, tip[0]);
    fclose(LOG_FILE); //Closes log file.

    write(GENERATE_FD, request, sizeof(Request));

    free(request);
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

    gettimeofday(&stop, NULL);
    float elapsed = (float) ((float) stop.tv_usec - start.tv_usec) / 1000;

    LOG_FILE = fopen(LOG_MSG_PATH, "a"); //Opens log file.
    fprintf(LOG_FILE, "%4.2f - %4d - %2d - %c - %5d - %9s\n", elapsed, getpid(), r->id, r->gender, r->duration, tip[1]);
    fclose(LOG_FILE); //Closes log file.

    if (r->gender == 'M') REJECTED_M++;
    else REJECTED_F++;

    if (r->denials < 3){
      write(GENERATE_FD, r, sizeof(*r));
    } else {
      gettimeofday(&stop, NULL);
      float elapsed = (float) ((float) stop.tv_usec - start.tv_usec) / 1000;

      LOG_FILE = fopen(LOG_MSG_PATH, "a"); //Opens log file.
      fprintf(LOG_FILE, "%4.2f - %4d - %2d - %c - %5d - %9s\n", elapsed, getpid(), r->id, r->gender, r->duration, tip[2]);
      fclose(LOG_FILE); //Closes log file.

      if (r->gender == 'M') DISCARDED_M++;
      else if (r->gender == 'F') DISCARDED_F++;
    }
    sleep(1); //Tries to enter every second.
  }
  free(r);

  return NULL;
}

int main(int argc, char* argv[]){

  gettimeofday(&start, NULL); //Start counting time.

  time_t t;
  srand((unsigned) time(&t)); //Seed randomization for RNG.

  //Processes arguments and handles incorrect syntax.
  if(argc != 3){
    printf("Wrong number of arguments. USAGE: program_name <number of requests> <max duration>\n");
    exit(-1);
  }
  REQUESTS = atoi(argv[1]);
  MAX_DURATION = atoi(argv[2]);

  //Creates registry messages path.
  char pid_str[32], endString[64] = "/tmp/ger.";
  sprintf(pid_str, "%d", getpid());
  strcat(endString, pid_str);
  LOG_MSG_PATH = (char*) malloc(strlen(endString) + 1);
  strcpy(LOG_MSG_PATH, endString);

  //Creates every thread.
  pthread_t gen_tid, rej_tid;

  pthread_create(&gen_tid, NULL, requestsThread, NULL);
  pthread_create(&rej_tid, NULL, rejectedListener, NULL);

  pthread_join(gen_tid, NULL);
  pthread_join(rej_tid, NULL);

  printf("\n------------------[ FINAL GERADOR STATS ]------------------\n\n");
  printf("Generated: TOTAL (%d), MALE (%d), FEMALE (%d)\n", GENERATED_M + GENERATED_F, GENERATED_M, GENERATED_F);
  printf("Rejected: TOTAL (%d), MALE (%d), FEMALE (%d)\n", REJECTED_M + REJECTED_F, REJECTED_M, REJECTED_F);
  printf("Discarded: TOTAL (%d), MALE (%d), FEMALE (%d)\n\n", DISCARDED_M + DISCARDED_F, DISCARDED_M, DISCARDED_F);
  printf("-----------------------------------------------------------\n\n");

  unlink(GENERATE_FIFO);

  pthread_exit(NULL);
}
