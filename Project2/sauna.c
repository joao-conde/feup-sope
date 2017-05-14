#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

#define SEC_TO_NANO 1000000

//Mutex initializer.
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Sauna specific variables.
int SAUNA_CAPACITY, SAUNA_VACANT;
char SAUNA_GENDER;
int RECEIVED_F, RECEIVED_M, REJECTED_F, REJECTED_M, SERVED_F, SERVED_M;

//FIFO path and descriptors
char* GENERATE_FIFO = "/tmp/entrada";
char* REJECTED_FIFO = "/tmp/rejeitados";
int REJECTED_FD;

//Log file path and descriptor.
char* LOG_MSG_PATH;
FILE* LOG_FILE;
char* tip[] = {"RECEIVED", "REJECTED", "SERVED"};
struct timeval start, stop; //Structs to track time interval.

//Request structure.
typedef struct{
  int id;
  char gender;
  int duration;
  int denials;
} Request;

void* saunaTicket(void* arg){

  Request* r = (Request*) arg;
  printf("Serving user #%d, a %s, for %d ms.\n", r->id, &r->gender, r->duration);

  struct timespec use = {0};
  use.tv_nsec = r->duration * SEC_TO_NANO;
  nanosleep(&use, NULL);

  pthread_mutex_lock(&mutex); //Enters critical section.
  SAUNA_VACANT++;
  if (SAUNA_VACANT == SAUNA_CAPACITY){
     SAUNA_GENDER = 'E';
  }
  pthread_mutex_unlock(&mutex); //Exits critical section.

  printf("User #%d leaving sauna.\n", r->id);

  //Logs the request.
  gettimeofday(&stop, NULL); //Stops counting time.
  float elapsed = (float) ((float) stop.tv_usec - start.tv_usec) / 1000;
  LOG_FILE = fopen(LOG_MSG_PATH, "a"); //Opens log file.
  fprintf(LOG_FILE, "%4.2f - %4d - %15lu - %2d - %c - %5d - %8s\n", elapsed, getpid(), pthread_self(), r->id, r->gender, r->duration, tip[2]);
  fclose(LOG_FILE); //Closes log file.

  pthread_exit(NULL);
}

void* requestHandler(void* arg){
  pthread_t tid[64];
  int current = 0;
  int fifo_fd;
  int recycle = 1;

  while ((fifo_fd = open(GENERATE_FIFO, O_RDONLY)) == -1){
    if(errno == ENOENT || errno == ENXIO){
      printf("Retrying...\n");
      sleep(1);
    }
    else {
      perror("ERROR DESCRIPTION");
      exit(-1);
    }
  }
  while(recycle){
    Request* r = malloc(sizeof(Request));

    //Handles a new request.
    if (read(fifo_fd, r, sizeof(Request)) != 0){

      gettimeofday(&stop, NULL); //Stops counting time.
      float elapsed = (float) ((float) stop.tv_usec - start.tv_usec) / 1000;
      LOG_FILE = fopen(LOG_MSG_PATH, "a"); //Opens log file.
      fprintf(LOG_FILE, "%4.2f - %4d - %15lu - %2d - %c - %5d - %8s\n", elapsed, getpid(), pthread_self(), r->id, r->gender, r->duration, tip[0]);
      fclose(LOG_FILE); //Closes log file.

      if ((r->gender == SAUNA_GENDER || SAUNA_GENDER == 'E') && SAUNA_VACANT > 0){
        SAUNA_VACANT--; //Decrements the available seat counter.
        SAUNA_GENDER = r->gender;
        pthread_create(&tid[current], NULL, saunaTicket, (void*) r);

        current++;
      } else {
        r->denials++;
        write(REJECTED_FD, r, sizeof(Request));

        //Logs the request.
        gettimeofday(&stop, NULL); //Stops counting time.
        float elapsed = (float) ((float) stop.tv_usec - start.tv_usec) / 1000;
        LOG_FILE = fopen(LOG_MSG_PATH, "a"); //Opens log file.
        fprintf(LOG_FILE, "%4.2f - %4d - %15lu - %2d - %c - %5d - %8s\n", elapsed, getpid(), pthread_self(), r->id, r->gender, r->duration, tip[1]);
        fclose(LOG_FILE); //Closes log file.
      }
    }
      else {
        printf("changing recycle variable\n");
        recycle = 0;
      }
  }
  printf("CLOSED\n");
  close(REJECTED_FD);

  //Waits for the ending of every ticket.
  for(int i = 0; i < sizeof(tid) / sizeof(pthread_t); i++){
    printf("WAITING FOR %d", i);
    pthread_join(tid[i],NULL);
  }
  pthread_exit(NULL);
}

int main(int argc, char* argv[]){

  gettimeofday(&start, NULL); //Start counting time.

  if(argc != 2){
    printf("Wrong number of arguments! USAGE: sauna <max sauna users>\n");
    exit(-1);
  }

  if(mkfifo(REJECTED_FIFO, S_IRUSR | S_IWUSR) != 0 && errno != EEXIST){
    printf("Error creating REJECTED fifo\n");
    exit(-1);
  }

  while ((REJECTED_FD = open(REJECTED_FIFO, O_WRONLY)) == -1){
    if(errno == ENOENT || errno == ENXIO){
      printf("Retrying...\n");
      sleep(1);
    }
    else {
      perror("ERROR DESCRIPTION");
      exit(-1);
    }
  }

  SAUNA_CAPACITY = atoi(argv[1]);
  SAUNA_VACANT = SAUNA_CAPACITY;
  SAUNA_GENDER = 'E'; //(E)mpty sauna

  //Creates registry messages path.
  char pid_str[32], endString[64] = "/tmp/bal.";
  sprintf(pid_str, "%d", getpid());
  strcat(endString, pid_str);
  LOG_MSG_PATH = (char*) malloc(strlen(endString) + 1);
  strcpy(LOG_MSG_PATH, endString);

  printf("\nSAUNA CAPACITY: %d\n\n", SAUNA_CAPACITY);
  /*
  printf("--------: FINAL SAUNA.C STATS :--------\n");
  printf("Received: TOTAL (%d), MALE (%d), FEMALE (%d)\n", RECEIVED_M + RECEIVED_F, RECEIVED_M, RECEIVED_F);
  printf("Rejected: TOTAL (%d), MALE (%d), FEMALE (%d)\n", REJECTED_M + REJECTED_F, REJECTED_M, REJECTED_F);
  printf("Served: TOTAL (%d), MALE (%d), FEMALE (%d)\n", SERVED_M + SERVED_F, SERVED_M, SERVED_F);
  */
  pthread_t req_tid;
  pthread_create(&req_tid, NULL, requestHandler, NULL);
  pthread_join(req_tid, NULL);

  pthread_exit(NULL);
}
