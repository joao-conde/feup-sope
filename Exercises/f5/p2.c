#include <stdio.h>
#include <unistd.h>
#include <wait.h>


#define READ  0
#define WRITE 1

#define INT      0
#define FLOAT    1
#define INVALID -1


struct Package{
  int n1;
  int n2;
  int sum;
  int sub;
  int division;
  int mult;
};


char* type_conversion(int type){

  char* integer = "INTEGER";
  char* myFloat = "FLOAT";
  char* invalid = "INVALID";

  switch(type){
    case INT:
      return integer;

    case FLOAT:
      return myFloat;

    case INVALID:
      return invalid;
    }

    return invalid;

}

int main(){

  int fd_PC[2]; //PARENT-CHILD pipe
  int fd_CP[2]; //CHILD-PARENT pipe
  struct Package info;

  if(pipe(fd_PC) < 0)
    perror("Error creating pipe parent-child");

  if(pipe(fd_CP) < 0)
    perror("Error creating pipe child-parent");

  switch(fork()){

    case -1:
      perror("Error in fork()");
      break;

    case 0: //child process;

      close(fd_PC[WRITE]);
      read(fd_PC[READ],&info,sizeof(info));
      close(fd_PC[READ]);

      printf("\n----RESULT VALUES----\n\n");
      printf("SUM: %d\n", info.n1 + info.n2);
      info.sum = INT;

      printf("SUB: %d\n", info.n1 - info.n2);
      info.sub = INT;

      printf("MULT: %d\n", info.n1 * info.n2);
      info.mult = INT;

      if(info.n2 == 0){
        printf("Dividend must not be 0!\n");
        info.division = INVALID;
      }
      else{
        if( (info.n1 % info.n2) == 0){
          info.division = INT;
        }
        else info.division = FLOAT;

        printf("DIV: %f\n", (float)info.n1 / (float)info.n2);
      }

      close(fd_CP[READ]);
      write(fd_CP[WRITE], &info, sizeof(info));
      close(fd_CP[WRITE]);

      break;

    default: //parent process
      close(fd_PC[READ]);
      printf("Numbers (n1 n2):");
      scanf("%d %d", &info.n1, &info.n2);
      write(fd_PC[WRITE],&info,sizeof(info));
      close(fd_PC[WRITE]);

      wait(NULL);


      close(fd_CP[WRITE]);
      read(fd_CP[READ], &info, sizeof(info));
      printf("\n----RESULT VALUES TYPE----\n\n");
      printf("Sum value is of type: %s\n", type_conversion(info.sum));
      printf("Subtraction value is of type: %s\n", type_conversion(info.sub));
      printf("Multiplaction value is of type: %s\n", type_conversion(info.mult));
      printf("Division value is of type: %s\n\n", type_conversion(info.division));
      close(fd_CP[READ]);

  }

  return 0;
}
