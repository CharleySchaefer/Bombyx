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
  double tauL=1e-3, tauU=1e12;

  double *time,  *Grelax;
  double *omega,  *Gelas,  *Gvisc;
  double gelas, gvisc;
  double gelas_exact, gvisc_exact;

  //----------------------------------
  // Double-reptation model settings
  int N=5000;
  double Zs=100, Ze=10; 
  double tauS=1e5; 
  //----------------------------------
  
  time    =(double*)malloc(Ntau*sizeof(double));
  Grelax  =(double*)malloc(Ntau*sizeof(double));
  omega   =(double*)malloc(Ntau*sizeof(double));


  // relaxation modulus
  for(i=0; i<Ntau; i++)
  {
    time[i]=tauL*pow(tauU/tauL, (double)i/(Ntau-1));
    Grelax[i]= Grelax_stickyrouse(N, Ze, Zs, tauS, time[i]);
  }


  for(i=0; i<Ntau; i++)
  {
    omega[i]=(1.0/tauU)*pow(tauU/tauL, (double)i/(Ntau-1) );
    NobileFEA(time, Grelax, Ntau, omega[i], &gelas, &gvisc);

    gelas_exact=Gelas_stickyrouse(N, Ze, Zs, tauS, omega[i]);
    gvisc_exact=Gvisc_stickyrouse(N, Ze, Zs, tauS, omega[i]);

    printf("%15e %15e %15e %15e %15e\n", omega[i], gelas, gvisc, gelas_exact, gvisc_exact);
  }


  return(1);
}


