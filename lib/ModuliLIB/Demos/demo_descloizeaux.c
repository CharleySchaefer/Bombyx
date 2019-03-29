/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../ModuliLIB.h"



int main(int argc, char argv[])
{
  printf("  -----------------\n");
  printf("  demo_descloizeaux\n");
  printf("  -----------------\n");

  int    i;
  int    Nsample;
  double xint, gint;
  double Uint, Fint;
  double tol;
  double *xsample, *gsample;
  double *Usample, *Fsample;
  FILE *ifp;

  // Tolerance required to feed to g_descloizeaux()
  int Nint = 1000; 

  //--------------------------------
  // Settings
  tol=1e-4;
  Nsample = 100;

  //--------------------------------
  // Initialise
  ifp=fopen("g_descloizeaux.out", "w");
  g_descloizeaux_initialise(&xsample, &gsample, Nsample, tol);

  // Calculate
  for (i=0; i<Nint; i++)
  {
    xint= 1e-3*pow(1e3/1e-3, (double)i/(Nint-1));
    gint=g_descloizeaux_interpolation(xsample, gsample, Nsample,  xint);
    fprintf(ifp, "%16e %16e\n", xint, gint); 
  }
  free(xsample);
  free(gsample);
  fclose(ifp);
  //--------------------------------

  //--------------------------------
  // Initialise
  ifp=fopen("F_descloizeaux.out", "w");
  F_descloizeaux_initialise(&Usample, &Fsample, Nsample, tol);

  // Calculate
  for (i=0; i<Nint; i++)
  {
    Uint= 1e-3*pow(1e1/1e-3, (double)i/(Nint-1));
    Fint=F_descloizeaux_interpolation(Usample, Fsample, Nsample,  Uint);
    fprintf(ifp, "%16e %16e\n", Uint, Fint); 
  }
  free(Usample);
  free(Fsample);
  fclose(ifp);
  //--------------------------------


  return(0);
}


