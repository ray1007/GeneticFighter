#include <stdio.h>
#include <stdlib.h>


int main( int argc, char *argv[] )
{

  FILE *fp;
  char path[1035];

  /* Open the command for reading. */
  fp = _popen("lf2.exe", "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }
  printf("%s",path);
  /* Read the output a line at a time - output it. */
  while (fgets(path, sizeof(path)-1, fp) != NULL) {
    printf("%s", path);
  }

  /* close */
  printf("Starttttttttttt\n");
  _pclose(fp);

  return 0;
}