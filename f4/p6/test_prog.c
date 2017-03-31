
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]){

  for(int i = 0; i < 6; i++){
    printf("%s\n" , argv[1]);
    sleep(5);
  }
  return 0;
}
