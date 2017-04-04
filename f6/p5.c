#include <stdio.h>
#include <pthread.h>


void * sum_thread(void* arg){
    printf("I'm the sum thread\n");
    int* res = malloc(sizeof(int));
    printf("%d %d\n", (*(int*)arg), (*(int*)arg+1));

    *res = (*((int*)arg)) + (*(int*)arg+1);
    return res;
    printf("%d\n", *res);
}

void * sub_thread(void* arg){

}

void * mult_thread(void* arg){

}

void * div_thread(void* arg){

}



int main(){

  float n1, n2;
  int sum[2], sub[2], mult[2];

  float* divRes;
  int* sumRes, subRes, multRes;

  pthread_t sumtid,subtid,divtid,multtid;

  struct info{
    float n1, n2;
  } division;



  printf("Insert 2 operands:");
  scanf("%f %f", &n1, &n2);

  //filling data structures
  sum[0] = n1; sum[1] = n2;
  sub[0] = n1; sub[1] = n2;
  mult[0] = n1; mult[1] = n2;
  division.n1 = n1; division.n2 = n2;



  pthread_create(&sumtid,NULL,sum_thread,&sum);
  pthread_join(sumtid,&sumRes);

  pthread_create(&subtid,NULL,sub_thread,&sub);
  pthread_create(&multtid,NULL,mult_thread,&mult);
  pthread_create(&divtid,NULL,div_thread,&division);

  printf("%d\n", sumRes);



  /*
  printf("DIVISION: %f / %f\n", division.n1, division.n2);
  printf("MULT: %d * %d\n", (int)mult[0], (int)mult[1]);
  printf("SUM: %d + %d\n", (int)sum[0], (int)sum[1]);
  printf("SUB: %d - %d\n", (int)sub[0], (int)sub[1]);
  */

  return 0;
}
