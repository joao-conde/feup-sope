#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

int SAUNA_CAPACITY, SAUNA_VACANT;
char SAUNA_GENDER;
int RECEIVED_F, RECEIVED_M, REJECTIONS_F, REJECTIONS_M, SERVED_F, SERVED_M;

char* GENERATE_FIFO = "/tmp/entrada";
char* REJECTED_FIFO = "/tmp/rejeitados";

int REJECTED_FD;

typedef struct{
  int id;
  char gender;
  int duration;
  int denials;
} Request;

void* saunaTicket(void* arg){

  Request* r = (Request*) arg;
  int serveUser = 0;

  pthread_mutex_lock(&mutex);
  if (r->gender == SAUNA_GENDER || SAUNA_GENDER == 'E'){
    if (SAUNA_VACANT > 0){
      SAUNA_VACANT--; //Decrements the available seat counter.
      SAUNA_GENDER = r->gender;
      serveUser = 1;
    }
    else{
      printf("SAUNA FULL\n");
    }
  }
    else{
      r->denials++;
      write(REJECTED_FD, r, sizeof(Request));
    }

  pthread_mutex_unlock(&mutex);

  if(serveUser == 1){
    printf("Serving user #%d, a %s, for %d ms.\n", r->id, &r->gender,r->duration);
    sleep(r->duration);

    pthread_mutex_lock(&mutex2);
    SAUNA_VACANT++;
    if(SAUNA_VACANT == SAUNA_CAPACITY)
      SAUNA_GENDER = 'E';
    pthread_mutex_unlock(&mutex2);

    printf("User #%d leaving sauna.\n", r->id);
  }

  return NULL;
}



void* requestHandler(void* arg){
  pthread_t tid[64];
  int current = 0;
  int fifo_fd;

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

  while(1){

    Request* r = malloc(sizeof(Request));

    if(read(fifo_fd, r, sizeof(Request)) == 0)
      continue;


    pthread_create(&tid[current],NULL,saunaTicket, (void*)r);
    current++;
  }


  for(int i = 0; i < sizeof(tid); i++)
    pthread_join(tid[i],NULL);

  return NULL;
}

int main(int argc, char* argv[]){

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


  printf("\nSAUNA CAPACITY: %d\n\n", SAUNA_CAPACITY);

  pthread_t req_tid;
  pthread_create(&req_tid, NULL, requestHandler, NULL);

  pthread_join(req_tid, NULL);

  pthread_exit(NULL);
}
