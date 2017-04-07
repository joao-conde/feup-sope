#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


struct Flags{

  char* name;
  char* type;
  int mode;

  int print;
  int delete;

  //falta ainda o exec lul fuk lulz

} flags;


void processArgs(int argc, char* argv[]){

  flags.name = "";
  flags.type = "$";
  flags.mode = 022;
  flags.print = 0;
  flags.delete = 0;

  for(int i = 0; i < argc; i++){

    if(strcmp(argv[i],"-name") == 0)
      flags.name = argv[i+1];

    if(strcmp(argv[i],"-type") == 0)
      flags.type = argv[i+1];

    if(strcmp(argv[i],"-perm") == 0)
      flags.mode = strtoul(argv[i+1],NULL,8);

    if(strcmp(argv[i],"-print") == 0)
      flags.print = 1;

    if(strcmp(argv[i],"-delete") == 0)
      flags.delete = 1;
  }

}

void searchDir(const char* path){

  struct stat fileStatus;


  //  process files
  printf("path: %s\n", path);

  stat(path,&fileStatus);
  if(S_ISDIR(fileStatus.st_mode))
    printf("I'm a directory! :-)\n");
  //  process dir (call searchDir again lulz)
}

int main(int argc, char* argv[]){

  char buffer[256];

  processArgs(argc,argv);

  getcwd(buffer,sizeof(buffer));
  searchDir(buffer);



  //test if variables are correct
  printf("path: %s\n", buffer);
  printf("Name argument: %s\n", flags.name);

  printf("Type argument: %s\n", flags.type);

  printf("Perm argument: %o\n", flags.mode);

  return 0;
}
