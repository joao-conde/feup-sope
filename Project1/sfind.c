#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
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

void searchDir(char* path){

  struct stat fileStatus;
  struct dirent *fileEntry;
  DIR* dp;



  //  process files
  //printf("path: %s\n\n", path);

  if(stat(path,&fileStatus) != 0)
    printf("stat error\n");

  if(S_ISDIR(fileStatus.st_mode)){

    if((dp = opendir(path)) == NULL)
      printf("Error opendir\n");

      char newPath[256];

    while((fileEntry = readdir(dp)) != NULL){

      strcpy(newPath,path);

      if(strcmp(fileEntry->d_name,".") == 0 || strcmp(fileEntry->d_name, "..") == 0){
        continue;
      }

      strcat(newPath,"/");
      strcat(newPath,fileEntry->d_name);
      //printf("new path: %s\n\n", newPath);
      stat(newPath,&fileStatus);

      if(S_ISREG(fileStatus.st_mode)){
        printf("%s\n",newPath);
      }

      if(S_ISDIR(fileStatus.st_mode)){
        printf("%s\n",newPath);
        switch(fork()){
          case -1:
            printf("Error fork()...\n");
            break;

          case 0: //child process
            searchDir(newPath);
            break;

        }
      }

    }
    printf("LUL: %s\n", newPath);
    closedir(dp);


  }
  //  process dir (call searchDir again lulz)
}

int main(int argc, char* argv[]){

  char* path = NULL;

  processArgs(argc,argv);



  path = getcwd(path,256);
  printf("initial path: %s\n\n", path);
  searchDir(path);



  //test if variables are correct
  /*
  printf("path: %s\n", buffer);
  printf("Name argument: %s\n", flags.name);

  printf("Type argument: %s\n", flags.type);

  printf("Perm argument: %o\n", flags.mode);
  */

  return 0;
}
