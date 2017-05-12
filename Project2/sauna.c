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
  usleep(r->duration);
  printf("User #%d leaving sauna.\n", r->id);

  pthread_exit(NULL);
}

void* requestHandler(void* arg){
  int fifo_fd;
  Request* r;

  while ((fifo_fd = open(GENERATE_FIFO, O_RDONLY)) == -1){
    if (errno == ENOENT) printf("No generate pipe available! Retrying...\n");
  }

  while(read(fifo_fd, r, sizeof(Request)) != 0){
    pthread_t tid[64]; //Contains every tid.
    int curr = 0; //The current index.

    Request* request = r;

    if (r->gender == SAUNA_GENDER){
      if (SAUNA_VACANT > 0){
        pthread_create(&tid[curr], NULL, saunaTicket, (void*) &request); //Creates a new ticket.
        SAUNA_VACANT--; //Decrements the available seat counter.
        curr++;
      }
      else{
      }
    }
    else{
      //TODO: Update denial variable.
    }
    printf("ID: %d\nGender: %c\nDuration: %d\nDenials: %d\n", r->id, r->gender, r->duration, r->denials);
  }

  pthread_exit(NULL);
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

  int fifo_fd;
  Request *r = malloc(sizeof(Request));

  printf("\nSAUNA CAPACITY: %d\n\n", SAUNA_CAPACITY);

  pthread_t req_tid, ticket_tid;

  pthread_create(&req_tid, NULL, requestHandler, NULL);
  pthread_create(&ticket_tid, NULL, saunaTicket, NULL);

  pthread_join(req_tid, NULL);
  pthread_join(ticket_tid, NULL);

  return 0;
}
