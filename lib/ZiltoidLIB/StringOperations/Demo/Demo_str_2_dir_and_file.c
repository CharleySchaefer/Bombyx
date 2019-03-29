#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_STR_L 1000

#include "../StringOperations.h"


int main()
{
  int i,ind1,ind2;
  char c;
  const char *str="./dir/file.txt";
  char *dir, *file;

  dir =(char*)malloc(MAX_STR_L*sizeof(char));
  file=(char*)malloc(MAX_STR_L*sizeof(char));

  
  str_2_dir_and_file(str, dir, file);
  printf("str:  %s\n", str );
  printf("dir:  %s\n", dir );
  printf("file: %s\n", file);
  
}
