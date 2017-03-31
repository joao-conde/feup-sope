#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){

  char* name_list[50];
  int fd;

  if(argc != 2){
    printf("Wrong format... USAGE: p3a [FILE NAME]\n");
    exit(1);
  }

  if((fd = open(argv[1], O_RDONLY)) == -1){
    perror("Error openning file");
    exit(2);
  }


  int index = 0;
  for(int n = read(fd, name_list[index], 10); n != 0; n = read(fd, name_list[index], 10)){
    index++;
  }

  for(int i = 0; i < sizeof(name_list); i++){
    printf("%s\n", name_list[i]);
  }


  return 0;
}
