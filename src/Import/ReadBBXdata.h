/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer <The University of York, UK>
*/
#ifndef READ_BBX_DATA_H
#define READ_BBX_DATA_H

#include "../../lib/ZiltoidLIB/ZiltoidLIB.h"

#ifndef MAX_LINE_WIDTH
  #define MAX_LINE_WIDTH 25000
#endif

#ifndef MAX_STR_L
  #define  MAX_STR_L 200
#endif

typedef struct
{
  int    N;   // number of monomers per chain
  double v;   // volume of a monomer in [m3]
  double phi; // volume fraction of the polymer (in solution)
  double T;   // temperature in [K]
  int    FrequencyUnits;   // 0: Angular [rad/s]; 1: Hertz [1/s]

  int    FirstDataLine;//
  int    LastDataLine;//
  int    Ndata;//
  double *w1, *Gelas,*w2, *Gvisc;

} INPUT;

  INPUT *make_bbx_input(void);
  int    free_bbx_input(INPUT *);

  #include "../MonteCarlo/BBX_MC_fitting.h"

  int readinput(char *, INPUT **, int );
#endif // READ_BBX_DATA_H
