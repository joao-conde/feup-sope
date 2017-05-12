#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>

int SAUNA_CAPACITY, SAUNA_VACANT;
char SAUNA_GENDER;
int RECEIVED_F, RECEIVED_M, REJECTIONS_F, REJECTIONS_M, SERVED_F, SERVED_M;

char* GENERATE_FIFO = "/tmp/entrada";
char* REJECTED_FIFO = "/tmp/rejeitados";

typedef struct{
  int id;
  char gender;
  int duration;
  int denials;
} Request;

void* saunaTicket(void* arg){

  Request* r = (Request*) arg;

  printf("Serving user #%d for %d ms.\n", r->id, r->duration);
  sleep(r->duration);
  printf("User #%d leaving sauna.\n", r->id);

  return NULL;
}

void* requestHandler(void* arg){
  int fifo_fd;
  Request r;
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  while((fifo_fd = open(GENERATE_FIFO, O_RDONLY)) == -1){
    printf("No generate pipe available! Retrying...\n");
  }


  while(read(fifo_fd, &r, sizeof(Request)) != 0){
    //printf("lul\n");
    //pthread_t tid[64]; //Contains every tid. Acho que se pode usar sempre o mesmo pq nao vamos precisar de guardar o valor
    pthread_t tid;
    int curr = 0; //The current index.

    //critical section controled by a mutex

    pthread_mutex_lock(&mutex);
    printf("ticket sent\n");
    if (r.gender == SAUNA_GENDER){
      if (SAUNA_VACANT > 0){
        pthread_create(&tid, NULL, saunaTicket, (void*) &r); //Creates a new ticket.
        SAUNA_VACANT--; //Decrements the available seat counter.
        curr++;
      }
      else{
      }
    }
    else{
      //TODO: Update denial variable.
    }
  pthread_mutex_unlock(&mutex);
    //printf("ID: %d\nGender: %c\nDuration: %d\nDenials: %d\n", r.id, r.gender, r.duration, r.denials);
  }

  return NULL;
}

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
  SAUNA_VACANT = SAUNA_CAPACITY;


  printf("\nSAUNA CAPACITY: %d\n\n", SAUNA_CAPACITY);

  pthread_t req_tid;
  pthread_create(&req_tid, NULL, requestHandler, NULL);

  pthread_join(req_tid, NULL);

  pthread_exit(NULL);
}
