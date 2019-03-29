/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer
*/
#include "MaxwellModel.h"

// relaxation modulus
double Grelax_maxwell(double tau)
{
  return exp(-tau);
}

// elastic/storage modulus G'
double Gelas_maxwell(double omega)
{
  double omega2=omega*omega;
  return omega2/(1+omega2);
}

// viscous/loss modulus G''
double Gvisc_maxwell(double omega)
{
  return omega/(1+omega*omega);
}

