#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define MAX 10


int main(void) {

 int i;
 pid_t pid;
 char str[10];
 pid=fork();
 switch (pid) {
         case -1:
            perror("fork");
            break;
         case  0: //filho
            for (i=1; i<=MAX; i++) {
              sprintf(str,"-%d",i);
              write(STDOUT_FILENO,str,strlen(str));
            }
            break;
         default: //pai
            for (i=1; i<=MAX; i++) {
              sprintf(str,"+%d",i);
              write(STDOUT_FILENO,str,strlen(str));
            }
 }
  return 0;
}
