// PROGRAMA p1.c
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int global=1;

int main(void) {
  int local = 2;
  if(fork() > 0) {
    printf("PID = %d; PPID = %d\n", getpid(), getppid());
    global++;
    local--;
  } else {
    printf("PID = %d; PPID = %d\n", getpid(), getppid());
    global--;
    local++;
  }
  printf("PID = %d - global = %d ; local = %d\n", getpid(), global, local);
  return 0;
}
