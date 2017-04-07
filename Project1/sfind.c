

int main(int argc, char* arv[]){

  pid_t pid;


  switch (pid=fork()) {

    case -1:
      printf("Error in fork() call...\n");
      exit(1);

    case 0: //child process
      break;

    default: //parent process

  }

  return 0;
}
