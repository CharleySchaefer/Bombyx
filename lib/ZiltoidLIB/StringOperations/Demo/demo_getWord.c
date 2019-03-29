#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_STR_L 1000

#include "../StringOperations.h"


int main()
{
  int i,ind1,ind2;
  char c;

  char line[100];
  const char *delim=" \t";
  char *token;

  sprintf(line,  "this	is a test string\n");

  // char *strtok(char *str, const char *delim)


  token=strtok(line, delim); // first word in line
  while( token != NULL ) {
      printf( " %s\n", token );
      token = strtok(NULL, delim); // next word in line
   }
}
