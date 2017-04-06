#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


struct Info{
  float n1, n2;
};




void * sum_thread(void* arg){

    printf("I'm the sum thread\n");
    int* res = malloc(sizeof(int));
    printf("%d %d\n", *((int*)arg),   *(((int*)arg)+1)  );

    *res = (*(int*)arg) + (*((int*)arg+1));
    printf("sum: %d\n", *res);
    return res;
}

void * sub_thread(void* arg){
  printf("I'm the sub thread\n");
  int* res = malloc(sizeof(int));
  printf("%d %d\n", *((int*)arg), *(((int*)arg)+1)  );

  *res = (*(int*)arg) - (*((int*)arg+1));
  printf("sub: %d\n", *res);
  return res;
}

void * mult_thread(void* arg){
  printf("I'm the mult thread\n");
  int* res = malloc(sizeof(int));
  printf("%d %d\n", *((int*)arg),   *(((int*)arg)+1)  );

  *res = (*(int*)arg) * (*((int*)arg+1));
  printf("mult: %d\n", *res);
  return res;
}

void * div_thread(void* arg){

  struct Info division;
  float* res = malloc(sizeof(float));

  printf("I'm the div thread\n");


  printf("%d %d\n", *((struct Info*)arg).n1, 6 );

  *res = (*(float*)arg) / (*((float*)arg+1));
  printf("div: %f\n", *res);
  return res;
}



int main(){

  float n1, n2;
  int sum[2], sub[2], mult[2];

  float* divRes;
  int* sumRes;
  int* subRes;
  int* multRes;

  pthread_t sumtid,subtid,divtid,multtid;


  struct Info division;

  printf("Insert 2 operands:");
  scanf("%f %f", &n1, &n2);

  //filling data structures
  sum[0] = n1; sum[1] = n2;
  sub[0] = n1; sub[1] = n2;
  mult[0] = n1; mult[1] = n2;
  division.n1 = n1; division.n2 = n2;


  printf("%d %d\n", division.n1, division.n2);

  pthread_create(&sumtid,NULL,sum_thread,&sum);
  pthread_join(sumtid,(void*)&sumRes);

  pthread_create(&subtid,NULL,sub_thread,&sub);
  pthread_join(subtid,(void*)&subRes);

  pthread_create(&multtid,NULL,mult_thread,&mult);
  pthread_join(multtid,(void*)&multRes);

  pthread_create(&divtid,NULL,div_thread,&division);
  pthread_join(divtid,(void*)&divRes);

  printf("SUM thread calculated: %d\n", *sumRes);
  printf("SUB thread calculated: %d\n", *subRes);
  printf("MULT thread calculated: %d\n", *multRes);
  printf("DIV thread calculated: %d\n", *sumRes);




  /*
  printf("DIVISION: %f / %f\n", division.n1, division.n2);
  printf("MULT: %d * %d\n", (int)mult[0], (int)mult[1]);
  printf("SUM: %d + %d\n", (int)sum[0], (int)sum[1]);
  printf("SUB: %d - %d\n", (int)sub[0], (int)sub[1]);
  */

  return 0;
}
