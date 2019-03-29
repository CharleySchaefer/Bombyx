/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer <The University of York, UK>
*/

#ifndef BBX_PARSE_PROGRAM_ARGUMENTS_H
#define BBX_PARSE_PROGRAM_ARGUMENTS_H

  #include "../main.h"
  #include "../../lib/ModuliLIB/ModuliLIB.h"
  #include "ReadBBXdata.h"
  #include "../MonteCarlo/BBX_MC_fitting.h"

  int ParseProgramArguments(int, char *[], MCSET **, SREP **, int *);

#endif //BBX_PARSE_PROGRAM_ARGUMENTS_H
