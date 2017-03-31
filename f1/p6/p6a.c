#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUF_LENGTH 256

int main(int argc, char** argv)
{

  FILE *src , *dst;
  char buf[BUF_LENGTH];

  if ( ( src = fopen( argv[1], "r" ) ) == NULL )
  {

   //perror("Unlucky openning input file");
   // printf("%s\n",strerror(3)) );
   //printf("%s\n",strerror(errno));
   perror("usage: p6a f1 f2");
   exit(1);
  }
  if ( ( dst = fopen( argv[2], "w" ) ) == NULL )
  {
    //perror("Unlucky openning output file");
    perror("usage: p6a f1 f2");
    exit(2);
  }


  while( ( fgets( buf, BUF_LENGTH, src ) ) != NULL )
  {
    fputs( buf, dst );
  }

  fclose( src );
  fclose( dst );

  exit(0);  // zero é geralmente indicativo de "sucesso"
}
