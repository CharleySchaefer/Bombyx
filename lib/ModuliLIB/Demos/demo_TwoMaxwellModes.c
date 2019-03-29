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
  double tau1=1.0, tau2=5.0, g1=1.5, g2=2.0; 

  double *time,  *Grelax;
  double *omega,  *Gelas,  *Gvisc;
  double gelas, gvisc;
  double gelas_exact, gvisc_exact;
  
  time=(double*)malloc(Ntau*sizeof(double));
  Grelax  =(double*)malloc(Ntau*sizeof(double));
  omega=(double*)malloc(Ntau*sizeof(double));

  // relaxation modulus
  for(i=0; i<Ntau; i++)
  {
    time[i]=tauL*pow(tauU/tauL, (double)i/(Ntau-1));
    Grelax[i]=  g1*Grelax_maxwell( time[i]/tau1 ) + g2*Grelax_maxwell( time[i]/tau2 );
  }


  for(i=0; i<Ntau; i++)
  {
    omega[i]=(1.0/tauU)*pow(tauU/tauL, (double)i/(Ntau-1) );
    NobileFEA(time, Grelax, Ntau, omega[i], &gelas, &gvisc);

    gelas_exact=  g1*Gelas_maxwell( tau1*omega[i] ) + g2*Gelas_maxwell( tau2*omega[i] );
    gvisc_exact=  g1*Gvisc_maxwell( tau1*omega[i] ) + g2*Gvisc_maxwell( tau2*omega[i] );


    printf("%15e %15e %15e %15e %15e\n", omega[i], gelas, gvisc, gelas_exact, gvisc_exact);
  }


  return(1);
}


