#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int ID = 1;

int GENERATED_M, GENERATED_F, REJECTED_M, REJECTED_F, DISCARDED_M, DISCARDED_F;

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

  int requests, maxDuration;
  time_t t;

  if(argc != 3){
    printf("Wrong number of arguments. USAGE: program_name <number of requests> <max duration>\n");
    exit(-1);
  }

  srand((unsigned) time(&t));

  requests = atoi(argv[1]);
  maxDuration = atoi(argv[2]);


  Request* r = generateRequest(maxDuration);
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

  r = generateRequest(maxDuration);
  printf("struct ID: %d\n", r->id);
  printf("struct gender: %s\n", &r->gender);
  printf("struct duration: %d\n", r->duration);
  printf("struct denials: %d\n", r->denials);





  return 0;
}
