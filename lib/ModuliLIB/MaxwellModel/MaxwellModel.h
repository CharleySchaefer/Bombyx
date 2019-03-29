/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer
*/
#ifndef MAXWELL_MODEL_H
#define MAXWELL_MODEL_H 1
  #include <math.h>

  // relaxation modulus
  double Grelax_maxwell(double );

  // elastic/storage modulus G'
  double Gelas_maxwell(double );

  // viscous/loss modulus G''
  double Gvisc_maxwell(double );
#endif


