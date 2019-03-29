/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <complex.h>
#include <string.h>
#include <time.h>



#include "../src/Import/ReadBBXdata.h"


int main(int argc, char *argv[])
{
  int verbose=1;
  INPUT  *Set=(INPUT*)malloc(sizeof(INPUT)); // Program input
  char *inputfile;

  //----------------------------------------------------------------------
  if(argc<2)
  {printf("ERROR: No input file specified - terminating.\n"); exit(1);}
  inputfile=argv[1];
  //----------------------------------------------------------------------

  readinput(inputfile, Set, verbose);


  return(0);
}
