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

#define MS_TO_MICRO 1000

//Mutex initializer.
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Sauna specific variables.
int SAUNA_CAPACITY, SAUNA_VACANT;
char SAUNA_GENDER;
int RECEIVED_F, RECEIVED_M, REJECTED_F, REJECTED_M, SERVED_F, SERVED_M;
int TO_READ;

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

  if (r->gender == 'M') SERVED_M++;
  else SERVED_F++;

  usleep(r->duration * MS_TO_MICRO); //Simulates the service.

  pthread_mutex_lock(&mutex); //Enters critical section.
  SAUNA_VACANT++;
  if (SAUNA_VACANT == SAUNA_CAPACITY){
     SAUNA_GENDER = 'E';
  }
  pthread_mutex_unlock(&mutex); //Exits critical section.

  printf("User #%d leaving sauna.\n", r->id);

  //Logs the request.
  gettimeofday(&stop, NULL); //Stops counting time.
  double elapsed = (stop.tv_sec - start.tv_sec)*1000.0f + (stop.tv_usec - start.tv_usec) / 1000.0f;
  LOG_FILE = fopen(LOG_MSG_PATH, "a"); //Opens log file.
  fprintf(LOG_FILE, "%9.2f - %4d - %15lu - %2d - %c - %5d - %8s\n", elapsed, getpid(), pthread_self(), r->id, r->gender, r->duration, tip[2]);
  fclose(LOG_FILE); //Closes log file.

  pthread_exit(NULL);
}

void* requestHandler(void* arg){
  pthread_t tid[128] = {0};
  int current = 0;
  int fifo_fd;

  while ((fifo_fd = open(GENERATE_FIFO, O_RDONLY)) == -1){
    if (errno == ENOENT || errno == ENXIO){
      printf("Retrying...\n");
      sleep(1);
    } else {
      perror("Error opening GENERATE fifo");
      exit(-1);
    }
  }

  //Gets the amount of requests to read.
  read(fifo_fd, &TO_READ, sizeof(int));

  //Generates the specified amount of requests.
  while (TO_READ){
    Request* r = malloc(sizeof(Request));

    if (r->gender == 'M') RECEIVED_M++;
    else RECEIVED_F++;

    //Handles a new request.
    if (read(fifo_fd, r, sizeof(Request)) != 0){
      TO_READ--;

      gettimeofday(&stop, NULL); //Stops counting time.
      double elapsed = (stop.tv_sec - start.tv_sec)*1000.0f + (stop.tv_usec - start.tv_usec) / 1000.0f;
      LOG_FILE = fopen(LOG_MSG_PATH, "a"); //Opens log file.
      fprintf(LOG_FILE, "%9.2f - %4d - %15lu - %2d - %c - %5d - %8s\n", elapsed, getpid(), pthread_self(), r->id, r->gender, r->duration, tip[0]);
      fclose(LOG_FILE); //Closes log file.

      if ((r->gender == SAUNA_GENDER || SAUNA_GENDER == 'E') && SAUNA_VACANT > 0){
        SAUNA_VACANT--; //Decrements the available seat counter.
        SAUNA_GENDER = r->gender;
        pthread_create(&tid[current], NULL, saunaTicket, (void*) r);

        current++;
      } else {
        r->denials++;

        write(REJECTED_FD, r, sizeof(Request));

        if (r->gender == 'M') REJECTED_M++;
        else REJECTED_F++;

        if (r->denials != 3) TO_READ++;

        //Logs the request.
        gettimeofday(&stop, NULL); //Stops counting time.
        double elapsed = (stop.tv_sec - start.tv_sec)*1000.0f + (stop.tv_usec - start.tv_usec) / 1000.0f;
        LOG_FILE = fopen(LOG_MSG_PATH, "a"); //Opens log file.
        fprintf(LOG_FILE, "%9.2f - %4d - %15lu - %2d - %c - %5d - %8s\n", elapsed, getpid(), pthread_self(), r->id, r->gender, r->duration, tip[1]);
        fclose(LOG_FILE); //Closes log file.
      }
    }
  }
  close(REJECTED_FD);

  //Waits for the ending of every ticket.
  for(int i = 0; tid[i] != 0; i++){
    pthread_join(tid[i], NULL);
  }
  return NULL;
}

int main(int argc, char* argv[]){

  gettimeofday(&start, NULL); //Start counting time.

  if (argc != 2){
    printf("Wrong number of arguments! USAGE: sauna <max sauna users>\n");
    exit(-1);
  }

  SAUNA_CAPACITY = atoi(argv[1]);
  SAUNA_VACANT = SAUNA_CAPACITY;
  SAUNA_GENDER = 'E'; //(E)mpty sauna

  if (mkfifo(REJECTED_FIFO, S_IRUSR | S_IWUSR) != 0 && errno != EEXIST){
    perror("Error creating REJECTED fifo");
    exit(-1);
  }

  while ((REJECTED_FD = open(REJECTED_FIFO, O_WRONLY)) == -1){
    if (errno == ENOENT || errno == ENXIO){
      printf("Retrying...\n");
      sleep(1);
    } else {
      perror("Error opening REJECTED fifo");
      exit(-1);
    }
  }

  //Creates registry messages path.
  char pid_str[32], endString[64] = "/tmp/bal.";
  sprintf(pid_str, "%d", getpid());
  strcat(endString, pid_str);
  LOG_MSG_PATH = (char*) malloc(strlen(endString) + 1);
  strcpy(LOG_MSG_PATH, endString);

  //Launches the request thread.
  pthread_t req_tid;
  pthread_create(&req_tid, NULL, requestHandler, NULL);
  pthread_join(req_tid, NULL);

  //Sauna statistics.
  printf("\n------------------[ FINAL SAUNA STATS ]------------------\n\n");
  printf("Received: TOTAL (%d), MALE (%d), FEMALE (%d)\n", RECEIVED_M + RECEIVED_F, RECEIVED_M, RECEIVED_F);
  printf("Rejected: TOTAL (%d), MALE (%d), FEMALE (%d)\n", REJECTED_M + REJECTED_F, REJECTED_M, REJECTED_F);
  printf("Served: TOTAL (%d), MALE (%d), FEMALE (%d)\n\n", SERVED_M + SERVED_F, SERVED_M, SERVED_F);
  printf("---------------------------------------------------------\n\n");

  pthread_exit(NULL);
}
