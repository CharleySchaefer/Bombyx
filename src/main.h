/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer <The University of York, UK>
*/
#ifndef MAIN_H
  #define MAIN_H

  #include <stdio.h>
  #include <stdlib.h>
  #include <math.h>
  #include <time.h>

  #define BBX_VERSION 1.0

  // MODELLING
  #define K_BOLTZMANN 1.3806503e-23 // Boltzmann constant [J/K] 
  #include "../lib/ZiltoidLIB/ZiltoidLIB.h"
  #include "../lib/ModuliLIB/ModuliLIB.h"

  // IO
  #include "Import/import.h"
  #include "export.h"

  // MONTE CARLO FITTING ALGORITHM
  #define BBX_MONTECARLO_OUTPUT_FILE "bbx_convergence.out"
  #include "MonteCarlo/BBX_MC_fitting.h"
  #include "MonteCarlo/BBX_MC_FilterSamples.h"
#endif //MAIN_H
