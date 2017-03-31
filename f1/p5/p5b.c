#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

int main(int argc, char** argv, char** env)
{



  printf("Hello %s!\n", getenv("USER_NAME"));

  /*for(int i = 0; env[i] !=  NULL ; i++){

    if(strncmp(env[i],"USER", 4) == 0){
      printf("Hello %s!\n", env[i] + 5);
    }
  }*/



  return 0;
}
