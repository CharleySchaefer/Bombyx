/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../ModuliLIB.h"

int main(int argc, char *argv[])
{
  int i;
  int    Ntau=200;
  double tauL=1e-8, tauU=1e3;

  double *time,  *Grelax;
  double *omega,  *Gelas,  *Gvisc;
  double gelas, gvisc;
  double gelas_exact, gvisc_exact;

  //----------------------------------
  // Double-reptation model settings
  double tol=1e-4;
  int     Ng=100, NF=100;
  double *gx, *gy;
  double *Fx, *Fy;

  // Double reptation model parameters
  double H=0.1; 
  //int NH=10;
  //double H, HL=1e-2, HU=1e2;
  //----------------------------------
  
  time=(double*)malloc(Ntau*sizeof(double));
  Grelax  =(double*)malloc(Ntau*sizeof(double));
  omega=(double*)malloc(Ntau*sizeof(double));


  // Initialise: Needed to use G_doublereptation()
  g_descloizeaux_initialise(&gx, &gy, Ng, tol);
  F_descloizeaux_initialise(&Fx, &Fy, NF, tol);


  // relaxation modulus
  for(i=0; i<Ntau; i++)
  {
    time[i]=tauL*pow(tauU/tauL, (double)i/(Ntau-1));
    Grelax[i]= G_doublereptation(gx, gy, Ng, Fx, Fy, NF, H, time[i] );
  }


  for(i=0; i<Ntau; i++)
  {
    omega[i]=(1.0/tauU)*pow(tauU/tauL, (double)i/(Ntau-1) );
    NobileFEA(time, Grelax, Ntau, omega[i], &gelas, &gvisc);



    printf("%15e %15e %15e\n", omega[i], gelas, gvisc);
  }


  return(1);
}


