#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>


struct Flags{
  char* name;
  char* type;
  int mode;
  int print;
  int delete;
  //TODO: -exec command.
} flags;

void sigint_handler(int signo){

  char terminate;

  kill(getppid(),SIGSTOP);

  printf("\n> DO YOU WISH TO TERMINATE?\n");
  scanf("%s",&terminate);

  //kill(getpid(),SIGSTOP);
  terminate = (char)toupper(terminate);

  if(terminate == 'N'){
    kill(getppid(),SIGCONT);
    //kill(getpid(),SIGCONT);
  }
  else{
    kill(getppid(),SIGQUIT);
    kill(getpid(),SIGQUIT);
  }

}

void subscribe_SIGINT(){

  struct sigaction act;
  act.sa_handler = sigint_handler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);

  sigaction(SIGINT,&act,NULL);

}

void processArgs(int argc, char* argv[]){

  flags.name = "";
  flags.type = "$";
  flags.mode = 022;
  flags.print = 0;
  flags.delete = 0;

  for(size_t i = 0; i < argc; i++){
    if (strcmp(argv[i],"-name") == 0) flags.name = argv[i+1];
    if (strcmp(argv[i],"-type") == 0) flags.type = argv[i+1];
    if (strcmp(argv[i],"-perm") == 0) flags.mode = strtoul(argv[i+1], NULL, 8);
    if (strcmp(argv[i],"-print") == 0) flags.print = 1;
    if (strcmp(argv[i],"-delete") == 0) flags.delete = 1;
  }

}

void searchDir(char* path){

  struct stat fileStatus;
  struct dirent *fileEntry;
  DIR* dp;
  pid_t pid;

  subscribe_SIGINT(); //Ctrl+C interruption
  sleep(4);
  //uncomment to test CTR C

  if (stat(path,&fileStatus) != 0) printf("stat error\n");

  if (S_ISDIR(fileStatus.st_mode)){

    if ((dp = opendir(path)) == NULL) printf("Error opendir\n");

    char newPath[256];

    while ((fileEntry = readdir(dp)) != NULL){

      //Ignore the current directory and previous directory folders.
      if (strcmp(fileEntry->d_name, ".") == 0 || strcmp(fileEntry->d_name, "..") == 0){
        continue;
      }
      //Update the current path.
      strcpy(newPath, path);
      strcat(newPath, "/");
      strcat(newPath, fileEntry->d_name);
      stat(newPath, &fileStatus);

      //Whether the read structure is a file.
      if (S_ISREG(fileStatus.st_mode)){
        if (flags.print == 1 && (strcmp(flags.type, "f") == 0 || strcmp(flags.name, fileEntry->d_name) == 0 || flags.mode == fileStatus.st_mode)){
            printf("%s\n", newPath);
        }
      }
      //Whether the read structure is a directory.
      if (S_ISDIR(fileStatus.st_mode)){
        if (flags.print == 1 && strcmp(flags.type, "d")){
            printf("%s\n", newPath);
        }
        switch (pid = fork()){
          case -1:
            perror("Error fork()...\n");
            break;

          case 0: //child process
            searchDir(newPath);
            break;

          /*default:
              waitpid(pid,NULL,WNOHANG);*/

        }
      }
    }
    closedir(dp);
  }
  //  process dir (call searchDir again lulz)
}


int main(int argc, char* argv[]){

  char* path = getcwd(NULL, 256); //Saves the current path.

  processArgs(argc,argv);

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
