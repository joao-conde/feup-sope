#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[]){

FILE *f1, *f2;
char buffer[100];

f1 = fopen(argv[1], "r");
f2 = fopen(argv[2], "w");

  while(1){

    int nr = fread(&buffer,sizeof(buffer),1,&f1);

    if(nr == sizeof(buffer)){
      fwrite(&buffer, sizeof(buffer), 1, &f2);
    }
    else{
      fwrite(&buffer, nr, 1, &f2);
      fclose(f1);
      fclose(f2);
      return 0;
    }
  }
}
