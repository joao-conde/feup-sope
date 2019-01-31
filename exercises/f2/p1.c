#include <termios.h>
#include <unistd.h>
#include <string.h>

#define maxLength  20

int main(void){


struct termios oldterm, newterm;
char echo = '*', ch;
char pass[maxLength];

write(STDOUT_FILENO, "\nPassword? ", 11);

tcgetattr(STDIN_FILENO, &oldterm);

newterm=oldterm;
newterm.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON);

tcsetattr(STDIN_FILENO, TCSAFLUSH, &newterm);

read(STDIN_FILENO, &ch, 1);
for(int i = 0; ch != '\n' && i < maxLength;  i++){

	write(STDOUT_FILENO, &echo, 1);
	pass[i] = ch;
	read(STDIN_FILENO, &ch, 1);

}

tcsetattr(STDIN_FILENO, TCSANOW, &oldterm);

write(STDOUT_FILENO, "\n", 1);
write(STDOUT_FILENO, &pass, 10 );

write(STDOUT_FILENO, "\n", 1);

return 0;

}
